#pragma once
#include "state/visibilityfilter.h"

namespace action {
  struct SetVisibilityFilter
  {
    state::VisibilityFilter visibilityFilter;

    state::VisibilityFilter operator()(state::VisibilityFilter) const {
      return visibilityFilter;
    }
  };

  auto setVisibilityFilter(state::VisibilityFilter visibilityFilter) {
    return SetVisibilityFilter{ visibilityFilter };
  }
}
