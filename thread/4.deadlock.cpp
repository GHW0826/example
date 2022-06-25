#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <unordered_map>
using namespace std;

// dead lock
// 1. 락 거는 순서 결정 (래퍼 클래스 만들어서 id 순서로 락거는 순서 구현)
// 트랜잭션 락이랑 비슷/

class Account
{

};

class AccountManger
{
public:
    static AccountManger* instance() {
        static AccountManger instance;
        return &instance;
    }

    Account* GetAccount(int id) {
        lock_guard<mutex> guard(mutex_);

        return nullptr;
    }
    void ProcessLogin() {
        lock_guard<mutex> guard(mutex_);

        User* user = UserManager::instance()->GetUser(100);
    }
private:
    mutex mutex_;
    unordered_map<int, Account*> account_;
};


class User
{

};

class UserManager
{
public:
    static UserManager* instance() {
        static UserManager instance;
        return &instance;
    }

    User* GetUser(int id) {
        lock_guard<mutex> guard(mutex_);

        return nullptr;
    }
    void ProcessSave() {
        // 1. 락 거는 순서 변경
        // lock_guard<mutex> guard(mutex_);

        Account* account = AccountManger::instance()->GetAccount(100);

        lock_guard<mutex> guard(mutex_);
    }
private:
    mutex mutex_;
    unordered_map<int, User*> account_;
};

void func1()
{
    for (int i = 0; i < 1'000; ++i)
        UserManager::instance()->ProcessSave();
}

void func2()
{
    for (int i = 0; i < 1'000; ++i)
        AccountManger::instance()->ProcessLogin();
}

int main()
{
    thread t1(func1);
    thread t2(func2);

    t1.join();
    t2.join();

    // 참고
    mutex m1, m2;
    lock(m1, m2);

    // adopt_lock : 이미 lock된 상태, 나중에 소멸될떄 풀어주기면 하면 된다.
    lock_guard<mutex> g1(m1, std::adopt_lock);
    return 0;
}
