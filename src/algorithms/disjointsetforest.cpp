#include "disjointsetforest.h"
#include <assert.h>

DisjointSetForest::DisjointSetForest()
{
}

/*virtual*/ DisjointSetForest::~DisjointSetForest()
{
}

/*virtual*/ std::vector<std::pair<int, int>> DisjointSetForest::SpanningTree(int vertices, const Graph& adjacencyList)
{
	assert(vertices == (signed)adjacencyList.size());

	this->spanningtree.clear();

	struct Node
	{
		int vertex;
		Node* parent;
	};

	// Initialize singleton sets.
	std::vector<Node> nodeArray;
	for (int i = 0; i < vertices; i++)
		nodeArray.push_back(Node{ i, nullptr });
	
	struct Edge
	{
		Node* nodeA;
		Node* nodeB;
	};

	// Create list of graph edges.
	std::vector<Edge> edgeArray;
	for (int i = 0; i < (signed)adjacencyList.size(); i++)
		for (auto p : adjacencyList[i])
			if (std::get<0>(p) != -1)
				edgeArray.push_back(Edge{ &nodeArray[i], &nodeArray[std::get<0>(p)] });

	assert(signed(edgeArray.size()) >= vertices);

	auto findGroupRep = [](Node* node) -> Node* {
		Node* rep = node;
		while (rep->parent)
			rep = rep->parent;
		// This loop is not needed for correctness of the algorithm.
		// It is here merely as an optimization.
		while (node->parent && node->parent != rep)
		{
			Node* next = node->parent;
			node->parent = rep;
			node = next;
		}
		return rep;
	};

	auto sameGroup = [&findGroupRep](Node* nodeA, Node* nodeB) -> bool {
		Node* repA = findGroupRep(nodeA);
		Node* repB = findGroupRep(nodeB);
		return repA == repB;
	};

	auto mergeGroups = [&findGroupRep](Node* nodeA, Node* nodeB) {
		Node* repA = findGroupRep(nodeA);
		Node* repB = findGroupRep(nodeB);
		// Arbitrarily parent one to the other.
		repA->parent = repB;
	};

	// Find a prime that does not divide edgeArray.size().
	int primes[] = { 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 };
	int numPrimes = sizeof(primes) / sizeof(int);
	int i, j = std::uniform_int_distribution<int>(0, numPrimes - 1)(this->generator);
	for (i = 0; i < numPrimes; i++)
	{
		if (edgeArray.size() % primes[i] != 0)
			break;
		j = (j + 1) % numPrimes;
	}
	assert(i != numPrimes);
	int chosenPrime = primes[i];
	
	// This is not a truly random method of walking the edge array, but that's the idea here.
	// According to group-theory, we will not repeat an edge.
	j = chosenPrime;
	for (i = 0; i < (signed)edgeArray.size(); i++)
	{
		j = (j + chosenPrime) % edgeArray.size();
		Edge& edge = edgeArray[j];
		if (!sameGroup(edge.nodeA, edge.nodeB))
		{
			mergeGroups(edge.nodeA, edge.nodeB);
			this->spanningtree.push_back(std::pair<int, int>(edge.nodeA->vertex, edge.nodeB->vertex));
			if (signed(this->spanningtree.size()) >= vertices)
				break;
		}
	}

	return this->spanningtree;
}