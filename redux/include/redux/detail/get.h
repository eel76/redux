#pragma once
#include <tuple>

namespace redux {
  inline namespace detail {
    template <size_t index, size_t size>
  struct get_impl;

    template <size_t index>
    struct get_impl<index, 1>
    {
      template <class Structure>
      decltype(auto) operator()(Structure&& s) {
        auto&& [a] = std::forward<Structure>(s);
        return std::get<index>(std::forward_as_tuple(a));
      }
    };

    template <size_t index>
    struct get_impl<index, 2>
    {
      template <class Structure>
      decltype(auto) operator()(Structure&& s) {
        auto&& [a,b] = std::forward<Structure>(s);
        return std::get<index>(std::forward_as_tuple(a, b));
      }
    };

    template <size_t index>
    struct get_impl<index, 3>
    {
      template <class Structure>
      decltype(auto) operator()(Structure&& s) {
        auto&& [a, b, c] = std::forward<Structure>(s);
        return std::get<index>(std::forward_as_tuple(a, b, c));
      }
    };

    template <size_t index, size_t size, class Structure>
    decltype(auto) get(Structure&& s) {
      return get_impl<index, size>{}(std::forward<Structure>(s));
    }
  }

}
