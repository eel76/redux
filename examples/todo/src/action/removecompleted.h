#pragma once
#include "state.h"
#include <algorithm>

namespace action {
  struct RemoveCompleted
  {
    state::Todos operator()(state::Todos todos) const {
      todos.erase(std::remove_if(todos.begin(), todos.end(),
                                 [](auto&& todo) { return todo.completed; }),
                  todos.end());
      return todos;
    }
  };

  auto removeCompleted() {
    return RemoveCompleted{};
  }
}
