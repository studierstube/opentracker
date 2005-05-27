 /* ========================================================================
  * PROJECT: DirectShow Video Processing Library
  * File:    CDsRenderer implementation
  * Version: 0.0.6 (08/16/2003)
  * ========================================================================
  * Author:  Thomas Pintaric, Vienna University of Technology
  * Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
  * =======================================================================
  *
  * Copyright (C) 2003  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Thomas Pintaric under
  * <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================*/

#pragma warning( disable:4514)

#include "DsRenderer.h"
#include "GlDebug.h"

#include "CPU.h"
// ---------------------------------------------------------------------------
HRESULT LoadSurfaceFromMemory(LPDIRECT3DTEXTURE9 texture,BYTE* pData,int width, int height, D3DFORMAT format)
{
	LPDIRECT3DSURFACE9 tSurface;
	if(FAILED(texture->GetSurfaceLevel(0,&tSurface))) return(E_FAIL);

	RECT dest_rect;
	dest_rect.top = 0;			
	dest_rect.left = 0;
	dest_rect.bottom = height;
	dest_rect.right = width;
	
	int bytes_per_pixel = PXBitsPerPixel(format)/8;
//----
	D3DLOCKED_RECT pLockedRect;
	if(FAILED(tSurface->LockRect(&pLockedRect,&dest_rect,0))) return(E_FAIL);
	for(int i=0; i<height; i++)
	{
		CopyMemory((BYTE*)pLockedRect.pBits + (pLockedRect.Pitch * i), 
			pData + (width*i*bytes_per_pixel), width*bytes_per_pixel);
	}
	if(FAILED(tSurface->UnlockRect())) return(E_FAIL);
	return(S_OK);
}

//-------------------------------------------------------------------------------------------------
//
// Common DLL routines and tables
//
// Object table - all com objects in this DLL
CFactoryTemplate g_Templates[]=
{   { L"TU-Vienna DirectShow Renderer"
    , &CLSID_DsRenderer
    , CDsRenderer::CreateInstance
    , NULL
    , &CDsRenderer::sudFilter 
    }
};
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);

//-------------------------------------------------------------------------------------------------
// The streams.h DLL entrypoint.
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);
//-------------------------------------------------------------------------------------------------

// The entrypoint required by the MSVC runtimes. This is used instead
// of DllEntryPoint directly to ensure global C++ classes get initialised.
BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved) {

    return DllEntryPoint(reinterpret_cast<HINSTANCE>(hDllHandle), dwReason, lpreserved);
}

//-------------------------------------------------------------------------------------------------
// Setup information

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};
//-------------------------------------------------------------------------------------------------

const AMOVIESETUP_PIN sudpPins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    }
};

//-------------------------------------------------------------------------------------------------
// setup data - allows the self-registration to work.
const AMOVIESETUP_FILTER CDsRenderer::sudFilter = {
    g_Templates[0].m_ClsID
  , g_Templates[0].m_Name
  , MERIT_DO_NOT_USE
  , 1                         // nPins
  , sudpPins	              // lpPin
};  

//-------------------------------------------------------------------------------------------------
// Constructor

CDsRenderer::CDsRenderer(REFCLSID RenderClass, // CLSID for this renderer
				 TCHAR *pName,         // Debug ONLY description
				 LPUNKNOWN pUnk,       // Aggregated owner object
				 HRESULT *phr) :       // General OLE return code

	CBaseVideoRenderer(RenderClass,pName,pUnk,phr),
    CPersistStream(pUnk, phr),
	m_pRendererInputPin(NULL),
	PixelFormat(defaultPixelformat),
	Timestamp(INVALID_TIMESTAMP),
	SampleNumber(0),
	m_hEvent(NULL),
	m_pLock(NULL)
{
	#ifdef _DEBUG
		DbgInitialise((struct HINSTANCE__ *) GetModuleHandle(DsRendererFileName));
		DbgSetModuleLevel(LOG_TRACE,5);
		DbgSetModuleLevel(LOG_MEMORY,5);
		DbgSetModuleLevel(LOG_ERROR,5);
		DbgSetModuleLevel(LOG_TIMING,5);
		DbgSetModuleLevel(LOG_LOCKING,5);
	#endif

		CPU_SetupFeatureFlag();
}

//-------------------------------------------------------------------------------------------------

