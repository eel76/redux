#pragma once

namespace redux {

  template <class Ts>
  struct View : Ts
  {
    using Ts::operator();
    template <class... Ignored>
    void operator()(Ignored...) const {
    }
  };

  template <class Ts>
  View(Ts)->View<Ts>;
}
