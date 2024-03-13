
#include <iostream>
#include <stdexcept>
#include <thread>
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
  good();
  bad();
}

void invokeSlot() {
  // catching exception obscures origin throw location
  try {
    f();
  } catch (const std::exception& e) {
    std::cerr << "Consume std::exception raised by slot invocation: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Consume unknown exception raised by slot invocation: " << std::endl;
  }
}

void asyncSignalEvent() {
  invokeSlot();
}

}  // namespace

int main(int argc, char* argv[]) {
  std::thread worker(asyncSignalEvent);
  sleep_for(seconds{10});
  worker.join();
  return 0;
}
