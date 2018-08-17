#pragma once
#include "redux/detail/get.h"
#include "redux/detail/overloaded.h"
#include <functional>

namespace redux {
  template <class Action>
  struct ModifyState
  {
    template <class State>
    auto operator()(State&& state, Action action) const {
      return std::invoke(action, std::forward<State>(state));
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
  struct Reducer : Overloaded<ModifyState<Actions>..., ForwardState>
  {
    template <class Updater, class State>
    void updateAll(Updater&& updater, State&& state) const {
      std::invoke(std::forward<Updater>(updater), std::forward<State>(state));
    }

    template <class Action, class Updater, class State>
    void updateAffected(Updater&& updater, State&& state) const {
      if constexpr (std::disjunction_v<std::is_same<Actions, Action>...>)
        std::invoke(std::forward<Updater>(updater), std::forward<State>(state));
    }
  };

  template <class... Reducers>
  struct CombinedReducer
  {
    template <class... ParamReducers>
    explicit CombinedReducer(ParamReducers&&... reducers)
    : mReducers(std::forward<ParamReducers>(reducers)...) {
    }

    template <class State, class Action>
    decltype(auto) operator()(State&& state, Action&& action) const {
      return combined(std::forward<State>(state), std::forward<Action>(action),
                      std::make_index_sequence<sizeof...(Reducers)>{});
    }

    template <class Updater, class State>
    void updateAll(Updater&& updater, State&& state) const {
      updateAll(std::forward<Updater>(updater), std::forward<State>(state),
                std::make_index_sequence<sizeof...(Reducers)>{});
    }

    template <class Action, class Updater, class State>
    void updateAffected(Updater&& updater, State&& state) const {
      updateAffected<Action>(std::forward<Updater>(updater), std::forward<State>(state),
                             std::make_index_sequence<sizeof...(Reducers)>{});
    }

  private:
    template <class State, class Action, size_t... Indexes>
    auto combined(State&& state, Action&& action, std::index_sequence<Indexes...>) const {
      return std::decay_t<State>{ (
      std::invoke(std::get<Indexes>(mReducers),
                  redux::get<Indexes, sizeof...(Indexes)>(std::forward<State>(state)),
                  std::forward<Action>(action)))... };
    }

    template <class Updater, class State, size_t... Indexes>
    void updateAll(Updater&& updater, State&& state, std::index_sequence<Indexes...>) const {
      ((void)std::get<Indexes>(mReducers).updateAll(
       updater, redux::get<Indexes, sizeof...(Indexes)>(state)),
       ...);

      std::invoke(updater, state);
    }

    template <class Action, class Updater, class State, size_t... Indexes>
    void updateAffected(Updater&& updater, State&& state, std::index_sequence<Indexes...>) const {
      ((void)std::get<Indexes>(mReducers).updateAffected<Action>(
       [=](auto&& child) {
         updater(child);
         updater(state);
       },
       redux::get<Indexes, sizeof...(Indexes)>(state)),
       ...);
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
