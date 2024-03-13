#include <row/bt_qt2_core/core.hpp>
#include <QThread>
#include <chrono>

using std::chrono::seconds;
using std::this_thread::sleep_for;

namespace {

void invokeSlot() {
  row::bt_qt2_core::f();
}

void asyncSignalEvent() noexcept {
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
