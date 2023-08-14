#pragma once
#include "OptimizedThreadPool.h"

class RequestHandler
{
public:
    RequestHandler();
    ~RequestHandler();
    // �������� ������� �� ����������
    void pushRequest(FuncType f, int* array, long left, long right);
private:
    // ��� �������
    OptimizedThreadPool m_tpool;
};

