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
      update();
    }

    template <class Action>
    void dispatch(Action&& action) {
      mEventLoop.post([&, applyAction{ std::forward<Action>(action) }] {
        mState = mReducer(std::move(mState), applyAction);
        // mState = mReducer(mState, applyAction);

        mReducer.updateAffected<Action>(updateViews(), mState);
      });
    }

    void update() {
      mReducer.updateAll(updateViews(), mState);
    }

    State getState() const {
      return mState;
    }

  private:
    auto updateViews() const {
      return [&](auto&& state) {
        std::invoke(mView, std::forward<decltype(state)>(state));
      };
    }

    State     mState;
    Reducer   mReducer;
    View      mView;
    EventLoop mEventLoop;
  };

  template <class State, class Reducer, class View>
  Store(State&& initial, Reducer&& reducer, View&& view)
  ->Store<std::decay_t<State>, std::decay_t<Reducer>, std::decay_t<View>>;
}
