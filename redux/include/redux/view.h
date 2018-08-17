#pragma once
#include <tuple>

namespace redux {

  template <class Updater>
  struct View : Updater
  {
    using Updater::operator();
    template <class Unknown>
    constexpr void operator()(Unknown&&) const {
    }
  };

  template <class Updater>
  View(Updater)->View<Updater>;

  template <class... Views>
  struct CombinedView
  {
    template <class... ParamViews>
    CombinedView(ParamViews&&... views)
    : mViews(std::forward<ParamViews>(views)...) {
    }

    template <class State>
    void operator()(State&& state) const {
      invoke(std::forward<State>(state),
             std::make_index_sequence<std::tuple_size<decltype(mViews)>{}>{});
    }

  private:
    template <class State, size_t... Is>
    void invoke(State state, std::index_sequence<Is...>) const {
      ((void)std::get<Is>(mViews)(state), ...);
    }
    std::tuple<Views...> mViews;
  };
  template <class... ParamViews>
  CombinedView(ParamViews...)->CombinedView<std::decay_t<ParamViews>...>;

  template <class... Updaters>
  auto combinedView(Updaters&&... updaters) {
    return CombinedView{ View{ std::forward<Updaters>(updaters) }... };
  }
}
