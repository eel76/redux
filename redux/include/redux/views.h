#pragma once
#include <tuple>

namespace redux {

  template <class... Ts>
  struct Views
  {
    template <class... Args>
    Views(Args&&... args)
    : mViews(std::forward<Args>(args)...) {
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
    std::tuple<Ts...> mViews;
  };
  template <class... Ts>
  Views(Ts...)->Views<Ts...>;

  template <class... Vs>
  auto combineViews(Vs&&... views) {
    return Views{ std::forward<Vs>(views)... };
  }
}
