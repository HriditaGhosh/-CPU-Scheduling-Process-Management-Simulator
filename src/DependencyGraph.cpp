#include "DependencyGraph.h"
#include <queue>
#include <stack>
#include <unordered_set>
#include <iostream>
using namespace std;

void addNode(DependencyGraph &g, int pid) {
    if (g.adjList.find(pid) == g.adjList.end()) {
        g.adjList[pid] = vector<int>();
        g.inDegree[pid] = 0;
    }
}

void removeNode(DependencyGraph &g, int pid) {
    if (g.adjList.find(pid) == g.adjList.end()) return;

    g.adjList.erase(pid);
    g.inDegree.erase(pid);

    for (auto &entry : g.adjList) {
        auto &neighbors = entry.second;
        for (auto it = neighbors.begin(); it != neighbors.end();) {
            if (*it == pid) it = neighbors.erase(it);
            else ++it;
        }
    }
    for (auto &entry : g.inDegree) entry.second = 0;
    for (auto &entry : g.adjList) {
        for (int dependent : entry.second) g.inDegree[dependent]++;
    }
}

bool addDependency(DependencyGraph &g, int fromPid, int toPid) {
    if (g.adjList.find(fromPid) == g.adjList.end() || g.adjList.find(toPid) == g.adjList.end()) return false;
    if (fromPid == toPid) return false;
    g.adjList[fromPid].push_back(toPid);
    g.inDegree[toPid]++;
    return true;
}

bool hasNode(const DependencyGraph &g, int pid) {
    return g.adjList.find(pid) != g.adjList.end();
}

int nodeCount(const DependencyGraph &g) {
    return (int)g.adjList.size();
}

vector<int> bfsTraversal(const DependencyGraph &g, int startPid) {
    vector<int> order;
    if (!hasNode(g, startPid)) return order;

    unordered_set<int> visited;
    queue<int> q;
    q.push(startPid);
    visited.insert(startPid);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        order.push_back(current);

        auto it = g.adjList.find(current);
        if (it != g.adjList.end()) {
            for (int neighbor : it->second) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
    }
    return order;
}

vector<int> dfsTraversal(const DependencyGraph &g, int startPid) {
    vector<int> order;
    if (!hasNode(g, startPid)) return order;

    unordered_set<int> visited;
    stack<int> st;
    st.push(startPid);

    while (!st.empty()) {
        int current = st.top();
        st.pop();
        if (visited.find(current) != visited.end()) continue;
        visited.insert(current);
        order.push_back(current);

        auto it = g.adjList.find(current);
        if (it != g.adjList.end()) {
            const auto &neighbors = it->second;
            for (auto rit = neighbors.rbegin(); rit != neighbors.rend(); ++rit) {
                if (visited.find(*rit) == visited.end()) st.push(*rit);
            }
        }
    }
    return order;
}

bool topoSort(const DependencyGraph &g, vector<int> &result) {
    result.clear();
    unordered_map<int, int> degreeCopy = g.inDegree;
    queue<int> q;

    for (auto &entry : degreeCopy) if (entry.second == 0) q.push(entry.first);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);

        auto it = g.adjList.find(current);
        if (it != g.adjList.end()) {
            for (int neighbor : it->second) {
                degreeCopy[neighbor]--;
                if (degreeCopy[neighbor] == 0) q.push(neighbor);
            }
        }
    }
    return result.size() == g.adjList.size();
}

void printAdjList(const DependencyGraph &g) {
    if (g.adjList.empty()) {
        cout << "(no dependency graph nodes yet)\n";
        return;
    }
    cout << "\n--- Dependency Graph (Adjacency List) ---\n";
    for (auto &entry : g.adjList) {
        cout << "P" << entry.first << " -> ";
        if (entry.second.empty()) {
            cout << "(no dependents)";
        } else {
            for (size_t i = 0; i < entry.second.size(); ++i) {
                cout << "P" << entry.second[i];
                if (i + 1 < entry.second.size()) cout << ", ";
            }
        }
        cout << "\n";
    }
}

void clearGraph(DependencyGraph &g) {
    g.adjList.clear();
    g.inDegree.clear();
}
