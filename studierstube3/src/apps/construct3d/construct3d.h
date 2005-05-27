#ifdef WIN32
#ifdef CONSTRUCT3D_EXPORTS
#define CONSTRUCT3D_API __declspec(dllexport)
#else
#define CONSTRUCT3D_API __declspec(dllimport)
#endif
#else
#define CONSTRUCT3D_API
#endif

