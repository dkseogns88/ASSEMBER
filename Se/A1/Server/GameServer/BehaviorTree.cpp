#include "pch.h"
#include "BehaviorTree.h"
#include "Node.h"

BehaviorTree::BehaviorTree(Node* root)
	: root(root)
{

}

void BehaviorTree::execute()
{
    std::cout << "Executing Behavior Tree..." << std::endl;
    bool result = root->run();
    if (result) {
        std::cout << "Behavior Tree executed successfully." << std::endl;
    }
    else {
        std::cout << "Behavior Tree execution failed." << std::endl;
    }

    cout << endl << endl;
}
