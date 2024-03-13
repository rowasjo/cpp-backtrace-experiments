#include <row/bt_qt2_core/core.hpp>
#include <optional>
#include <string_view>

namespace row::bt_qt2_core {

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

}  // namespace

void f() {
  good();
  bad();
}

}  // namespace row::bt_qt2_core
