#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
using namespace std;

//int sum = 0;
atomic<int> sum = 0;
/*
    int eax = sum;
    eax += 1;
    sum = eax;
*/

void Add() {
    for (int i = 0; i < 1000'000; ++i)
        sum.fetch_add(1);
}

void Sub() {
    for (int i = 0; i < 1000'000; ++i)
        sum.fetch_add(-1);
}

int main()
{

    thread t1(Add);
    thread t2(Sub);

    t1.join();
    t2.join();
    
    cout << sum << "\n";
    
    return 0;
}
