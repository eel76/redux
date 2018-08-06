#pragma once
#include "eventloop.h"
#include <functional>

namespace redux {
  template <class State, class Reducer>
  class Store
  {
    public:
    template <class StateParam, class ReducerParam, class View>
    Store(StateParam&& initial, ReducerParam&& reducer, View&& view)
    : mState(std::forward<StateParam>(initial))
    , mReducer(std::forward<ReducerParam>(reducer))
    , mView(std::forward<View>(view))
    , mEventLoop() {
    }

    void update() {
      mEventLoop.post([=] { mView(mState); });
    }

    template <class Action>
    void dispatch(Action&& action) {
      mEventLoop.post([=, applyAction{ std::forward<Action>(action) }] {
        mState = mReducer(std::move(mState), applyAction);
      });

      update();
    }

    private:
    std::decay_t<State>               mState;
    std::decay_t<Reducer>             mReducer;
    std::function<void(State const&)> mView;
    EventLoop                         mEventLoop;
  };

  template <class Model, class Reducer, class View>
  Store(Model&& initial, Reducer&& reducer, View&& view)->Store<Model, Reducer>;
}
