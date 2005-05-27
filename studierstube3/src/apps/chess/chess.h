
#ifdef WIN32
#ifdef CHESS_EXPORTS
#define CHESS_API __declspec(dllexport)
#else
#define CHESS_API __declspec(dllimport)
#endif
#else
#define CHESS_API
#endif

