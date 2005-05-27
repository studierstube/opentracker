
#ifdef WIN32
#ifdef MD2_ART_EXPORTS

#define MD2_ART_API __declspec(dllexport)
#else
#define MD2_ART_API __declspec(dllimport)

#endif	// MD2ATART_EXPORTS

#else

#define MD2_ART_API

#endif	// WIN32
