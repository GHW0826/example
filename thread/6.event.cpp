#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>

#include <queue>
#include <windows.h>

using namespace std;

mutex m;
queue<int> q;

HANDLE handle;

void create() {
    while (true) {
        {
            unique_lock<mutex> lock(m);
            q.push(100);
        }
        // signal
        ::SetEvent(handle);
        this_thread::sleep_for(100ms);
    }
}

void destroy() {
    while (true) {

        // Wait
        ::WaitForSingleObject(handle, INFINITE);
        // Non-signal
        
        // ::ResetEvent(handle); manual 수동일시 직접 해줘야함.

        unique_lock<mutex> lock(m);
        if (q.empty() == false) {
            int data = q.front();
            q.pop();
            cout << data << "\n";
        }
    }
}

int main()
{
    // 커널 오브젝트
    // Usage count
    // signal (파란불) / non-signal (빨간불 ) << bool
    // auto / manual << bool
    handle = ::CreateEvent(NULL /* 보안 속성 */, FALSE /* 수동, 자동 리셋 */, FALSE /* 초기 상태 */, NULL);

    thread t1(create);
    thread t2(destroy);

    t1.join();
    t2.join();

    ::CloseHandle(handle);

    return 0;
}
