#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

int global = 10; // 全局变量
std::mutex mtx;

void threadFunc1() {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 每隔1秒执行一次
        std::lock_guard<std::mutex> lock(mtx); // 使用互斥锁保护共享资源
        ++global;
        std::cout << "Thread 1 incremented global to: " << global << std::endl;
    }
}

void threadFunc2(int& localRef) {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 每隔1秒执行一次
        ++localRef;
        std::cout << "Thread 2 incremented local to: " << localRef << std::endl;
    }
}


int main() {
    int local = 20; // 局部变量

    // 创建线程1和线程2
    std::thread tid1(threadFunc1);
    std::thread tid2(threadFunc2, std::ref(local)); 

    // 主线程休眠15秒
    std::this_thread::sleep_for(std::chrono::milliseconds(15000));

    // 输出主线程中的local和global值
    std::cout << "Main thread local = " << local << "  global = " << global << std::endl;

    // 等待两个线程结束
    tid1.join();
    tid2.join();

    return 0;
}