/**
 * @defgroup media Multimedia extensions
 * @ingroup extensions
 *
 *
 * @author Flo Ledermann ledermann@ims.tuwien.ac.at
 */


#ifdef WIN32
    #ifdef FMODSOUND_EXPORTS
        #define FMODSOUND_API __declspec(dllexport)
        #define FMODSOUND_EXTERN
    #else
        #define FMODSOUND_API __declspec(dllimport)
        #define FMODSOUND_EXTERN extern
    #endif
#else
    #define FMODSOUND_API
#endif
