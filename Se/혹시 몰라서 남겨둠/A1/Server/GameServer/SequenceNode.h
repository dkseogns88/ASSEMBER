#pragma once

#include "Node.h"

// Sequence 노드: 자식 노드를 순차적으로 실행
// 하나라도 fail이면 노드 자체가 fail
// 실행 결과, 처음으로 running이 나오면, 자식 노드의 위치를 기록함. 결과는 running임.
class SequenceNode : public Node
{
public:
    SequenceNode(std::vector<Node*> children) : children(children) {}

    virtual bool run() override {
        for (Node* child : children) {
            if (!child->run()) {
                return false;
            }
        }
        return true;
    }

    void addChild(Node* child) {
        children.push_back(child);
    }

private:
    std::vector<Node*> children;

};

