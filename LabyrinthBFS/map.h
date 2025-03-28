#ifndef MAP_H
#define MAP_H
#pragma once
#include <vector>
#include <QDebug>

#include <fstream>

class Map
{
public:
    enum class PosType : uint8_t {
        Wall = 0,
        Path = 1,
        Exit = 2,
        Entrance = 3,
        Walked = 4,
        JustWalked = 5,
        JustExited = 6
    };


    Map();
    void buildFromFile(const std::string& filePath);
    void reset();
    std::vector<std::vector<PosType>>& getMatrix();


private:
    std::vector<std::vector<PosType>> m_matrix;
};

#endif // MAP_H
