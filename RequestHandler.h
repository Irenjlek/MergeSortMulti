#pragma once
#include "ThreadPool.h"

class RequestHandler
{
public:
    RequestHandler();
    ~RequestHandler();
    // �������� ������� �� ����������
    void pushRequest(FuncType f, int* array, long left, long right);
private:
    // ��� �������
    ThreadPool m_tpool;
};

