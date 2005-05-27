#include "reverb.h"

#include <string.h>
#include <stdlib.h>

/*
    Pre-verb stuff
*/
REVERBTAP		PreverbTap[PREVERB_NUMTAPS];

/*
    Reverb stuff
*/
REVERBTAP		ReverbTap[REVERB_NUMTAPS];

extern int outputfreq;

/*
[
	[DESCRIPTION]
	Callback to mix in one reverb tap.  It copies the buffer into its own history buffer also.

	[PARAMETERS]
	'originalbuffer'	Pointer to the original mixbuffer, not any buffers passed down 
						through the dsp chain.  They are in newbuffer.
	'newbuffer'			Pointer to buffer passed from previous DSP unit.
	'length'			Length in SAMPLES of buffer being passed.
	'param'				User parameter.  In this case it is a pointer to LowPassBuffer.
 
	[RETURN_VALUE]
	a pointer to the buffer that was passed in, with a tap mixed into it.

	[REMARKS]
]
*/
void * F_CALLBACKAPI ReverbCallback(void *originalbuffer, void *newbuffer, int length, int param)
{
	int		mixertype = FSOUND_GetMixer();
	REVERBTAP	*tap = (REVERBTAP *)param;

	// must be 16bit stereo integer buffer.. sorry blendmode (32bit) and fpu (32bit float) dont support this.
	if (mixertype == FSOUND_MIXER_BLENDMODE || 
		mixertype == FSOUND_MIXER_QUALITY_FPU)
    {
		return newbuffer;
    }

#define FEEDBACK 1	// this determines whether to use already pre-reverbed data for the tap, or the original clean buffer

	// reverb history buffer is a ringbuffer.  If the length makes the copy wrap, then split the copy 
	// into end part, and start part.. 
	if (tap->historyoffset + length > tap->historylen)
	{
		int taillen = tap->historylen - tap->historyoffset;
		int startlen = length - taillen;

		// mix a scaled version of history buffer into output
		FSOUND_DSP_MixBuffers(newbuffer, tap->historybuff + (tap->historyoffset << 2), taillen, outputfreq, tap->volume, tap->pan, FSOUND_STEREO | FSOUND_16BITS);
		FSOUND_DSP_MixBuffers((char *)newbuffer+((tap->historylen - tap->historyoffset) << 2), tap->historybuff, startlen, outputfreq, tap->volume, tap->pan, FSOUND_STEREO | FSOUND_16BITS);

		// now copy input into reverb/history buffer 
#ifdef FEEDBACK
		memcpy(tap->historybuff + (tap->historyoffset << 2), newbuffer, taillen<<2);
		memcpy(tap->historybuff, (char *)newbuffer + ((tap->historylen - tap->historyoffset) << 2), startlen << 2);
#else
		memcpy(tap->historybuff + (tap->historyoffset << 2), originalbuffer, taillen<<2);
		memcpy(tap->historybuff, (char *)originalbuffer + ((tap->historylen - tap->historyoffset) << 2), startlen << 2);
#endif

	}
	// no wrapping reverb buffer, just write dest
	else
	{
		// mix a scaled version of history buffer into output
		FSOUND_DSP_MixBuffers(newbuffer, tap->historybuff + (tap->historyoffset << 2), length, outputfreq, tap->volume, tap->pan, FSOUND_STEREO | FSOUND_16BITS);

		// now copy input into reverb/history buffer 
#ifdef FEEDBACK
		memcpy(tap->historybuff + (tap->historyoffset << 2), newbuffer, length << 2);
#else
		memcpy(tap->historybuff + (tap->historyoffset << 2), originalbuffer, length << 2);
#endif
	}


	tap->historyoffset += length;
	if (tap->historyoffset >= tap->historylen) 
    {
		tap->historyoffset -= tap->historylen;
    }

	// reverb history has been mixed into new buffer, so return it.
	return newbuffer;
}


/*
[
	[DESCRIPTION]
	Similair to a reverb tap except the history copy and the tap mix are done back to front.

	[PARAMETERS]
	'originalbuffer'	Pointer to the original mixbuffer, not any buffers passed down 
						through the dsp chain.  They are in newbuffer.
	'newbuffer'			Pointer to buffer passed from previous DSP unit.
	'length'			Length in SAMPLES of buffer being passed.
	'param'				User parameter.  In this case it is a pointer to a REVERBTAP structure
 
	[RETURN_VALUE]
	A pointer to the new modified buffer.  

	[REMARKS]
	All the <<2 stuff is to convert samples to bytes, as all offsets and lengths are based 
	on samples not bytes.  For mmx the output size is 4 bytes per sample.

	[SEE_ALSO]
	LowPassCallback
]
*/
void * F_CALLBACKAPI PreverbCallback(void *originalbuffer, void *newbuffer, int length, int param)
{
	int		mixertype = FSOUND_GetMixer();
	REVERBTAP	*tap = (REVERBTAP *)param;

	// must be 16bit stereo integer buffer.. sorry blendmode (32bit) and fpu (32bit float) dont support this.
	if (mixertype == FSOUND_MIXER_BLENDMODE || 
		mixertype == FSOUND_MIXER_QUALITY_FPU)
    {
		return newbuffer;
    }


	// preverbbuff is a ringbuffer.  If the length makes the copy wrap, then split the copy 
	// into end part, and start part
	if (tap->historyoffset + length > tap->historylen)
	{
		int taillen = tap->historylen - tap->historyoffset;
		int startlen = length - taillen;		// whatever is left

		// get a clean version of the preverb buffer (should be an unscaled history of the mixbuffer)
		memcpy(tap->workarea, tap->historybuff + (tap->historyoffset << 2), (tap->historylen - tap->historyoffset)<<2);
		memcpy(tap->workarea + ((tap->historylen - tap->historyoffset) << 2), tap->historybuff, (length - (tap->historylen - tap->historyoffset)) << 2);

		// now copy input into preverb/history buffer 
		memcpy(tap->historybuff + (tap->historyoffset << 2), newbuffer, (tap->historylen - tap->historyoffset)<<2);
		memcpy(tap->historybuff, (signed char *)newbuffer + ((tap->historylen - tap->historyoffset) << 2), (length - (tap->historylen - tap->historyoffset)) << 2);

	}
	// no wrapping preverb buffer, just write dest
	else
	{
		// get a clean version of the preverb buffer (should be an unscaled history of the mixbuffer)
		memcpy(tap->workarea, tap->historybuff + (tap->historyoffset << 2), length << 2);

		// now copy input into preverb/history buffer 
		memcpy(tap->historybuff + (tap->historyoffset << 2), newbuffer, length << 2);
	}

	// now mix a scaled input into this 
	FSOUND_DSP_MixBuffers(tap->workarea, newbuffer, length, outputfreq, tap->volume, tap->pan, FSOUND_STEREO | FSOUND_16BITS);

	tap->historyoffset += length;
	if (tap->historyoffset >= tap->historylen) 
    {
		tap->historyoffset -= tap->historylen;
    }

	// preverb history has been mixed into new buffer, so return it.
	return tap->workarea;
}


