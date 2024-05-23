#include "pch.h"
#include "BehaviorTree.h"
#include "Node.h"

BehaviorTree::BehaviorTree(Node* root)
	: root(root)
{

}

void BehaviorTree::execute()
{
    bool result = root->run();
    if (result) {
        std::cout << "Behavior Tree executed successfully.\n\n\n";
    }
    else {
       std::cout << "Behavior Tree execution failed.\n\n\n";
    }
}
