#pragma once


struct PNode
{
    FVector Position;
    float GCost;
    float HCost;
    PNode* Parent;

    float FCost() const { return GCost + HCost; }
    bool operator>(const PNode& Other) const { return FCost() > Other.FCost(); }
};

class PathFinder
{
public:
    map<FVector, vector<FVector>> EdgeMap;
    bool IsRead = false;
    vector<FVector> path;
    unordered_map<FVector, PNode*, FVectorHash> allNodes;


public:
    vector<FVector> AStar(FVector& Start, FVector& Goal, map<FVector, vector<FVector>>& Edges);
    void ReadFile();

    FVector CreateVec(float x, float y, float z);

    FVector ReturnRandomPos();

private:
    FVector parseToVector(const string& str);
    float ManhattanDistance(const FVector v1, const FVector v2);
};

extern PathFinderRef GPathFinder;