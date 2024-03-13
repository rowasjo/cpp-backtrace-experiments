#include <QThread>
#include <iostream>
#include <optional>
#include <chrono>

using std::chrono::seconds;
using std::this_thread::sleep_for;

namespace {
static std::optional<std::string_view> state = std::nullopt;

void good() {
  if (state) {
    state.value();
  }
}

void bad() {
  state.value();  // throws std::bad_optional_access
}

void f() {
  //  std::string s = "f";
  good();
  bad();
}

void invokeSlot() {
  f();
}

void asyncSignalEvent() /*noexcept*/ {
  invokeSlot();
}

class WorkerThread : public QThread {
 protected:
  void run() override { asyncSignalEvent(); }
};

}  // namespace

int main(int argc, char* argv[]) {
  WorkerThread worker;
  worker.start();
  sleep_for(seconds{10});
  worker.wait();
  return 0;
}
