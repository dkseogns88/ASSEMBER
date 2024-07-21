#include "pch.h"
#include "PathFinder.h"
#include "fstream"
#include "Room.h"
#include <limits>

PathFinderRef GPathFinder = make_shared<PathFinder>();

vector<FVector> PathFinder::AStar(FVector& Start, FVector& Goal, map<FVector, vector<FVector>>& Edges)
{
	if (!IsRead)
	{
		cout << "레벨 정보 파일 안읽었음" << endl;
		return {};
	}

	priority_queue<PNode, vector<PNode>, greater<PNode>> pq;
	unordered_set<FVector, FVectorHash> closed; // 방문 여부
	
	allNodes.clear();
	path.clear();

	//유클리드 거리 계산, 초기값
	{
		float g = 0;
		float h = 10 * (abs(Goal.x - Start.x) + abs(Goal.y - Start.y) + abs(Goal.z - Start.z));
		PNode startNode = { Start, g, h, nullptr };
		pq.push(startNode);
	}

	cout << "A* 실행" << endl;
	while (!pq.empty())
	{
		PNode current = pq.top();
		pq.pop();

		if (current.Position == Goal)
		{
			//경로를 역추적해서 반환
			PNode* pathNode = &current;

			while (pathNode != nullptr)
			{
				path.push_back(pathNode->Position);
				pathNode = pathNode->Parent;
			}

			reverse(path.begin(), path.end());
			
			cout << "A* 성공" << endl;
			return path;
		}

		if (closed.find(current.Position) != closed.end()) continue;

		closed.insert(current.Position);

		for (const FVector& neighbor : Edges[current.Position])
		{
			if (closed.find(neighbor) != closed.end())
				continue;

			float gridSize = 200.f;
			FVector Dir = (neighbor - current.Position) / gridSize;

			float cost = 0;
			if (Dir.x != 0 && Dir.y != 0)
			{
				cost = 14.f;
			}
			else cost = 10.f;

			float g = current.GCost = cost;
			float h = 10 * (abs(Goal.x - neighbor.x) + abs(Goal.y - neighbor.y) + abs(Goal.z - neighbor.z));

			if (allNodes.find(neighbor) == allNodes.end())
			{
				PNode* neighborNode = new PNode{ neighbor, g, h, new PNode(current) };
				pq.push(*neighborNode);
				allNodes[neighbor] = neighborNode;
			}
			else
			{
				PNode* neighborNode = allNodes[neighbor];
				if (g + h < neighborNode->FCost())
				{
					neighborNode->GCost = g;
					neighborNode->Parent = new PNode(current);
					pq.push(*neighborNode);
				}
			}
		}
	}

	cout << "A* 실패" << endl;

	return {};
}

void PathFinder::ReadFile()
{
	ifstream file("../../A1/Saved/SavedMaps/EdgesMap.txt");
	string line;
	FVector currentKey;

	if (file) {
		while (getline(file, line))
		{
			line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

			if (line.substr(0, 4) == "Key:")
			{
				currentKey = parseToVector(line.substr(4));
				EdgeMap[currentKey];
			}
			else if (line.substr(0, 6) == "Value:")
			{
				FVector value = parseToVector(line.substr(6));
				EdgeMap[currentKey].push_back(value);
			}
		}
		IsRead = true;
	}
	else return;



}

FVector PathFinder::CreateVec(float x, float y, float z)
{
	const FVector& target{ x, y, z };

	FVector closestKey;
	float minDistance = std::numeric_limits<float>::infinity();

	for (const auto& pair : EdgeMap) {
		float distance = ManhattanDistance(pair.first, target);
		if (distance < minDistance) {
			minDistance = distance;
			closestKey = pair.first;
		}
	}

	return closestKey;
}

FVector PathFinder::ReturnRandomPos()
{
	std::vector<FVector> keys;
	for (const auto& pair : EdgeMap) {
		keys.push_back(pair.first);
	}

	std::srand(std::time(0));
	int randomIndex = std::rand() % keys.size();
	FVector randomKey = keys[randomIndex];

	return randomKey;
}

FVector PathFinder::parseToVector(const string& str)
{
	FVector vec;
	sscanf_s(str.c_str(), "(%f,%f,%f)", &vec.x, &vec.y, &vec.z);
	return vec;

}

float PathFinder::ManhattanDistance(const FVector v1, const FVector v2)
{
	return std::abs(v1.x - v2.x) + std::abs(v1.y - v2.y) + std::abs(v1.z - v2.z);
}
