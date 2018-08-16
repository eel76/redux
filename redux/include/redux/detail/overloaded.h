#pragma once
#include <type_traits>

namespace redux { inline namespace detail {
  // template <class... Ts>
  // struct Overloaded : Ts...
  //{ using Ts::operator()...; };

  // template <class... Ts>
  // Overloaded(Ts...)->Overloaded<Ts...>;

  template <class... Fs>
  struct Overloaded : Fs...
  {
    Overloaded()
    : Fs{}... {
    }
    template <class... Ts>
    explicit Overloaded(Ts&&... ts)
    : Fs{ std::forward<Ts>(ts) }... {
    }

    using Fs::operator()...;
  };

  template <class... Ts>
  Overloaded(Ts&&...)->Overloaded<std::decay_t<Ts>...>;
}}
