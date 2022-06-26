#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>

#include <condition_variable>
#include <queue>
#include <stack>
#include <windows.h>

using namespace std;



template<typename T>
class LockQueue
{
public:
    LockQueue() {}
    LockQueue(const LockQueue&) = delete;
    LockQueue& operator=(const LockQueue&) = delete;

    void Push(T value) {
        lock_guard<mutex> lock(mutex_);
        q_.push(std::move(value));
        cv_.notify_one();
    }

    void Empty() {
        lock_guard<mutex> lock(mutex_);
        return q_.empty();
    }

    bool TryPop(T& value) {
        lock_guard<mutex> lock(mutex_);
        if (q_.empty() == true)
            return false;

        value = std::move(q_.front());
        q_.pop();
        return true;

    }

    void WaitPop(T& value) {
        unique_lock<mutex> lock(mutex_);
        cv_.wait(lock, [this] { return q_.empty() == false; });
        value = std::move(q_.front());
        q_.pop();
    }
private:
    queue<T> q_;
    mutex mutex_;
    condition_variable cv_;
};


template<typename T>
class LockStack
{
public:
    LockStack() {}
    LockStack(const LockStack&) = delete;
    LockStack& operator=(const LockStack&) = delete;

    void Push(T value) {
        lock_guard<mutex> lock(mutex_);
        s_.push(std::move(value));
        cv_.notify_one();
    }

    void Empty() {
        lock_guard<mutex> lock(mutex_);
        return s_.empty();
    }

    bool TryPop(T& value) {
        lock_guard<mutex> lock(mutex_);
        if (s_.empty() == true)
            return false;

        value = std::move(s_.top());
        s_.pop();
        return true;

    }

    void WaitPop(T& value) {
        unique_lock<mutex> lock(mutex_);
        cv_.wait(lock, [this] { return s_.empty() == false; });
        value = std::move(s_.top());
        s_.pop();
    }
private:
    stack<T> s_;
    mutex mutex_;
    condition_variable cv_;
};

LockQueue<int> q;
LockStack<int> s;

void Push() {
    while (true) {
        int val = rand() % 100;
        q.Push(val);

        this_thread::sleep_for(10ms);
    }
}

void Pop() {
    while (true) {
        int data = 0;
        if (q.TryPop(data) == true)
            cout << data << "\n";
    }
}

int main()
{
    thread t1(Push);
    thread t2(Pop);

    t1.join();
    t2.join();

    return 0;
}
