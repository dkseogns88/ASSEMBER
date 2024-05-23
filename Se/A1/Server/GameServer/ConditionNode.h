#pragma once
#include "Node.h"
class ConditionNode : public Node
{
public:
    ConditionNode(std::function<bool()> condition) : condition(condition) {}
    virtual bool run() override {
        return condition();
    }

private:
    std::function<bool()> condition;

};

