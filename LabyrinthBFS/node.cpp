#include "node.h"

Node::Node(int id, Map::PosType type) : m_id{id}, m_type{type} {}

void Node::setPos(QPointF pos)
{
    m_pos = pos;
}

void Node::setId(int id)
{
    m_id = id;
}

void Node::setType(Map::PosType type)
{
    m_type = type;
}

QPointF Node::getPos()
{
    return m_pos;
}

int Node::getId()
{
    return m_id;
}

Map::PosType Node::getType()
{
    return m_type;
}
