#pragma once
#include "coroutine"
#include "future"
#include "PathFinder.h"

class CoroutineJob
{
public:
    struct promise_type;
    using handle_type = coroutine_handle<promise_type>;

    CoroutineJob(handle_type handle) : _handle(handle) {}

    ~CoroutineJob()
    {
        if (_handle)
            _handle.destroy();
    }

    void resume()
    {
        if (_handle)
            _handle.resume();
    }

private:
    handle_type _handle;

public:
    struct promise_type
    {
        CoroutineJob get_return_object() { return CoroutineJob(handle_type::from_promise(*this)); }

        std::suspend_always initial_suspend()
        {
            cout << "Prepare Job" << endl;
            return {};
        }

        std::suspend_always final_suspend() noexcept
        {
            cout << "End Job" << endl;
            return {};
        }

        std::suspend_never return_void() { return {}; }

        void unhandled_exception() { }
    };

    static CoroutineJob CoroutineFunc(PathFinderRef pf, FVector& start, FVector& goal);
};

//CoroutineJob CoroutineJob::CoroutineFunc(PathFinderRef pf, FVector& start, FVector& goal)
//{
//    pf->AStar(start, goal, pf->EdgeMap);
//    co_await std::suspend_always();
//}
