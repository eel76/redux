#pragma once
#include "detail/get.h"
#include "detail/overloaded.h"
#include <functional>

namespace redux {
  template <class Action>
  struct Invoke
  {
    template <class State>
    auto operator()(State state, Action action) const {
      return action(state);
    }
  };

  struct Ignored
  {
    template <class State, class Action>
    auto operator()(State state, Action&&) const {
      return state;
    }
  };

  template <class... Actions>
  using Reducer = Overloaded<Invoke<Actions>..., Ignored>;

  template <size_t... Indexes, class ReducerTuple>
  auto combineReducersImpl(std::index_sequence<Indexes...>, ReducerTuple&& reducerTuple) {
    return [reducers{ std::forward<ReducerTuple>(
           reducerTuple) }](auto state, auto&& action) -> decltype(state) {
      return { (std::invoke(std::get<Indexes>(reducers),
                            redux::get<Indexes, sizeof...(Indexes)>(state), action))... };
    };
  }

  template <class... Reducers>
  auto combineReducers(Reducers&&... reducers) {
    return combineReducersImpl(std::make_index_sequence<sizeof...(Reducers)>{},
                               std::make_tuple(std::forward<Reducers>(reducers)...));
  }
}
