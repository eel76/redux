#include "action/addtodo.h"
#include "action/setvisibilityfilter.h"
#include "action/toggletodo.h"
#include "redux.h"
#include "state.h"

#include <iostream>
#include <map>

auto drawVisibilityFilter() {
  return redux::View{ [](state::VisibilityFilter visibilityFilter) {
    auto filters = std::map<state::VisibilityFilter, char const*>{
      { { state::VisibilityFilter::SHOW_ALL, "show all" },
        { state::VisibilityFilter::SHOW_COMPLETED, "show completed" },
        { state::VisibilityFilter::SHOW_ACTIVE, "show active" } }
    };

    std::cout << "Filter: " << filters[visibilityFilter] << "\n";
  } };
}

auto drawTodos() {
  return redux::View{ [](state::AppState) { std::cout << "Draw Todos\n"; } };
}

int main() {
  auto const visibilityFilter = redux::Reducer<action::SetVisibilityFilter>{};
  auto const todos   = redux::Reducer<action::AddTodo, action::ToggleTodo>{};
  auto const reducer = redux::CombinedReducer{ todos, visibilityFilter };
  auto const combinedView = combineViews(drawVisibilityFilter(), drawTodos());

  auto store = redux::Store{ state::AppState{}, reducer, combinedView };

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
