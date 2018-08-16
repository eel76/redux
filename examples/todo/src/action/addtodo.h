#pragma once
#include "state.h"

namespace action {
  struct AddTodo
  {
    std::string text;

    state::Todos operator()(state::Todos todos) const {
      todos.push_back(state::Todo{ text, false });
      return todos;
    }
  };

  auto addTodo(std::string text) {
    return AddTodo{ text };
  }
}
