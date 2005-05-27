
#ifdef WIN32
#ifdef WORMS_EXPORTS
#define WORMS_API __declspec(dllexport)
#else
#define WORMS_API __declspec(dllimport)
#endif
#else
#define WORMS_API
#endif
