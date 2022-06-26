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

__declspec(thread) int value;
thread_local int LthreadId = 0;

void ThreadMain(int threadId) {
    LthreadId = threadId;
    value = threadId * 100;

    while (true) {
        cout << "id: " << LthreadId << "\n";
        cout << "value: " << value << "\n\n";
        this_thread::sleep_for(1s);
    }
}

int main()
{
    vector<thread> threads;
    for (int i = 0; i < 10; ++i)
        threads.emplace_back(thread(ThreadMain, i));

    for (int i = 0; i < 10; ++i) {
        if (threads[i].joinable())
            threads[i].join();
    }

    return 0;
}
