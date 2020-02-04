#pragma once
#include <stdio.h>

class noncopyable
{

protected:
    noncopyable()  = default;
    ~noncopyable() = default;
    void test_func() {
        printf("noncopyable  test\n");
    }
public:
    noncopyable(const noncopyable&) = delete ;
    void operator=(const noncopyable&) = delete ;
    noncopyable(noncopyable&&) = delete;

};