CDsRenderer::~CDsRenderer()
{}

//-------------------------------------------------------------------------------------------------
// CreateInstance
//
// Provide the way for COM to create a CDsRenderer object
CUnknown * WINAPI CDsRenderer::CreateInstance(LPUNKNOWN punk, HRESULT *phr) {

    CDsRenderer *pNewObject = new CDsRenderer(CLSID_DsRenderer,NAME("TU-Vienna DirectShow Renderer"), punk, phr );
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }

    return pNewObject;
} // CreateInstance

//-------------------------------------------------------------------------------------------------
STDAPI
DllRegisterServer()
{
  return AMovieDllRegisterServer2( TRUE );
}

STDAPI
DllUnregisterServer()
{
  return AMovieDllRegisterServer2( FALSE );
}

//-------------------------------------------------------------------------------------------------
// Enforce MediaType restrictions

HRESULT CDsRenderer::CheckMediaType(const CMediaType *mtIn)
{
	//override
	//return(S_OK);

    if ((*mtIn->FormatType() != FORMAT_VideoInfo) && 
//		(*mtIn->FormatType() != FORMAT_VideoInfo2))
		(true))

        return E_INVALIDARG;
	if(CheckPixelFormat(mtIn)) return(NOERROR);
	else return(E_FAIL);
}

//-------------------------------------------------------------------------------------------------
// Check if the stream is compliant with our chosen PIXELFORMAT
//
BOOL CDsRenderer::CheckPixelFormat(const CMediaType *pMediaType) const
{
	if (IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Video) &&
		IsEqualGUID(*pMediaType->Subtype(), PXtoMEDIASUBTYPE(PixelFormat)))
		{
			VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
			return(TRUE);
		}
    else return(FALSE);

}
//-------------------------------------------------------------------------------------------------
//
// NonDelegatingQueryInterface
//
// Reveals IID_IRendererParam
//

STDMETHODIMP CDsRenderer::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);
   if (riid == IID_IRendererParam) 
				return GetInterface((IRendererParam *) this, ppv);
   else if (riid == IID_IRenderTargetOpenGL) 
				return GetInterface((IRenderTargetOpenGL *) this, ppv);
   else if (riid == IID_IRenderTargetDirect3D) 
				return GetInterface((IRenderTargetDirect3D *) this, ppv);
   else if (riid == IID_IRenderTargetUserCallback) 
				return GetInterface((IRenderTargetUserCallback *) this, ppv);
   else if (riid == IID_IRenderTargetMemoryBuffer)
        return GetInterface((IRenderTargetMemoryBuffer *) this, ppv);
   else if (riid == IID_IRendererVFx)
        return GetInterface((IRendererVFx *) this, ppv);
   else return CBaseVideoRenderer::NonDelegatingQueryInterface(riid, ppv);

} // NonDelegatingQueryInterface

//-----------------------------------------------------------------------------
// GetClassID
// This is the only method of IPersist
//
STDMETHODIMP CDsRenderer::GetClassID(CLSID *pClsid)
{
    return CBaseFilter::GetClassID(pClsid);
} // GetClassID

//-----------------------------------------------------------------------------

#define WRITEOUT(var)  hr = pStream->Write(&var, sizeof(var), NULL); \
		       if (FAILED(hr)) return hr;

#define READIN(var)    hr = pStream->Read(&var, sizeof(var), NULL); \
		       if (FAILED(hr)) return hr;

//-------------------------------------------------------------------------------------------------
//
// ScribbleToStream
//
// Overriden to write our state into a stream
//
HRESULT CDsRenderer::WriteToStream(IStream *pStream)
{
	HRESULT hr;
    WRITEOUT(PixelFormat);
    return NOERROR;
} // ScribbleToStream

//-------------------------------------------------------------------------------------------------
//
// ReadFromStream
//
// Likewise overriden to restore our state from a stream
//
HRESULT CDsRenderer::ReadFromStream(IStream *pStream)
{
	HRESULT hr;
    READIN(PixelFormat);
    return NOERROR;
} // ReadFromStream


//-------------------------------------------------------------------------------------------------

