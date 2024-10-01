#pragma once

#include "Node.h"

// Sequence ���: �ڽ� ��带 ���������� ����
// �ϳ��� fail�̸� ��� ��ü�� fail
// ���� ���, ó������ running�� ������, �ڽ� ����� ��ġ�� �����. ����� running��.
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

