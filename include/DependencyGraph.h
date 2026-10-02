#ifndef DEPENDENCY_GRAPH_H
#define DEPENDENCY_GRAPH_H

#include <vector>
#include <unordered_map>

// DependencyGraph.h  (Lab 8 : BFS / DFS, Lab 11 : Topological Sort)
// -------------------------------------------------------------------------
// Manual directed GRAPH stored as an ADJACENCY LIST, representing "must run
// before" dependencies between processes. Supports BFS, DFS, and a
// Kahn's-algorithm Topological Sort (reports a cycle if one exists).

struct DependencyGraph {
    std::unordered_map<int, std::vector<int>> adjList; // pid -> dependents
    std::unordered_map<int, int> inDegree;              // pid -> prerequisite count
};

void addNode(DependencyGraph &g, int pid);
void removeNode(DependencyGraph &g, int pid);
bool addDependency(DependencyGraph &g, int fromPid, int toPid); // fromPid must finish before toPid
bool hasNode(const DependencyGraph &g, int pid);
int nodeCount(const DependencyGraph &g);

std::vector<int> bfsTraversal(const DependencyGraph &g, int startPid);
std::vector<int> dfsTraversal(const DependencyGraph &g, int startPid);
bool topoSort(const DependencyGraph &g, std::vector<int> &result); // false => cycle

void printAdjList(const DependencyGraph &g);
void clearGraph(DependencyGraph &g);

#endif
