#pragma once
#include "redux/detail/get.h"
#include "redux/detail/overloaded.h"
#include <functional>

namespace redux {
  template <class Action>
  struct Invoke
  {
    template <class State>
    auto operator()(State&& state, Action&& action) const {
      return std::invoke(std::forward<Action>(action), std::forward<State>(state));
    }
  };

  struct ForwardState
  {
    template <class State, class Unknown>
    decltype(auto) operator()(State&& state, Unknown&&) const {
      return std::forward<State>(state);
    }
  };

  template <class... Actions>
  struct Reducer : Overloaded<Invoke<Actions>..., ForwardState>
  {};

  template <class... Reducers>
  struct CombinedReducer
  {
    template <class... ParamReducers>
    explicit CombinedReducer(ParamReducers&&... reducers)
    : mReducers(std::forward<ParamReducers>(reducers)...) {
    }

    template <class State, class Action>
    auto operator()(State&& state, Action&& action) const {
      return combined(std::forward<State>(state), std::forward<Action>(action),
                      std::make_index_sequence<sizeof...(Reducers)>{});
    }

    private:
    template <class State, class Action, size_t... Indexes>
    auto combined(State&& state, Action&& action, std::index_sequence<Indexes...>) const {
      return std::decay_t<State>{ (
      std::invoke(std::get<Indexes>(mReducers),
                  redux::get<Indexes, sizeof...(Indexes)>(state), action))... };
    }

    std::tuple<Reducers...> mReducers;
  };

  template <class... ParamReducers>
  CombinedReducer(ParamReducers&&...)->CombinedReducer<std::decay_t<ParamReducers>...>;

  template <class... Reducers>
  auto combineReducers(Reducers&&... reducers) {
    return CombinedReducer{ std::forward<Reducers>(reducers)... };
  }
}
