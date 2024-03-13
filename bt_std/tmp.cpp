
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>
#include <chrono>

using std::chrono::seconds;
using std::this_thread::sleep_for;

namespace {

static const std::vector<int>& vec = {1, 2, 3};

void good1() {
  std::cout << "Accessing good1: " << vec.at(0) << std::endl;
}

void bad2() {
  size_t i = 10;
  vec.at(i);  // This will throw std::out_of_range
}

void good3() {
  std::cout << "Accessing good3: " << vec.at(1) << std::endl;
}

void f6() {
  good1();
  bad2();  // This call will throw std::out_of_range
  good3();
}

void f5() {
  f6();
}

void f4_slotCall() noexcept {
  try {
    f5();
  } catch (const std::exception& e) {
    std::cerr << "Consume std::exception raised by slot invocation: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Consume unknown exception raised by slot invocation: " << std::endl;
  }
}

void f3_notificationCallOne() noexcept {
  f4_slotCall();
}

void f2_engine_impl_run() {
  f3_notificationCallOne();
}

void f1_custom_thread_extras() {
  try {
    f2_engine_impl_run();
  } catch (const std::exception& e) {
    std::cerr << "Consume std::exception raised in custom thread: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Consume unknown exception raised in custom thread: " << std::endl;
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  std::thread worker(f1_custom_thread_extras);
  sleep_for(seconds{10});
  worker.join();  // Join on the worker thread
  return 0;
}
