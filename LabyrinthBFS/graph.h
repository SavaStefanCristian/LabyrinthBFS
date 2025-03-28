#ifndef GRAPH_H
#define GRAPH_H
#pragma once
#include "node.h"
#include "map.h"
#include <vector>
#include <unordered_map>
#include <list>
#include <queue>

class Graph
{
public:
    Graph();
    ~Graph();

    void buildFromMap(Map& map);
    void reset();

    std::vector<Node*>& getNodes();
    std::unordered_map<Node*,std::vector<Node*>>& getEdges();
    std::unordered_map<Node*,std::pair<int,int>>& getLookupCoordTable();

    Node* addNode(Map::PosType type);
    void addEdge(Node* first, Node* second);

    std::vector<std::list<Node*>> findPaths();



private:
    struct pairHash{
        std::size_t operator()(const std::pair<int,int>& p) const {
            return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
        }
    };


    std::vector<Node*> m_nodes;
    std::unordered_map<Node*,std::vector<Node*>> m_edges;
    std::unordered_map<std::pair<int,int>,int, pairHash> m_lookupNodeTable;
    std::unordered_map<Node*,std::pair<int,int>> m_lookupCoordTable;
    Node* m_entrance;
};

#endif // GRAPH_H
