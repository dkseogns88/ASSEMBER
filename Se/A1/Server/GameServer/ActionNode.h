#pragma once
#include "Node.h"

// Action ���: ���� �ൿ�� ����
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
