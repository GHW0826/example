#include <iostream>
#include <thread>
#include <vector>
using namespace std;

void print() {
    cout << "print" << endl;
}

void print2(int num) {
    cout << num << endl;
}

int main()
{
    thread t1(print);
    t1.join();                              // thread가 끝날때 까지 대기

    int cnt = t1.hardware_concurrency();    // CPU 코어 갯수 확인
    cout << cnt << endl;

    auto id = t1.get_id();                  // thread 고유 ID
    cout << id << endl;

    if (t1.joinable())                     // 실제로 연동된 thread가 없는지 확인.
        t1.detach();                       // std::thread 객체에서 실제 스레드를 분리 (분리시 스레드 상태, 정보 추출 불가능)

    thread t2;
    t2 = thread(print2, 20);               // 나중에 연동 가능.
    t2.join();


    vector<thread> threads;
    for (int i = 0; i < 10; ++i)
        threads.emplace_back(thread(print2, i));

    for (int i = 0; i < 10; ++i)
        if (threads[i].joinable())
            threads[i].join();

    return 0;
}
