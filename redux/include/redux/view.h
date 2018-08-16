#pragma once
#include <tuple>

namespace redux {

  template <class Updater>
  struct View : Updater
  {
    using Updater::operator();
    template <class Unknown>
    void operator()(Unknown&&) const {
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
    void operator()(State state) const {
      invoke(mViews, state,
             std::make_index_sequence<std::tuple_size<decltype(mViews)>{}>{});
    }

    private:
    template <class Tuple, size_t... Is, class State>
    static void invoke(Tuple t, State state, std::index_sequence<Is...>) {
      using swallow = int[];
      (void)swallow{ 1, (std::invoke(get<Is>(t), state), void(), int{})... };
    }
    std::tuple<Views...> mViews;
  };
  template <class... ParamViews>
  CombinedView(ParamViews...)->CombinedView<std::decay_t<ParamViews>...>;

  template <class... Views>
  auto combineViews(Views&&... views) {
    return CombinedView{ std::forward<Views>(views)... };
  }
}