void Reverb_Init()
{
	// ====================================================================================================================
	// PREVERB SETUP
	// ====================================================================================================================
	{
		int delay[PREVERB_NUMTAPS]	= { 57, 97, 163 };		// // prime numbers! dont go lower than 20!  it will be smaller than the DSP bufferlen!! (a check is done below for this)
		int volume[PREVERB_NUMTAPS] = { 128, 78, 46 };
		int pan[PREVERB_NUMTAPS]	= { 128-24, 128+24, 128 };
		int count;

		for (count=0; count< PREVERB_NUMTAPS; count++)
		{
			PreverbTap[count].delayms		= delay[count];	
			PreverbTap[count].volume		= volume[count];
			PreverbTap[count].pan			= pan[count];
			PreverbTap[count].historyoffset = 0;
			PreverbTap[count].historylen	= (PreverbTap[count].delayms * outputfreq / 1000);
			
			if (PreverbTap[count].historylen < FSOUND_DSP_GetBufferLength())
            {
				PreverbTap[count].historylen = FSOUND_DSP_GetBufferLength();	// just in case our calc is not the same.
            }

			PreverbTap[count].historybuff	= calloc(PreverbTap[count].historylen + 2048, 4);	// * 4 is for 16bit stereo (mmx only)
			PreverbTap[count].workarea		= calloc(FSOUND_DSP_GetBufferLength(), 4); 
			PreverbTap[count].Unit			= FSOUND_DSP_Create(&PreverbCallback, FSOUND_DSP_DEFAULTPRIORITY_USER+count,	(int)&PreverbTap[count]);
		}
	}

	// ====================================================================================================================
	// REVERB SETUP
	// ====================================================================================================================
	{
		// something to fiddle with..		
		int delay[REVERB_NUMTAPS]	= { 131, 149, 173, 211, 281, 401, 457};	// prime numbers!
		int volume[REVERB_NUMTAPS]	= { 120, 100,  95,  90,  80,  60,  50};
		int pan[REVERB_NUMTAPS]		= { 100, 128, 128, 152, 128, 100, 152};
		int count;

		for (count=0; count< REVERB_NUMTAPS; count++)
		{
			ReverbTap[count].delayms		= delay[count];	
			ReverbTap[count].volume			= volume[count];
			ReverbTap[count].pan			= pan[count];
			ReverbTap[count].historyoffset	= 0;
			ReverbTap[count].historylen		= (ReverbTap[count].delayms * outputfreq / 1000);
			if (ReverbTap[count].historylen < FSOUND_DSP_GetBufferLength())
            {
				ReverbTap[count].historylen = FSOUND_DSP_GetBufferLength();	// just in case our calc is not the same.
            }

			ReverbTap[count].historybuff	= calloc(ReverbTap[count].historylen, 4);	// * 4 is for 16bit stereo (mmx only)
			ReverbTap[count].workarea		= NULL;
			ReverbTap[count].Unit			= FSOUND_DSP_Create(&ReverbCallback, FSOUND_DSP_DEFAULTPRIORITY_USER+20+(count*2),	(int)&ReverbTap[count]);
		}
	}
}


void Reverb_Close()
{
    int count;

	for (count=0; count<PREVERB_NUMTAPS; count++)
	{
		if (PreverbTap[count].Unit)
        {
			FSOUND_DSP_Free(PreverbTap[count].Unit);
        }
		PreverbTap[count].Unit = NULL;

		if (PreverbTap[count].historybuff)
        {
			free(PreverbTap[count].historybuff);		
        }
		PreverbTap[count].historybuff = NULL;

		if (PreverbTap[count].workarea)
        {
			free(PreverbTap[count].workarea);
        }
		PreverbTap[count].workarea = NULL;
	}

	for (count=0; count<REVERB_NUMTAPS; count++)
	{
		if (ReverbTap[count].Unit)
        {
			FSOUND_DSP_Free(ReverbTap[count].Unit);	
        }
		ReverbTap[count].Unit = NULL;

		if (ReverbTap[count].historybuff)
        {
			free(ReverbTap[count].historybuff);
        }
		ReverbTap[count].historybuff = NULL;

		if (ReverbTap[count].workarea)
        {
			free(ReverbTap[count].workarea);
        }
		ReverbTap[count].workarea = NULL;
	}

}