CBasePin* CDsRenderer::GetPin(int n)
{
    CAutoLock cObjectCreationLock(&m_ObjectCreationLock);

    // Should only ever be called with zero
    ASSERT(n == 0);

    if (n != 0) {
        return NULL;
    }

    // Create the input pin if not already done so

    if (m_pRendererInputPin == NULL) 
	{

        // hr must be initialized to NOERROR because
        // CRendererInputPin's constructor only changes
        // hr's value if an error occurs.
        HRESULT hr = NOERROR;

        m_pRendererInputPin = new CDsRendererInputPin(this,&hr,L"In");
        if (m_pRendererInputPin == NULL) {
            return NULL;
        }

        if (FAILED(hr)) {
            delete m_pRendererInputPin;
            m_pRendererInputPin = NULL;
            return NULL;
        }

		 m_pInputPin = m_pRendererInputPin;
    }
    return m_pInputPin;
}

//-------------------------------------------------------------------------------------------------

// IRendererParam Interface Methods ------------------------------------------

//STDMETHODIMP CDsRenderer::setSyncPrimitives(CCritSec *_Lock,LPTSTR _ESyncName)
STDMETHODIMP CDsRenderer::setSyncPrimitives(CComCriticalSection *_Lock, HANDLE _hSyncEvent)
{
	bool no_error = DuplicateHandle(GetCurrentProcess(),_hSyncEvent,
								    GetCurrentProcess(), &m_hEvent,
								    0, TRUE, DUPLICATE_SAME_ACCESS);
#ifdef _DEBUG
	ASSERT(no_error);
#endif
//	m_hEvent = _hSyncEvent;
//	m_hEvent = OpenEvent(EVENT_ALL_ACCESS,TRUE,_ESyncName);
	m_pLock = _Lock;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::setPixelFormat(const PIXELFORMAT _pixelFormat)
{
	PixelFormat = _pixelFormat;
	SetDirty(TRUE);
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::getPixelFormat(PIXELFORMAT *_PixelFormat)
{
    CheckPointer(_PixelFormat,E_POINTER);
	*_PixelFormat = PixelFormat;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::getFrameSize(unsigned int *_Width, unsigned int *_Height)
{
	if(!m_pRendererInputPin->IsConnected())
	{
		(*_Width) = 0;
		(*_Height) = 0;
		return(E_FAIL);
	}
	CMediaType mediaType = m_pRendererInputPin->CurrentMediaType();
    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) mediaType.Format();
    (*_Width)   = (unsigned int) pvi->bmiHeader.biWidth;
    (*_Height)  = (unsigned int) pvi->bmiHeader.biHeight;

	return(S_OK);
}

STDMETHODIMP CDsRenderer::getInterlacing(bool *_isInterlaced)
{
	if(!m_pRendererInputPin->IsConnected())
	{
		(*_isInterlaced) = false;
		return(E_FAIL);
	}
	CMediaType mediaType = m_pRendererInputPin->CurrentMediaType();
	if(*mediaType.FormatType() != FORMAT_VideoInfo2)
	{
		(*_isInterlaced) = false;
		return(E_FAIL);
	}
	*_isInterlaced = IsInterlaced(&mediaType);
	return(S_OK);
}

STDMETHODIMP CDsRenderer::getCurrentTimestamp(REFERENCE_TIME *_Timestamp)
{
    CheckPointer(_Timestamp,E_POINTER);
	*_Timestamp = Timestamp;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::setAllocatorBuffers(unsigned int _num_alloc)
{
	HRESULT hr;
	if(!m_pRendererInputPin) return(E_FAIL);
	CComPtr<IMemAllocator> pAllocator = NULL;
	if(FAILED(hr = m_pRendererInputPin->GetAllocator(&pAllocator))) return(hr);
	if(FAILED(hr = pAllocator->Decommit())) return(hr);
	ALLOCATOR_PROPERTIES requestedProperties;
	ALLOCATOR_PROPERTIES actualProperties;
	pAllocator->GetProperties(&requestedProperties);
	if(requestedProperties.cBuffers != _num_alloc) requestedProperties.cBuffers = _num_alloc;
	hr = pAllocator->SetProperties(&requestedProperties,&actualProperties);
	return(hr);
}

STDMETHODIMP CDsRenderer::getAllocatorBuffers(unsigned int *_num_alloc)
{
	HRESULT hr;
	CComPtr<IMemAllocator> pAllocator = NULL;
	if(FAILED(hr = m_pRendererInputPin->GetAllocator(&pAllocator))) return(hr);
	if(FAILED(hr = pAllocator->Decommit())) return(hr);
	ALLOCATOR_PROPERTIES requestedProperties;
	pAllocator->GetProperties(&requestedProperties);
	*_num_alloc = requestedProperties.cBuffers;
	return(S_OK);
}

// IRenderTargetOpenGL Interface Methods ------------------------------------------

STDMETHODIMP CDsRenderer::setOpenGLContext(const HGLRC _parentRC, const HDC _parentDC)
{
	ParentDC = _parentDC;
	ParentRC = _parentRC;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::setOpenGLTextureTarget(const int _openGLTexture)
{
	OpenGLTexture = _openGLTexture;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::enableOpenGLRenderTarget(const bool _enableOpenGL)
{
	OpenGLTargetEnabled = _enableOpenGL;
	return(NOERROR);
}

// IRenderTargetDirect3D Interface Methods ------------------------------------------

STDMETHODIMP CDsRenderer::setDirect3DTextureTarget(IDirect3DTexture9 *_d3DTexture)
{
	D3DTexture = _d3DTexture;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::enableDirect3DRenderTarget(const bool _enableDirect3D)
{
	Direct3DTargetEnabled = _enableDirect3D;
	return(NOERROR);
}

// IRenderTargetUserCallback Interface Methods ------------------------------------------

STDMETHODIMP CDsRenderer::setUserCallback(PTR_UserCallback _userCallback)
{
	userCallback = _userCallback;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::setUserCallback(PTR_SimpleUserCallback _simpleUserCallback)
{
	simpleUserCallback = _simpleUserCallback;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::enableUserCallback(const bool _enableCallback)
{
	UserCallbackEnabled = _enableCallback;
	return(NOERROR);
}

// CQueuedMemoryBuffer Methods ------------------------------------------

CQueuedMemoryBuffer::CQueuedMemoryBuffer(IMediaSample* _pSample,
										 BYTE* _pixel_buffer,
										 unsigned int _width, 
										 unsigned int _height,
										 PIXELFORMAT _pixel_format,
										 REFERENCE_TIME _timestamp,
										 unsigned long _sample_number) :
	pSample(_pSample),
	pLock(new CComCriticalSection()),
	instances(0),
	pixel_buffer(_pixel_buffer),
	pixel_format(_pixel_format),
	width(_width),
	height(_height),
	timestamp(_timestamp),
	sample_number(_sample_number)
{
	pLock->Init();
}

CQueuedMemoryBuffer::~CQueuedMemoryBuffer()
{
	pSample->Release();
	pLock->Term();
	delete pLock;
}

MemoryBufferHandle CQueuedMemoryBuffer::GetHandle()
{
	MemoryBufferHandle handle;
	handle.n = sample_number;
	handle.t = timestamp;
	return(handle);
}

bool CQueuedMemoryBuffer::MatchesHandle(MemoryBufferHandle handle)
{
	return((handle.t == timestamp) && (handle.n == sample_number));
}


// IRenderTargetMemoryBuffer Interface Methods ------------------------------------------

STDMETHODIMP CDsRenderer::checkoutMemoryBuffer(MemoryBufferHandle *pHandle,
											   BYTE** _memoryBuffer,
											   unsigned int* _Width,
											   unsigned int* _Height,
											   PIXELFORMAT* _PixelFormat,
											   REFERENCE_TIME* _Timestamp)
{
	//CAutoLock cAutolock(qsLock);
	qsLock->Lock();
    CheckPointer(pHandle,E_POINTER);
	
	if(MemoryBufferList.size() == 0)
	{
		qsLock->Unlock();
		return(E_FAIL);
	}
		// first frame hasn't been decoded yet

	MemoryBufferList.sort(MEMORY_BUFFER_SORTING_FUNC);
	TMemoryBufferList::iterator mbIterator = MemoryBufferList.begin();
	CQueuedMemoryBuffer* pQMB = (*mbIterator);

// --- debug test ---
#ifdef _DEBUG
	CQueuedMemoryBuffer* dbg_b[DEFAULT_NUMBER_OF_ALLOCATOR_BUFFERS];
	TMemoryBufferList::iterator dbg_Iterator = MemoryBufferList.begin();
	int i=0;
	for(dbg_Iterator = MemoryBufferList.begin();
		dbg_Iterator != MemoryBufferList.end();
		dbg_Iterator++)
	{
		dbg_b[i] = (*dbg_Iterator);
		i++;
	}
#endif
// -----------------

	pQMB->instances++;

	(*pHandle) = pQMB->GetHandle();

	if(_memoryBuffer) (*_memoryBuffer) = pQMB->pixel_buffer;
	if(_Width)		  (*_Width) = pQMB->width;
	if(_Height)		  (*_Height) = pQMB->height;
	if(_PixelFormat)  (*_PixelFormat) = pQMB->pixel_format;
	if(_Timestamp)	  (*_Timestamp) = pQMB->timestamp;

	qsLock->Unlock();
	return(S_OK);
}

STDMETHODIMP CDsRenderer::checkinMemoryBuffer(MemoryBufferHandle Handle,
											  const bool _forceRelease)
{
	qsLock->Lock();
	//CAutoLock cAutolock(qsLock);

	TMemoryBufferList::iterator mbIterator = MemoryBufferList.begin();
	for(mbIterator=MemoryBufferList.begin(); 
		mbIterator != MemoryBufferList.end();
		++mbIterator)
	{
		CQueuedMemoryBuffer *_pQMB = (*mbIterator);
		if(_pQMB->MatchesHandle(Handle))
		{
			if(_pQMB->instances > 0)
			{
				_pQMB->instances = (_forceRelease ? 0 : (_pQMB->instances-1));
				qsLock->Unlock();
				return(S_OK);
			}
			else 
			{
				qsLock->Unlock();
				return(E_FAIL); // instances == 0
			}
		}
	}

	qsLock->Unlock();
	return(E_FAIL); // handle not found
}

STDMETHODIMP CDsRenderer::checkoutQueuedMediaSample(MemoryBufferHandle *pHandle,
													IMediaSample** _mediaSample)
{
	qsLock->Lock();
//	CAutoLock cAutolock(qsLock);
    CheckPointer(pHandle,E_POINTER);
    CheckPointer(_mediaSample,E_POINTER);

	if(MemoryBufferList.size() == 0)
	{
		qsLock->Unlock();
		return(E_FAIL);
		// first frame hasn't been decoded yet
	}

	TMemoryBufferList::iterator mbIterator = MemoryBufferList.begin();
	CQueuedMemoryBuffer* pQMB = (*mbIterator);

	pQMB->instances++;
	(*_mediaSample) = pQMB->pSample;

	qsLock->Unlock();
	return(S_OK);
}

STDMETHODIMP CDsRenderer::checkinQueuedMediaSample(MemoryBufferHandle Handle,
												   const bool _forceRelease)
{
	return(checkinMemoryBuffer(Handle,_forceRelease));
}

STDMETHODIMP CDsRenderer::enableMemoryBuffer(const bool _enableMemoryBuffer,
											 unsigned int _allocatorBuffers)
{
	MemoryBufferEnabled = _enableMemoryBuffer;
	AllocatorBuffers = _allocatorBuffers;

	HRESULT hr;
	unsigned int alloc_buffers;
	if(FAILED(hr = getAllocatorBuffers(&alloc_buffers))) return(hr);
	if(alloc_buffers < AllocatorBuffers) 
		return(setAllocatorBuffers(AllocatorBuffers));
	else return(S_OK);
}

// IRendererVideoProc Interface Methods ------------------------------------------

STDMETHODIMP CDsRenderer::set_VFxDeinterlaceState(VFX_STATE _state)
{
	CVFxDeinterlace* vfx_d = (CVFxDeinterlace*) FindVFx(DEINTERLACE_internal_name);
	if(vfx_d == NULL)
	{
		vfx_d = new CVFxDeinterlace(new CVFxDeinterlaceParameter());
		AddVFx(vfx_d);
	}

	CVFxDeinterlaceParameter* param = (CVFxDeinterlaceParameter*) vfx_d->GetParameters();
	param->state = _state;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::set_VFxDeinterlaceMethod(DEINTERLACE_METHOD _method)
{
	CVFxDeinterlace* vfx_d = (CVFxDeinterlace*) FindVFx(DEINTERLACE_internal_name);
	if(vfx_d == NULL)
	{
		vfx_d = new CVFxDeinterlace(new CVFxDeinterlaceParameter());
		AddVFx(vfx_d);
	}

	CVFxDeinterlaceParameter* param = (CVFxDeinterlaceParameter*) vfx_d->GetParameters();
	param->method = _method;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::get_VFxDeinterlaceParam(VFX_STATE* _state, DEINTERLACE_METHOD* _method)
{
	CVFxDeinterlace* vfx_d = (CVFxDeinterlace*) FindVFx(DEINTERLACE_internal_name);
	if(vfx_d == NULL) return(E_FAIL);

	CVFxDeinterlaceParameter* param = (CVFxDeinterlaceParameter*) vfx_d->GetParameters();
	*_state = param->state;
	*_method = param->method;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::set_VFxFlipState(VFX_STATE _state)
{
	CVFxFlip* vfx_f = (CVFxFlip*) FindVFx(FLIP_internal_name);
	if(vfx_f == NULL)
	{
		vfx_f = new CVFxFlip(new CVFxFlipParameter());
		AddVFx(vfx_f);
	}

	CVFxFlipParameter* param = (CVFxFlipParameter*) vfx_f->GetParameters();
	param->state = _state;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::set_VFxFlipMethod(bool _flip_h, bool _flip_v)
{
	CVFxFlip* vfx_f = (CVFxFlip*) FindVFx(FLIP_internal_name);
	if(vfx_f == NULL)
	{
		vfx_f = new CVFxFlip(new CVFxFlipParameter());
		AddVFx(vfx_f);
	}

	CVFxFlipParameter* param = (CVFxFlipParameter*) vfx_f->GetParameters();
	param->flip_h = _flip_h;
	param->flip_v = _flip_v;
	return(NOERROR);
}

STDMETHODIMP CDsRenderer::get_VFxFlipParam(VFX_STATE* _state, bool* _flip_h, bool* _flip_v)
{
	CVFxFlip* vfx_f = (CVFxFlip*) FindVFx(FLIP_internal_name);
	if(vfx_f == NULL) return(E_FAIL);

	CVFxFlipParameter* param = (CVFxFlipParameter*) vfx_f->GetParameters();
	*_state = param->state;
	*_flip_h = param->flip_h;
	*_flip_v = param->flip_v;
	return(NOERROR);
}

// CVFxList Methods -------------------------------------------------------------

/*
namespace std
{
   template <>
   struct greater<CVFxBase *> : public binary_function<CVFxBase *, CVFxBase *, bool>
   {
        operator()(CVFxBase * a, CVFxBase * b)
        {
             if (!a) return true;
             if (!b) return false;
			 return (a->GetPriority()) > (b->GetPriority());
        }
   };
};
*/

// ---------------------------------
CVFxList::CVFxList()
{}

// ---------------------------------
CVFxList::~CVFxList()
{
	ClearVFxList();
}

// ---------------------------------
void CVFxList::AddVFx(CVFxBase* vfx)
{
	vfxList.push_front(vfx);
	vfxList.sort(VFX_BUFFER_SORTING_FUNC);
}

// ---------------------------------
CVFxBase* CVFxList::FindVFx(const char* internal_name)
{
	if(vfxList.empty()) return(NULL); 

	std::list<CVFxBase*>::iterator iter;
	iter = vfxList.begin();

	do 
	{
		if(strcmp(internal_name,(*iter)->GetInternalName()) == 0)
			return(*iter);
		else iter++;
	} while(iter != vfxList.end());

	return(NULL);
}

// ---------------------------------
HRESULT CVFxList::ApplyVFx(CVFxMediaSample pVFxSample)
{
	HRESULT hr;
	if(vfxList.empty()) return(S_OK); 

	std::list<CVFxBase*>::iterator iter;
	iter = vfxList.begin();

	do 
	{
		hr = (*iter)->ApplyEffect(pVFxSample);
		if(FAILED(hr))
		{
			// so some error handling...
		}
		iter++;
	} while(iter != vfxList.end());

	return(S_OK);
}

// ---------------------------------
void CVFxList::ClearVFxList()
{
	while(!vfxList.empty())
	{
		CVFxBase* p = vfxList.front();
		delete p;
		vfxList.pop_front();
	}
	vfxList.clear();
}



//-------------------------------------------------------------------------------------------------
HRESULT CRenderTargetOpenGL::EstablishSharedOpenGLContext(CComCriticalSection *Lock, HANDLE syncEvent)
{
	if(Lock == NULL) return(E_FAIL); // no sync, no fun :(

	// make sure to assign the SAME critical section primitive (CSGLRCSetup)
	// to all instances of this filter

	// ----------------------------------
	// parent process:
	// ----------------------------------
	// wglMakeCurrent(hDC,hRC);
	// glEnable(GL_TEXTURE_2D);
	// ...
	// wglMakeCurrent(NULL,NULL);
	// g_MediaControl->Run();					// will eventually call this method
	// WaitForSingleObject(Event,INFINITE);
	// ----------------------------------

	if(LocalDC != NULL && LocalDC != ParentDC)
	{
		// parent process has created a new device context
		if (!wglMakeCurrent(NULL,NULL)) ShowOpenGLErrorDetails();
		if (!wglDeleteContext(LocalRC)) ShowOpenGLErrorDetails();
		LocalRC = NULL;
		// ??LocalDC = NULL;
	}

	if(LocalRC == NULL)
	{
		//-------------------------------------------
		Lock->Lock();
		LocalDC = ParentDC; // ParentDC must not be current!
		LocalRC = wglCreateContext(LocalDC);
		if(!wglShareLists(ParentRC,LocalRC)) ShowOpenGLErrorDetails();
		if(!wglMakeCurrent(LocalDC, LocalRC)) ShowOpenGLErrorDetails();
		Lock->Unlock();

		int event_result = SetEvent(syncEvent);
		// parent process can continue
		//-------------------------------------------
		glEnable(GL_TEXTURE_2D);
		return(S_OK);
	}
	return(S_OK);
}

//-------------------------------------------------------------------------------------------------
void CDsRenderer::OnRenderStart(IMediaSample *pMediaSample)
{
	if((OpenGLTargetEnabled) && (OpenGLTexture != INVALID_TEXTURE))
		EstablishSharedOpenGLContext(m_pLock, m_hEvent);
}

//-------------------------------------------------------------------------------------------------
HRESULT CDsRenderer::DoRenderSample(IMediaSample *pSample)
{
	if(m_pLock != NULL)
		qsLock->Lock();
	    //CAutoLock cAutolock(m_pLock);
	else return(S_OK); // no sync, no fun :(

	HRESULT hr;
	SampleNumber++;

	BYTE* PixelBuffer = NULL;
	pSample->GetPointer(&PixelBuffer);
	CMediaType* pType = &m_pRendererInputPin->CurrentMediaType();
    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pType->pbFormat;
		// VIDEOINFOHEADER2 is backwards compatible with VIDEOINFOHEADER, so it's safe to typecast
    DWORD Width    = pvi->bmiHeader.biWidth;
    DWORD Height   = pvi->bmiHeader.biHeight;

	ApplyVFx(CVFxMediaSample(pSample,pType));

	REFERENCE_TIME Rt;
	hr = pSample->GetMediaTime(&Timestamp,&Rt);   // we just care about the start
	if(FAILED(hr))								  // probably VFW_E_MEDIA_TIME_NOT_SET
	{
		if(m_pClock) m_pClock->GetTime(&Timestamp);
		else Timestamp += pvi->AvgTimePerFrame;		// ...whatever!
	}

	if(UserCallbackEnabled)   RenderToUserCallback(pSample,PixelBuffer,Width,Height,PixelFormat,Timestamp);
	if(MemoryBufferEnabled)   RenderToMemoryBuffer(pSample,PixelBuffer,Width,Height,PixelFormat,Timestamp,SampleNumber);
	if(Direct3DTargetEnabled) RenderToDirect3DTexture(PixelBuffer,Width,Height,PixelFormat);
	if(OpenGLTargetEnabled)   RenderToOpenGLTexture(PixelBuffer,Width,Height,PixelFormat);

	// -----------------------------------------------------------------------------------
	if(m_hEvent != NULL && LocalDC == ParentDC) SetEvent(m_hEvent);
	// re-use the same event handle for "frame done" and "change device context" messages

	qsLock->Unlock();
	return(S_OK);
}

//******************************************************************************************************

HRESULT CRenderTargetOpenGL::RenderToOpenGLTexture(BYTE* _PixelBuffer,
												   unsigned int _Width, 
												   unsigned int _Height,
												   PIXELFORMAT _PixelFormat)
{
	if((OpenGLTexture == INVALID_TEXTURE) || (LocalDC != ParentDC)) return(E_FAIL);

	glBindTexture(GL_TEXTURE_2D, OpenGLTexture);
	glTexSubImage2D(GL_TEXTURE_2D,
					0,
					0,
					0,
					_Width,
					_Height,
					PXtoOpenGL(_PixelFormat),
					GL_UNSIGNED_BYTE,
					(void*)_PixelBuffer);

	DWORD Error = glGetError();

#ifdef DEBUG
	if(Error != GL_NO_ERROR) ShowOpenGLErrorDetails("glTexSubImage2D() failed.");
#endif

	return(Error == GL_NO_ERROR ? S_OK : E_FAIL);
}

//-----------------------------------------------------------------------------------------------

HRESULT CRenderTargetDirect3D::RenderToDirect3DTexture(BYTE* _PixelBuffer,
													   unsigned int _Width, 
												       unsigned int _Height,
												       PIXELFORMAT _PixelFormat)
{
	if(!D3DTexture) return(E_FAIL);
	return(LoadSurfaceFromMemory(D3DTexture,
								 _PixelBuffer,
								 _Width,
								 _Height,
								 PXtoD3D(_PixelFormat)));
}

//-----------------------------------------------------------------------------------------------

HRESULT CRenderTargetUserCallback::RenderToUserCallback(IMediaSample *pMediaSample,
														BYTE* _PixelBuffer,
														unsigned int _Width, 
														unsigned int _Height,
														PIXELFORMAT _PixelFormat,
														REFERENCE_TIME _Timestamp)
{
	if(userCallback != NULL) userCallback(pMediaSample);
	if(simpleUserCallback != NULL) 
			simpleUserCallback(_PixelBuffer,_Width,_Height,_PixelFormat,_Timestamp);
	return(S_OK);
}

//-----------------------------------------------------------------------------------------------

HRESULT CRenderTargetMemoryBuffer::RenderToMemoryBuffer(IMediaSample* pMediaSample,
														BYTE* _PixelBuffer,
														unsigned int _Width, 
												        unsigned int _Height,
														PIXELFORMAT _PixelFormat,
														REFERENCE_TIME _Timestamp,
														unsigned long _SampleNumber)
{
	//CAutoLock cAutolock(qsLock);
	qsLock->Lock();


/*
	if(QueuedSampleInstances > 0) return(S_OK); // drop sample
	if(QueuedSample != NULL) 
		if(FAILED(hr = QueuedSample->Release())) return(hr);
	QueuedSample = pMediaSample;
	ULONG refCount = QueuedSample->AddRef();
	QueuedSampleData = _PixelBuffer;
	QueuedSampleWidth = _Width;
	QueuedSampleHeight = _Height;
	QueuedSampleFormat = _PixelFormat;
	QueuedSampleTimestamp = _Timestamp;
*/
	CQueuedMemoryBuffer *pQMB = new CQueuedMemoryBuffer(pMediaSample,
											            _PixelBuffer,
														_Width,
														_Height,
														_PixelFormat,
														_Timestamp,
														_SampleNumber);


	TMemoryBufferList tempMemoryBufferList;
	
	// remove unreferenced old samples
	TMemoryBufferList::iterator mbIterator = MemoryBufferList.begin();
	for(mbIterator=MemoryBufferList.begin(); 
		mbIterator != MemoryBufferList.end();
		mbIterator++)
	{
		CQueuedMemoryBuffer *_pQMB = (*mbIterator);
		if(_pQMB->instances <= 0) delete _pQMB;
		else tempMemoryBufferList.push_back(_pQMB);
	}

	MemoryBufferList.erase(MemoryBufferList.begin(),MemoryBufferList.end());
	MemoryBufferList = tempMemoryBufferList;

	if(MemoryBufferList.size() < AllocatorBuffers)
	{
		MemoryBufferList.push_back(pQMB);
		ULONG refCount = pQMB->pSample->AddRef();
		MemoryBufferList.sort(MEMORY_BUFFER_SORTING_FUNC);
		qsLock->Unlock();
		return(S_OK);
	}
	else
	{
		MemoryBufferList.sort(MEMORY_BUFFER_SORTING_FUNC);
		qsLock->Unlock();
		return(S_OK); // drop sample
	}
}

//-----------------------------------------------------------------------------------------------


