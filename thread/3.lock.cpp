#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
using namespace std;

vector<int> v;

mutex m;
recursive_mutex rm;

// RAII
template<typename T>
class LockGuard
{
public:
    LockGuard(T& m) {
        mutex_ = &m;
        mutex_->lock();
    }
    ~LockGuard() {
        mutex_->unlock();
    }
private:
        T* mutex_;
};

void push() {
    for (int i = 0; i < 10'000; ++i)
        v.push_back(i);
}

void push1() {
    for (int i = 0; i < 10'000; ++i) {
        m.lock();
        v.push_back(i);
        m.unlock();
    }
}

void push2() {
    LockGuard<mutex> lockGuard(m);

    for (int i = 0; i < 10'000; ++i) {
        v.push_back(i);

        if (i == 5'000)
            break;
    }
}

void push3() {
    lock_guard<mutex> lockGuard(m);

    for (int i = 0; i < 10'000; ++i) {
        v.push_back(i);
        if (i == 5'000)
            break;
    }
}

void push3() {

    unique_lock<mutex> uniqueLock(m, defer_lock);

    // 지연 락 가능
    m.lock();
    for (int i = 0; i < 10'000; ++i) {
        v.push_back(i);
        if (i == 5'000)
            break;
    }
}

int main()
{
    thread t1(push1);
    thread t2(push1);

    t1.join();
    t2.join();

    cout << v.size() << endl;

    return 0;
}
