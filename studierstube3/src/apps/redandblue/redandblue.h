
#ifdef WIN32
#ifdef REDANDBLUE_EXPORTS
#define REDANDBLUE_API __declspec(dllexport)
#else
#define REDANDBLUE_API __declspec(dllimport)
#endif
#else
#define REDANDBLUE_API
#endif
