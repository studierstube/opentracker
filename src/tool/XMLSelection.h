
#ifndef __XML_SELECTION__
#define __XML_SELECTION__

#ifdef USE_TINYXML
  #include <tinyxml.h>
  #define OT_DOMDOCUMENT TiXmlDocument
  #define OT_DOMELEMENT TiXmlElement
  #ifdef WIN32
    #ifdef TINYXML_STATIC
      #ifdef _DEBUG
        // this is the static linking version of the TinyXML library
		// modified by Daniel Wagner
        #pragma comment( lib, "tinyxml_libd.lib" )
      #else
        #pragma comment( lib, "tinyxml_lib.lib" )
      #endif 
    #elif defined(TINYXML_DLL)
        // this is the dynamic linking version of the TinyXML library
		// modified by Daniel Wagner
      #ifdef _DEBUG
        #pragma comment( lib, "tinyxml_dlld.lib" )
      #else
        #pragma comment( lib, "tinyxml_dll.lib" )
      #endif 
    #else
        // this is the default version of the TinyXML library
      #ifdef _DEBUG
        #pragma comment( lib, "tinyxmld.lib" )
      #else
        #pragma comment( lib, "tinyxml.lib" )
      #endif 
    #endif
  #endif
#else
  #include <xercesc/dom/DOM.hpp>
  #define OT_DOMDOCUMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument
  #define OT_DOMELEMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMElement
  #ifndef USE_XERCES
    #define USE_XERCES 1
  #endif
  #ifdef WIN32
    #ifdef _DEBUG
        #pragma comment( lib, "xerces-c_2d.lib" )
    #else
        #pragma comment( lib, "xerces-c_2.lib" )
    #endif 
  #endif
#endif //USE_TINYXML


#endif //__XML_SELECTION__
