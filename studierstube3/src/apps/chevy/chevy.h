
#ifdef WIN32
#ifdef CHEVY_EXPORTS
#define CHEVY_API __declspec(dllexport)
#else
#define CHEVY_API __declspec(dllimport)
#endif
#else
#define CHEVY_API
#endif
