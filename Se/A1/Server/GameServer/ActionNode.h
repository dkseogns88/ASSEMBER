#pragma once
#include "Node.h"

// Action 노드: 실제 행동을 수행
class ActionNode : public Node
{
public:
    ActionNode(std::function<bool()> action) : action(action) {}
    virtual bool run() override {
        return action();
    }

private:
    std::function<bool()> action;
};
