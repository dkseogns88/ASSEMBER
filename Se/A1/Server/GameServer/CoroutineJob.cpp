#include "pch.h"
#include "CoroutineJob.h"

CoroutineJob CoroutineJob::CoroutineFunc(PathFinderRef pf, FVector& start, FVector& goal)
{
    pf->AStar(start, goal, pf->EdgeMap);
    co_await std::suspend_always();
}