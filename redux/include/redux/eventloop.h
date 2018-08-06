#pragma once

namespace redux {
  class EventLoop
  {
    public:
    template <class Event>
    void post(Event&& event) {
      event();
    }
  };
}
