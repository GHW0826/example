#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>
using namespace std;

// volatile : 컴파일러 최적화 하지말라.
class SpinLock
{
public:
    void lock() {
        // cas (compare nad swap)
        /*
            bool expected;
            bool desired;
            if (locked_ == expected) {
                expected = locked_;
                locked_ = desired;
                return true;
            }
            else {
                expected = locked_;
                return false;
            }
        */
        bool expected = false;
        bool desired = true;
        while (locked_.compare_exchange_strong(expected, desired) == false) {
            expected = false;
            
            // this_thread::sleep_for(chrono::microseconds(1000));
            // this_thread::sleep_for(100ms);
            // CPU 할당 취소
            // this_thread::yield();
        }

        locked_.store(true);
    }

    void unlock() {
        locked_.store(false);
    }
private:
    atomic<bool> locked_ = false;
};

int sum = 0;
mutex m;
SpinLock spinLock;

void Add() {
    for (int i = 0; i < 1'000'000; ++i) {
        lock_guard<SpinLock> guard(spinLock);
        ++sum;
    }
}

void Sub() {
    for (int i = 0; i < 1'000'000; ++i) {
        lock_guard<SpinLock> guard(spinLock);
        --sum;
    }
}
int main()
{
    thread t1(Add);
    thread t2(Sub);

    t1.join();
    t2.join();

    return 0;
}
