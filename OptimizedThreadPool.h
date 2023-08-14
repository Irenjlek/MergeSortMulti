#include <queue>
#include <future>
#include <condition_variable>
#include <vector>

using namespace std;

// ������� ����������� ��� ���������� ����
typedef function<void()> task_type;
// ��� ��������� �� �������, ������� �������� �������� ��� ������� �����
typedef void (*FuncType) (int* array, long left, long right);

template<class T>
class BlockedQueue {
public:
    void push(T& item) {
        lock_guard<mutex> l(m_locker);
        // ������� ���������������� push
        m_task_queue.push(item);
        // ������ ����������, ����� �����, ���������
        // pop ��������� � ������ ������� �� �������
        m_notifier.notify_one();
    }
    // ����������� ����� ��������� �������� �� �������
    void pop(T& item) {
        unique_lock<mutex> l(m_locker);
        if (m_task_queue.empty())
            // ����, ���� ������� push
            m_notifier.wait(l, [this] {return !m_task_queue.empty(); });
        item = m_task_queue.front();
        m_task_queue.pop();
    }
    // ������������� ����� ��������� �������� �� �������
    // ���������� false, ���� ������� �����
    bool fast_pop(T& item) {
        lock_guard<mutex> l(m_locker);
        if (m_task_queue.empty())
            // ������ �������
            return false;
        // �������� �������
        item = m_task_queue.front();
        m_task_queue.pop();
        return true;
    }
private:
    mutex m_locker;
    // ������� �����
    queue<T> m_task_queue;
    // �����������
    condition_variable m_notifier;
};

// ��� �������
class OptimizedThreadPool {
public:
    OptimizedThreadPool();
    // ������
    void start();
    // ���������
    void stop();
    // ������� �����
    void push_task(FuncType f, int* array, long left, long right);
    // ������� ����� ��� ������
    void threadFunc(int index);
private:
    int m_thread_count;
    // ������
    vector<thread> m_threads;
    // ��������� ������������� �������
    mutex m_locker;
    // ������� �����
    vector<BlockedQueue<task_type>> m_thread_queues;
    int m_index;
};

