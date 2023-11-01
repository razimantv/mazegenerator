#ifndef DISJOINTSETFORESTALGORITHM_H
#define DISJOINTSETFORESTALGORITHM_H

#include "spanningtreealgorithm.h"

class DisjointSetForest : public SpanningtreeAlgorithm
{
public:
	DisjointSetForest();
	virtual ~DisjointSetForest();

	virtual std::vector<std::pair<int, int>> SpanningTree(int vertices, const Graph& adjacencyList) override;
};

#endif /* end of include guard: DISJOINTSETFORESTALGORITHM_H */