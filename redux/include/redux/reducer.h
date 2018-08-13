#pragma once
#include "redux/detail/get.h"
#include "redux/detail/overloaded.h"
#include <functional>

namespace redux {
  template <class Action>
  struct Invoke
  {
    template <class State, class Update>
    auto operator()(State state, Action action, Update update) const {
      return update(action(state));
    }
  };

  struct ForwardState
  {
    template <class State, class... Ignored>
    auto operator()(State state, Ignored&&...) const {
      return state;
    }
  };

  template <class... Actions>
  using Reducer = Overloaded<Invoke<Actions>..., ForwardState>;

  template <size_t... Indexes, class ReducerTuple>
  auto combineReducersImpl(std::index_sequence<Indexes...>, ReducerTuple&& reducerTuple) {
    return [reducers{ std::forward<ReducerTuple>(
           reducerTuple) }](auto state, auto action, auto update) -> decltype(state) {
      return update(decltype(state){
      (std::invoke(std::get<Indexes>(reducers),
                   redux::get<Indexes, sizeof...(Indexes)>(state), action, update))... });
    };
  }

  template <class... Reducers>
  auto combineReducers(Reducers&&... reducers) {
    return combineReducersImpl(std::make_index_sequence<sizeof...(Reducers)>{},
                               std::make_tuple(std::forward<Reducers>(reducers)...));
  }
}
