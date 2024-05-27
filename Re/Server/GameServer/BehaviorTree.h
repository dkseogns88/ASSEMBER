#pragma once

class Node;

class BehaviorTree {
public:
    BehaviorTree(Node* root);
    void execute();

private:
    Node* root;
};