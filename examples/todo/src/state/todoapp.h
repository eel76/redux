#pragma once
#include "state/todos.h"
#include "state/visibilityfilter.h"

namespace state {
  struct TodoApp
  {
    Todos            todos;
    VisibilityFilter visibilityFilter;
  };
}
