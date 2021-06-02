#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>
#include <iterator.h>
#include <type_traits.h>

template <class T1, class T2>
inline void construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T>
inline void destroy(T* pointer)
{
    pointer->~T();
}

template <class ForwarIterator>
inline void __destroy_aux(ForwarIterator first, ForwarIterator last, false_type)
{
    for(; first < last; ++first)
    {
        destroy(&*first);
    }
}

template <class ForwarIterator>
inline void __destroy_aux(ForwarIterator first, ForwarIterator last, true_type)
{

}

template <class ForwarIterator, class T>
inline void __destroy(ForwarIterator first, ForwarIterator last, T*)
{
    typedef typename type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, value_type(first));
}

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*){}

#endif