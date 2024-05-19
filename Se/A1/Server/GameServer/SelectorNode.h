#pragma once

#include "Node.h"

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

private:
    std::vector<Node*> children;
};


// 2�ʰ� ����ϴ� �׼�
static bool waitForSeconds() {
    std::cout << "WaitSeconds " << std::endl;
    return true; // ��� ����
}