#pragma once

#include "Node.h"

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

private:
    std::vector<Node*> children;
};


// 2초간 대기하는 액션
static bool waitForSeconds() {
    std::cout << "WaitSeconds " << std::endl;
    return true; // 대기 성공
}