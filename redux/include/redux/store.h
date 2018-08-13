#pragma once
#include "redux/eventloop.h"
#include <functional>

namespace redux {
  template <class State, class Reducer, class View>
  class Store
  {
    public:
    template <class StateParam, class ReducerParam, class ViewParam>
    Store(StateParam&& initial, ReducerParam&& reducer, ViewParam&& view)
    : mState(std::forward<StateParam>(initial))
    , mReducer(std::forward<ReducerParam>(reducer))
    , mView(std::forward<ViewParam>(view))
    , mEventLoop() {
    }

    template <class Action>
    void dispatch(Action&& action) {
      mEventLoop.post([=, applyAction{ std::forward<Action>(action) }] {
        mState = mReducer(std::move(mState), applyAction, [&](auto state) {
          mView(state);
          return state;
        });
      });
    }

    private:
    std::decay_t<State>   mState;
    std::decay_t<Reducer> mReducer;
    std::decay_t<View>    mView;
    EventLoop             mEventLoop;
  };

  template <class Model, class Reducer, class View>
  Store(Model&& initial, Reducer&& reducer, View&& view)->Store<Model, Reducer, View>;
}
