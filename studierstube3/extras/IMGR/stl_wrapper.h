
#ifndef STL_WRAPPER_INCLUDED
#define STL_WRAPPER_INCLUDED

#ifdef _MSC_VER

#include <typeinfo>
#include <list>
#include <set>
#include <stack>
#include <vector>
#include <map>

#ifdef OLD_VERSION // Visual C 4.2:

#undef bool

namespace std {

// TEMPLATE CLASS __list
template<class _TYPE>
class __list : public list<_TYPE, allocator<_TYPE> > 
	{
	public:
		typedef __list<_TYPE> _Myt;
		typedef allocator<_TYPE> _A;
		explicit __list()
			: list<_TYPE, _A>() {}
		explicit __list(size_type _N, const _TYPE& _V = _TYPE())
			: list<_TYPE, _A>(_N, _V) {}
		typedef const_iterator _It;
		__list(_It _F, _It _L)
			: list<_TYPE, _A>(_F, _L) {}
		void swap(_Myt& _X)
			{list<_TYPE, _A>::swap((list<_TYPE, _A>&)_X); }
		friend void swap(_Myt& _X, _Myt& _Y)
			{_X.swap(_Y); }
	};

// TEMPLATE CLASS __set
template<class _K, class _Pr>
class __set : public set<_K, _Pr, allocator<_K> > 
	{
	public:
		typedef __set<_K, _Pr> _Myt;
		typedef allocator<_K> _A;
		explicit __set(const _Pr& _Pred = _Pr())
			: set<_K, _Pr, _A>(_Pred) {}
		typedef const_iterator _It;
		__set(_It _F, _It _L, const _Pr& _Pred = _Pr())
			: set<_K, _Pr, _A>(_F, _L, _Pred) {}
		void swap(_Myt& _X)
			{set<_K, _Pr, _A>::swap((set<_K, _Pr, _A>&)_X); }
		friend void swap(_Myt& _X, _Myt& _Y)
			{_X.swap(_Y); }
	};

// TEMPLATE CLASS Vector
template<class _TYPE>
class __vector : public vector<_TYPE, allocator<_TYPE> > 
	{
	public:
		typedef __vector<_TYPE> _Myt;
		typedef allocator<_TYPE> _A;
		explicit __vector()
			: vector<_TYPE, _A>() {}
		explicit __vector(size_type _N, const _TYPE& _V = _TYPE())
			: vector<_TYPE, _A>(_N, _V) {}
		typedef const_iterator _It;
		__vector(_It _F, _It _L)
			: vector<_TYPE, _A>(_F, _L) {}
		void swap(_Myt& _X)
			{vector<_TYPE, _A>::swap((vector<_TYPE, _A>&)_X); }
		friend void swap(_Myt& _X, _Myt& _Y)
			{_X.swap(_Y); }
	};

// TEMPLATE CLASS Map
template<class _K, class _TYPE, class _Pr>
class __map : public map<_K, _TYPE, _Pr, allocator<_TYPE> > 
	{
	public:
		typedef __map<_K, _TYPE, _Pr> _Myt;
		typedef allocator<_TYPE> _A;
		explicit __map(const _Pr& _Pred = _Pr())
			: map<_K, _TYPE, _Pr, _A>(_Pred) {}
		typedef const value_type *_It;
		__map(_It _F, _It _L, const _Pr& _Pred = _Pr())
			: map<_K, _TYPE, _Pr, _A>(_F, _L, _Pred) {}
		void swap(_Myt& _X)
			{map<_K, _TYPE, _Pr, _A>::swap((map<_K, _TYPE, _Pr, _A>&)_X); }
		friend void swap(_Myt& _X, _Myt& _Y)
			{_X.swap(_Y); }
	};

} // end of namespace std

#define STL_LIST              _STD __list
#define STL_SET               _STD __set
#define STL_VECTOR            _STD __vector
#define STL_LESS              _STD   less
#define STL_MAP               _STD __map
#define STL_BINARY_FUNCTION   _STD binary_function

#define bool int 

#endif // OLD_VERSION

#define STL_LIST              _STD list
#define STL_SET               _STD set
#define STL_VECTOR            _STD vector
#define STL_LESS              _STD less
#define STL_MAP               _STD map
#define STL_BINARY_FUNCTION   _STD binary_function



#else   // ifdef _MSC_VER

#ifdef __GNUG__
#define red   __rb_tree_red
#define black __rb_tree_black

#include <typeinfo>
#include <list>
#include <set>
#include <stack>
#include <vector>
#include <map>

#undef red
#undef black
#else

#include <typeinfo>
#include <list.h>
#include <set.h>
#include <vector.h>
#include <deque.h>
#include <stack.h>
#include <map.h>

#endif


#define STL_LIST              std::list
#define STL_SET               std::set
#define STL_VECTOR            std::vector
#define STL_LESS              std::less
#define STL_MAP               std::map
#define STL_BINARY_FUNCTION   std::binary_function

#endif

#endif // STL_WRAPPER_INCLUDED

