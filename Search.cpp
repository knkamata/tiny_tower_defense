#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

/*
 * Weighted Graph consist of
 * - Edges: Each of the edges has nodes from and to, and weight
 * - Nodes: Each of the nodes has edges comming into the node
 */

struct WeightedEdge
{
	struct WeightedGraphNode	*mFrom;
	struct WeightedGraphNode	*mTo;
	float						mWeight;
};

struct WeightedGraphNode
{
	vector<WeightedEdge *>	mEdges;
};

// Weighted graph that has the vector of nodes
struct WeightedGraph
{
	vector<WeightedGraphNode *>	mNodes;	// Start
};

float ComputeHeuristic(const WeightedGraphNode *from, const WeightedGraphNode *goal)
{
	// TODO: Use Manhattan distance, Euclidean distance, etc.
	return 0.0f;
}

/*
 * A* search using weighted graph
 * - Edges: Each of the edges has nodes from and to, and weight
 * - Nodes: Each of the nodes has edges comming into the node
 * g(x): Actual path cost from start to x
 * h(x): Heuristic cost of x
 * f(x) = g(x) + h(x)
 * A* search selects a path that has the smallest f(x)
 */

struct AStarScratch
{
	const WeightedEdge*	mParentEdge = nullptr;
	float				mHeuristic = 0.0f;
	float				mActualFromStart = 0.0f;
	bool				mInOpenSet = false;
	bool				mInClosedSet = false;
};

using AStarMap = unordered_map<const WeightedGraphNode*, AStarScratch>;

bool AStar(
	const WeightedGraph &g,
	const WeightedGraphNode *start,
	const WeightedGraphNode *goal,
	AStarMap &outMap)
{
	vector<const WeightedGraphNode *>	openSet;
	const WeightedGraphNode				*current = start;

	outMap[current].mInOpenSet = true;
	do
	{
		for (const WeightedEdge *edge : current->mEdges)
		{
			const WeightedGraphNode *neighbor = edge->mTo;
			AStarScratch &data = outMap[neighbor];
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// Compute actual cost that is the parent's plus const of traversing edge
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					// Compute what new actual cost (g(x)) is if current becomes parent
					float newCost = outMap[current].mActualFromStart + edge->mWeight;
					if (newCost < data.mActualFromStart)
					{
						data.mParentEdge = edge;
						data.mActualFromStart = newCost;
					}
				}
			}
		}
		if (openSet.empty())
			break;
		auto iter = min_element(openSet.begin(), openSet.end(), [&outMap](const WeightedGraphNode *a, const WeightedGraphNode *b) {
			// Calculate f(x) for nodes a, b
			float fOffA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
			float fOffB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
			return fOffA < fOffB;
		});
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = true;	// different from GBFS
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	return (current == goal) ? true : false;
}
