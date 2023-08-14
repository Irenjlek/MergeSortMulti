#pragma once
#include "OptimizedThreadPool.h"

class RequestHandler
{
public:
    RequestHandler();
    ~RequestHandler();
    // отправка запроса на выполнение
    void pushRequest(FuncType f, int* array, long left, long right);
private:
    // пул потоков
    OptimizedThreadPool m_tpool;
};

