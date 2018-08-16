#pragma once
#include <string>
#include <vector>

namespace state {
  struct Todo
  {
    std::string text;
    bool        completed;
  };

  using Todos = std::vector<Todo>;

  enum class VisibilityFilter { SHOW_ALL, SHOW_COMPLETED, SHOW_ACTIVE };

  struct TodoApp
  {
    Todos            todos;
    VisibilityFilter visibilityFilter;
  };
}
