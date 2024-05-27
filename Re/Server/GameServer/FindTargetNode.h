#pragma once
#include "Node.h"
#include "Monster.h"

class FindTargetNode : public Node {
public:
    FindTargetNode(MonsterRef monster, Node* targetSelector, Node* noTargetSequence)
        : _monster(monster), targetSelector(targetSelector), noTargetSequence(noTargetSequence) {}

    virtual bool run() override {
        if (MonsterRef monster = _monster.lock()) 
        {
            if (monster->_target.lock()) {
                return targetSelector->run();
            }
            else {
                return noTargetSequence->run();
            }
        }
        return false;
    }

private:
    weak_ptr<class Monster> _monster;

    Node* targetSelector;
    Node* noTargetSequence;
};