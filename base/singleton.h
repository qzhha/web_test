#pragma once

#include "noncopyable.h"
#include <stdlib.h>
#include <pthread.h>

namespace detail
{
template <typename T>
struct has_no_destroy
{
    template<typename C> static char test(decltype(&C::no_destroy));
    template<typename C> int test(...);

    const static bool value = true;
    //const static bool value = sizeof(test<T>(0)) == 1;
};
}


template<typename T>
class singleton: noncopyable
{
private:
    singleton() {}
    ~singleton(){}
public:

    static T & instance()
    {
        pthread_once(&ponce_,& singleton::init);
        return *value_;
    }


private:
    static void init()
    {
        value_ = new T();
        if(!detail::has_no_destroy<T>::value)
        {
            ::atexit(destroy);
        }       
    }
    static void destroy()
    {
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy; (void) dummy;
        delete value_;
        value_ = nullptr;
    }
private:
    static pthread_once_t ponce_;
    static T*             value_;
};

template<typename T>
pthread_once_t singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* singleton<T>::value_ = nullptr;

