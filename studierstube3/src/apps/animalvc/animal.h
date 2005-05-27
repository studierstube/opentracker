
#ifdef WIN32
#ifdef ANIMAL_EXPORTS
#define ANIMAL_API __declspec(dllexport)
#else
#define ANIMAL_API __declspec(dllimport)
#endif
#else
#define ANIMAL_API
#endif
