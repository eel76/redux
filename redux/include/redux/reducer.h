#pragma once
#include "redux/detail/get.h"
#include "redux/detail/overloaded.h"
#include <functional>

namespace redux {
  template <class Action>
  struct ModifyState
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
  struct Reducer : Overloaded<ModifyState<Actions>..., ForwardState>
  {
    template <class Visitor, class State>
    void visit(Visitor&& visitor, State&& state) const {
      std::invoke(std::forward<Visitor>(visitor), std::forward<State>(state));
    }

    template <class Action, class Visitor, class State>
    void update(Visitor&& visitor, State&& state) const {
      if constexpr (std::disjunction_v<std::is_same<Actions, Action>...>)
        std::invoke(std::forward<Visitor>(visitor), std::forward<State>(state));
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

    template <class Visitor, class State>
    void visit(Visitor&& visitor, State&& state) const {
      visit(std::forward<Visitor>(visitor), std::forward<State>(state),
            std::make_index_sequence<sizeof...(Reducers)>{});
    }

    template <class Action, class Visitor, class State>
    void update(Visitor&& visitor, State&& state) const {
      update<Action>(std::forward<Visitor>(visitor), std::forward<State>(state),
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

    template <class Visitor, class State, size_t... Indexes>
    void visit(Visitor&& visitor, State&& state, std::index_sequence<Indexes...>) const {
      using ignored = int[];
      (void)ignored{
        1,
        (std::invoke([reducer{ std::get<Indexes>(mReducers) }, visitor](
                     auto&& childState) { reducer.visit(visitor, childState); },
                     redux::get<Indexes, sizeof...(Indexes)>(state)),
         void(), int{})...
      };

      std::invoke(visitor, state);
    }

    template <class Action, class Visitor, class State, size_t... Indexes>
    void update(Visitor&& visitor, State&& state, std::index_sequence<Indexes...>) const {
      auto const childVisitor = [=](auto&& childState) {
        visitor(childState);
        visitor(state);
      };

      using ignored = int[];
      (void)ignored{ 1, (std::invoke(
                         [reducer{ std::get<Indexes>(mReducers) }, childVisitor](auto&& childState) {
                           reducer.update<Action>(childVisitor, childState);
                         },
                         redux::get<Indexes, sizeof...(Indexes)>(state)),
                         void(), int{})... };
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
