#include <queue>
#include <future>
#include <condition_variable>
#include <vector>

using namespace std;

// ������� ����������� ��� ���������� ����
typedef function<void()> task_type;
// ��� ��������� �� �������, ������� �������� �������� ��� ������� �����
typedef void (*FuncType) (int* array, long left, long right);
// ��� �������
class ThreadPool {
public:
    // ������
    void start();
    // ���������
    void stop();
    // ������� �����
    void push_task(FuncType f, int* array, long left, long right);
    // ������� ����� ��� ������
    void threadFunc();
private:
    // ������
    vector<thread> m_threads;
    // ��������� ������������� �������
    mutex m_locker;
    // ������� �����
    queue<task_type> m_task_queue;
    // ��� ������������� ������ �������
    condition_variable m_event_holder;
    // ���� ��� ��������� ������ �������
    volatile bool m_work;
};

