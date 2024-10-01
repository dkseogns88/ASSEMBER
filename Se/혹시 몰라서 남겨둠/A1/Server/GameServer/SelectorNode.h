#pragma once

#include "Node.h"
#include "Monster.h"

// �ڽ� ��� ��, �ϳ��� �����ϸ� ����
// ��� �ڽ� ��尡 fail�̸�, ����� ����� fail��

class SelectorNode : public Node {
public:
    SelectorNode(std::vector<Node*> children) : children(children) {}

    virtual bool run() override {
        for (Node* child : children) {
            if (child->run()) {
                return true;
            }
        }
        return false;
    }

    void addChild(Node* child) {
        children.push_back(child);
    }

private:
    std::vector<Node*> children;
};
