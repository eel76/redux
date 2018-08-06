#include "action/addtodo.h"
#include "action/setvisibilityfilter.h"
#include "action/toggletodo.h"
#include "redux.h"
#include "state/todoapp.h"

#include <iostream>

auto draw() {
  return [](auto) { std::cout << "draw\n"; };
}

int main() {
  auto const visibilityFilter = redux::Reducer<action::SetVisibilityFilter>();
  auto const todos = redux::Reducer<action::AddTodo, action::ToggleTodo>();

  auto const todoApp = combineReducers(todos, visibilityFilter);

  auto store = redux::Store{ state::TodoApp{}, todoApp, draw() };

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
