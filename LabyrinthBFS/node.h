#ifndef NODE_H
#define NODE_H
#pragma once
#include <QPointF>
#include "map.h"

class Node
{
public:
    Node(int id, Map::PosType type);
    void setPos(QPointF pos);
    void setId(int id);
    void setType(Map::PosType type);
    QPointF getPos();
    int getId();
    Map::PosType getType();

private:
    int m_id;
    QPointF m_pos;
    Map::PosType m_type;
};

#endif // NODE_H
