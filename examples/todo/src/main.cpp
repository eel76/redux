#include "action.h"
#include "redux.h"
#include "state.h"

#include <iostream>
#include <map>

auto drawVisibilityFilter() {
  return [](state::VisibilityFilter visibilityFilter) {
    auto filters = std::map<state::VisibilityFilter, std::string>{
      { { state::VisibilityFilter::SHOW_ALL, "show all" },
        { state::VisibilityFilter::SHOW_COMPLETED, "show completed" },
        { state::VisibilityFilter::SHOW_ACTIVE, "show active" } }
    };

    std::cout << "Filter:\n  " << filters[visibilityFilter] << "\n";
  };
}

auto drawTodos() {
  return [](state::AppState appState) {
    std::cout << "Todos:\n";

    auto const isVisible =
    std::map<state::VisibilityFilter, std::function<bool(state::Todo const&)>>{
      { state::VisibilityFilter::SHOW_ALL, [](auto&&) { return true; } },
      { state::VisibilityFilter::SHOW_COMPLETED,
        [](auto&& todo) { return todo.completed; } },
      { state::VisibilityFilter::SHOW_ACTIVE,
        [](auto&& todo) { return !todo.completed; } }
    }[appState.visibilityFilter];

    for (auto&& todo : appState.todos)
      if (isVisible(todo))
        std::cout << "  " << todo.text << ": "
                  << (todo.completed ? "completed" : "active") << "\n";
  };
}

int main() {
  auto const visibilityFilter = redux::Reducer<action::SetVisibilityFilter>{};
  auto const todos   = redux::Reducer<action::AddTodo, action::ToggleTodo>{};
  auto const reducer = redux::CombinedReducer{ todos, visibilityFilter };
  auto const view    = redux::combinedView(drawVisibilityFilter(), drawTodos());

  auto store = redux::Store{ state::AppState{}, reducer, view };

  store.dispatch(action::addTodo("Learn about actions"));
  store.dispatch(action::addTodo("Learn about reducers"));
  store.dispatch(action::addTodo("Learn about store"));
  store.dispatch(action::toggleTodo(0));
  store.dispatch(action::toggleTodo(1));

  store.dispatch(action::setVisibilityFilter(state::VisibilityFilter::SHOW_COMPLETED));

  store.dispatch(action::removeCompleted());

  // auto event = char{};

  // while (std::cin >> event)
  //  dispatch (store, event);
}
