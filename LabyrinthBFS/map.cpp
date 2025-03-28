#include "map.h"


Map::Map() {}

void Map::buildFromFile(const std::string& filePath)
{
    int entrances = 0;
    std::ifstream file;
    try {
        file.open(filePath);
        int dim1 = 0, dim2 = 0;
        file>>dim1 >> dim2;
        m_matrix.resize(dim1);
        for(int i = 0; i < dim1; ++i) {
            m_matrix[i].resize(dim2);
            for(int j = 0 ;j < dim2 ; ++j) {
                int posValue;
                file>> posValue;
                if(posValue < 0 || posValue > 3) throw std::string("Invalid value!\n");
                m_matrix[i][j] = static_cast<Map::PosType>(posValue);
                if(posValue == 3) entrances++;
            }
        }
        file.close();
    }
    catch(const std::ifstream::failure& e) {
        qDebug() << "ERROR WHILE READING FILE\n";
        return;
    }
    catch(const std::string& e) {
        qDebug() << e;
        return;
    }
    if(entrances == 0 || entrances > 1) qDebug() << "INVALID ENTRANCES NUMBER\n";
}

void Map::reset()
{
    if(!m_matrix.empty()) {
        for(std::vector<PosType> curr : m_matrix) {
            if(!curr.empty()) curr.clear();
        }
        m_matrix.clear();
    }
}

std::vector<std::vector<Map::PosType>>& Map::getMatrix()
{
    return m_matrix;
}
