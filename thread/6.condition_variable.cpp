#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>

#include <condition_variable>
#include <queue>
#include <windows.h>

using namespace std;

mutex m;
queue<int> q;

//user-level obj
condition_variable cv;
condition_variable_any cva;

void create() {
    while (true) {

        // 1) lock
        // 2) 공유 변수 값 수정
        // 3) unlock
        // 4) 조건변수 통해 다른 스레드에게 통지
        {
            unique_lock<mutex> lock(m);
            q.push(100);
        }

        // wait중인 스레드 있으면 1개만 깨움
        cv.notify_one();
    }
}

void destroy() {
    while (true) {
        unique_lock<mutex> lock(m);
        // 1) lock
        // 2) 조건 확인 
        // -> 만족시 빠져 나와 이어서 코드 진행
        // -> x만족시 lock을 풀고 대기 상태
        cv.wait(lock, []() {
            return q.empty() == false;
        });

        // notify_one을 했으면 항상 조건식을 만족해 보이지만
        // spurious wakeup (가짜기상)
        // notify_one을 할때 lock을 잡고 있는 것이 아니기 떄문.

        {
            int data = q.front();
            q.pop();
            cout << q.size() << "\n";
        }
    }
}

int main()
{
    thread t1(create);
    thread t2(destroy);

    t1.join();
    t2.join();

    return 0;
}
