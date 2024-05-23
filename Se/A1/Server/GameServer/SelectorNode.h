#pragma once

#include "Node.h"
#include "Monster.h"

// 자식 노드 중, 하나라도 성공하면 성공
// 모든 자식 노드가 fail이면, 노드의 결과도 fail임

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
