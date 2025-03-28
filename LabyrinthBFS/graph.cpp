#include "graph.h"

Graph::Graph() {}

Graph::~Graph()
{
    for(Node* n : m_nodes) {
        delete n;
    }
}

void Graph::buildFromMap(Map &map)
{
    const std::vector<std::vector<Map::PosType>>& matrix = map.getMatrix();
    for(int i = 0 ; i < matrix.size(); ++i) {
        for(int j = 0 ; j < matrix[0].size(); ++j) {
            if(matrix[i][j]==Map::PosType::Wall) continue;
            m_lookupNodeTable[std::pair<int,int>(i,j)] = addNode(matrix[i][j])->getId();
            if(matrix[i][j]==Map::PosType::Entrance) m_entrance = m_nodes[m_nodes.size()-1];
            m_lookupCoordTable[m_nodes[m_nodes.size()-1]] = std::pair<int,int>(i,j);
        }
    }
    int dx[2]{1,0};
    int dy[2]{0,1};
    for(int i = 0 ; i < matrix.size(); ++i) {
        for(int j = 0 ; j < matrix[0].size(); ++j) {
            if(matrix[i][j]==Map::PosType::Wall) continue;
            Node*& currNode = m_nodes[m_lookupNodeTable[std::pair<int,int>(i,j)]];
            for(int d=0;d<2;++d) {
                int neighbourX = i + dx[d], neighbourY = j + dy[d];
                if(neighbourX < 0 || neighbourY < 0 || neighbourX >= matrix.size() || neighbourY >= matrix[0].size()) continue;
                if(matrix[neighbourX][neighbourY]!=Map::PosType::Wall) {
                    Node*& neighbourNode = m_nodes[m_lookupNodeTable[std::pair<int,int>(neighbourX,neighbourY)]];
                    addEdge(currNode,neighbourNode);
                    addEdge(neighbourNode,currNode);
                }
            }
        }
    }
}

void Graph::reset()
{
    if(!m_nodes.empty()) m_nodes.clear();
    if(!m_edges.empty()) m_edges.clear();
}

std::vector<Node *> &Graph::getNodes()
{
    return m_nodes;
}

std::unordered_map<Node *, std::vector<Node *> > &Graph::getEdges()
{
    return m_edges;
}

std::unordered_map<Node *, std::pair<int, int> > &Graph::getLookupCoordTable()
{
    return m_lookupCoordTable;
}

Node* Graph::addNode(Map::PosType type)
{
    Node* newNode = new Node(m_nodes.size(),type);
    m_nodes.emplace_back(newNode);
    return newNode;
}

void Graph::addEdge(Node *first, Node *second)
{
    m_edges[first].push_back(second);
}



std::vector<std::list<Node *> > Graph::findPaths()
{
    std::vector<std::list<Node*>> paths;
    if(m_nodes.size() <=0) return paths;
    std::vector<bool> closedList(m_nodes.size(),false);
    std::queue<std::list<Node*>> openList;
    std::list<Node*> currList;
    Node* currNode;
    currList.push_back(m_entrance);
    openList.push(currList);
    closedList[m_entrance->getId()] = true;
    while(!(openList.empty())) {
        currList = openList.front();
        openList.pop();
        currNode = currList.back();
        if(currNode->getType()==Map::PosType::Exit) paths.push_back(currList);
        for(Node* neighbour : m_edges[currNode]) {
            if(closedList[neighbour->getId()]==false) {
                std::list<Node*> toPushList(currList);
                toPushList.push_back(neighbour);
                openList.push(toPushList);
                closedList[neighbour->getId()] = true;
            }
        }
    }
    for(const std::list<Node*>& list : paths) {
        for(Node* curr : list) {
            if(curr->getType()==Map::PosType::Path) curr->setType(Map::PosType::Walked);
        }
    }
    return paths;

}



