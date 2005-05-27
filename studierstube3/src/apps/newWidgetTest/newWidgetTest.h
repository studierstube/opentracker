
#ifdef WIN32
#ifdef NEWWIDGETTEST_EXPORTS
#define NEWWIDGETTEST_API __declspec(dllexport)
#else
#define NEWWIDGETTEST_API __declspec(dllimport)
#endif
#else
#define NEWWIDGETTEST_API
#endif
