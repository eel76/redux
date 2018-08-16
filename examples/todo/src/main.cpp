#include "action/addtodo.h"
#include "action/setvisibilityfilter.h"
#include "action/toggletodo.h"
#include "redux.h"
#include "state.h"

#include <iostream>

auto drawVisibilityFilter() {
  return redux::View{ [](state::VisibilityFilter) {
    std::cout << "Draw VisibilityFilter\n";
  } };
}

auto drawTodos() {
  return redux::View{ [](state::TodoApp) { std::cout << "Draw Todos\n"; } };
}

int main() {
  auto const visibilityFilter = redux::Reducer<action::SetVisibilityFilter>{};
  auto const todos   = redux::Reducer<action::AddTodo, action::ToggleTodo>{};
  auto const reducer = redux::CombinedReducer{ todos, visibilityFilter };
  auto const combinedView = combineViews(drawVisibilityFilter(), drawTodos());

  auto store = redux::Store{ state::TodoApp{}, reducer, combinedView };

  store.dispatch(action::addTodo("Learn about actions"));
  store.dispatch(action::addTodo("Learn about reducers"));
  store.dispatch(action::addTodo("Learn about store"));
  store.dispatch(action::toggleTodo(0));
  store.dispatch(action::toggleTodo(1));

  store.dispatch(action::setVisibilityFilter(state::VisibilityFilter::SHOW_COMPLETED));

  // auto event = char{};

  // while (std::cin >> event)
  //  dispatch (store, event);
}
