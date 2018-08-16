#include "action/addtodo.h"
#include "action/setvisibilityfilter.h"
#include "action/toggletodo.h"
#include "redux.h"
#include "state.h"

#include <iostream>

auto drawVisibilityFilter() {
  return redux::View{ [](state::VisibilityFilter) {
    std::cout << "VisibilityFilter\n";
  } };
}

auto drawTodos() {
  return redux::View{ [](state::TodoApp) { std::cout << "Todos\n"; } };
}

int main() {
  auto const visibilityFilter = redux::Reducer<action::SetVisibilityFilter>{};
  auto const todos   = redux::Reducer<action::AddTodo, action::ToggleTodo>{};
  auto const reducer = redux::CombinedReducer{ todos, visibilityFilter };

  // auto const views = redux::Views{ drawVisibilityFilter(), drawTodos() };
  auto const view = combineViews(drawVisibilityFilter(), drawTodos());

  auto store = redux::Store{ state::TodoApp{}, reducer, view };

  // std::cout << "before\n";

  store.dispatch(action::addTodo("Learn about actions"));
  store.dispatch(action::addTodo("Learn about reducers"));
  store.dispatch(action::addTodo("Learn about store"));
  store.dispatch(action::toggleTodo(0));
  store.dispatch(action::toggleTodo(1));

  store.dispatch(action::setVisibilityFilter(state::VisibilityFilter::SHOW_COMPLETED));

  // std::cout << "after\n";

  // auto event = char{};

  // while (std::cin >> event)
  //  dispatch (store, event);
}
