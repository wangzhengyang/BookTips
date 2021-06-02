#ifndef ALLOC_H
#define ALLOC_h

#include <stddef.h>
#include <stdlib.h>

//第一级配置器
template <int i>
class first_malloc_template
{
public:
    static void *allocate(size_t n)
    {
        void *p = malloc(n);
        if (p == 0)
        {
            p = alloc_malloc(n);
        }
        return p;
    }

    static void deallocate(void *p)
    {
        free(p);
    }

    static void *reallocate(void *p, size_t n)
    {
        void *p = realloc(p, n);
        if (p == 0)
        {
            p = alloc_realloc(p, n);
        }
        return p;
    }

    static void (*set_malloc_hander(void (*func)()))()
    {
        void (*p)() = alloc_handler;
        alloc_handler = func;
        return p;
    }

private:
    static void *alloc_malloc(size_t n);
    static void *alloc_realloc(void *p, size_t n);
    static void (*alloc_handler)();
};

template <int i>
void (*first_malloc_template<i>::alloc_handler)() = 0;

template <int i>
void *first_malloc_template<i>::alloc_malloc(size_t n)
{
    void (*handler)() = 0;
    void *p = 0;
    for (;;)
    {
        handler = alloc_handler;
        if (handler != 0)
        {
            handler();
        }
        p = malloc(n);
        if (p)
        {
            return p;
        }
    }
}

template <int i>
void *first_malloc_template<i>::alloc_realloc(void *p, size_t n)
{
    void (*handler)() = 0;
    void *p = 0;
    for (;;)
    {
        handler = alloc_handler;
        if (handler)
        {
            handler();
        }
        p = realloc(p, n);
        if (p)
        {
            return p;
        }
    }
}

typedef first_malloc_template<0> first_malloc_alloc;

//第二级配置器

enum
{
    __ALIGIN = 8
};
enum
{
    __MAX_BYTES = 128
};
enum
{
    __FREELISTS = __MAX_BYTES / __ALIGIN
}; // 128/8 = 16

template <int i>
class second_malloc_template
{
public:
    static void *allocate(size_t n);
    static void deallocate(void *p, size_t n);
    static void *reallocate(void *p, size_t old_n, size_t new_n);

private:
    static size_t freelists_index(size_t n)
    {
        return ((n + __ALIGIN - 1) / __ALIGIN - 1);
    }
    static size_t round_up(size_t n)
    {
        return ((n + __ALIGIN - 1) & ~(__ALIGIN - 1));
    }
    static void *refill(size_t n);
    static char *chunk_alloc(size_t n, int &nobjs);

private:
    union obj
    {
        union obj *next;
        char data[1];
    };

    static obj *volatile freelists[__FREELISTS];
    static char *start_free;
    static char *end_free;
    static size_t heap_size;
};

template <int i>
char *second_malloc_template<i>::start_free = 0;

template <int i>
char *second_malloc_template<i>::end_free = 0;

template <int i>
size_t second_malloc_template<i>::heap_size = 0;

template <int i>
second_malloc_template<i>::obj *volatile second_malloc_template<i>::freelists[__FREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//内存池代码
template <int i>
char *second_malloc_template<i>::chunk_alloc(size_t n, int &nobjs)
{
    char *result = 0;
    size_t need_bytes = n * nobjs;
    size_t left_bytes = start_free - end_free;
    if (left_bytes >= need_bytes)
    {
        result = start_free;
        start_free += need_bytes;
        return result;
    }
    else if (left_bytes >= n)
    {
        nobjs = left_bytes / n;
        result = start_free;
        start_free += n * nobjs;
        return result;
    }
    else
    {
        size_t need_to_get = 2 * need_bytes + round_up(heap_size >> 4);

        if (left_bytes > 0)
        {
            obj *volatile *list = freelists + freelists_index(left_bytes);
            ((obj *)start_free)->next = *list;
            (*list)->next = (obj *)start_free;
        }

        start_free = (char *)malloc(need_to_get);
        if (0 == start_free)
        {
            int j = 0;
            obj *volatile *list;
            obj *p;
            for (i = n; i < __MAX_BYTES; i += __ALIGIN)
            {
                list = freelists + freelists_index(i);
                p = *list;
                if (p != 0)
                {
                    *list = p->next;
                    start_free = (char *)p;
                    end_free = start_free + i;
                    return (chunk_alloc(n, nobjs));
                }
            }
            end_free = 0;
            start_free = (char *)first_malloc_alloc::allocate(need_to_get);
        }

        heap_size += need_to_get;
        end_free = start_free + need_to_get;
        return (chunk_alloc(n, nobjs));
    }
}

template <int i>
void *second_malloc_template<i>::refill(size_t n)
{
    int nobjs = 20;
    char *chunk = chunk_alloc(n, nobjs);
    obj *volatile *list;

    if (nobjs == 1)
    {
        return (chunk);
    }

    list = freelists + freelists_index(n);

    obj *result;
    obj *current_obj;
    obj *next_obj;

    result = (obj *)chunk;

    *list = next_obj = (obj *)(chunk + n);
    for (int i = 0;; i++)
    {
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + n);
        if (nobjs - 1 == i)
        {
            current_obj->next = 0;
            break;
        }
        else
        {
            current_obj->next = next_obj;
        }
    }
    return (result);
}

template <int i>
void *second_malloc_template<i>::allocate(size_t n)
{
    obj *volatile *free_list;
    obj *result;

    if (n > (size_t)__MAX_BYTES)
    {
        return (first_malloc_alloc::allocate(n));
    }

    free_list = freelists + freelists_index(n);
    result = *free_list;
    if (result == 0)
    {
        void *r = refill(round_up(n));
        return r;
    }
    *free_list = (*free_list)->next;
    return result;
}

template <int i>
void second_malloc_template<i>::deallocate(void *p, size_t n)
{
    obj *volatile *free_list;

    if (n > (size_t)__MAX_BYTES)
    {
        first_malloc_alloc::deallocate(p);
        return;
    }

    free_list = freelists + freelists_index(n);
    obj *q = (obj *)p;
    q->next = *free_list;
    return;
}

template <int i>
void *second_malloc_template<i>::reallocate(void *p, size_t old_size, size_t new_size)
{
    if (old_size == new_size)
    {
        return p;
    }

    deallocate(p, old_size);
    return allocate(new_size);
}

typedef second_malloc_template<0> alloc;

template <class T, class Alloc>
class simple_alloc{
public:
    static T* allocate(size_t n){return (T*)Alloc::allocate(n * sizeof(T));}
    static T* allocate() {return (T*)Alloc::allocate(sizeof(T));}
    static void deallocate(T* p, size_t n) {return Alloc::deallocate(p, n * sizeof(T));}
    static void deallocate(T* p) {return Alloc::deallocate(p, sizeof(T));}
};

#endif