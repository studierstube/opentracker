
#ifdef WIN32
#ifdef CONTEXTLOADER_EXPORTS
#define CONTEXTLOADER_API __declspec(dllexport)
#else
#define CONTEXTLOADER_API __declspec(dllimport)
#endif
#else
#define CONTEXTLOADER_API
#endif
