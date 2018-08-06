#pragma once
#include "state/todos.h"

namespace action {
  struct ToggleTodo
  {
    int index;

    state::Todos operator()(state::Todos todos) const {
      todos[index].completed = !todos[index].completed;
      return todos;
    }
  };

  auto toggleTodo(int index) {
    return ToggleTodo{ index };
  }
}
