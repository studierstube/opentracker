
#ifdef WIN32
#ifdef SPRAY_EXPORTS
#define SPRAY_API __declspec(dllexport)
#else
#define SPRAY_API __declspec(dllimport)
#endif
#else
#define SPRAY_API
#endif
