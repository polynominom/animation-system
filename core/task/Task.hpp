#pragma once
#include <future>
#include <functional>

namespace AnimationSystem
{
    // template <class R>
    // class Task
    // {
    // public:
    //     Task(): _mFuture(_mSignal.get_future());
    //     ~Task();
    //     Task(const Task &) = delete;
    //     Task &operator=(const Task &other) = delete;
    //     Task &operator=(Task &&other) = delete;
    //     Task(Task &&other) noexcept;    
    //     // TODO: add task progress with observed approach
    //     bool isRunning();   
    // private:
    //     std::unique_ptr<std::function<R>> _pFunc;
    //     std::future<R> _mFuture;
    //     std::promise<R> _mPromise;
    // };
} // namespace AnimationSystem
