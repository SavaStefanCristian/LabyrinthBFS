#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_showMap = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p{this};

    if(m_showMap == false) {
    p.setPen(QPen(Qt::white,1));

    std::vector<Node*>& nodes = m_graph.getNodes();
    for(auto& n : nodes)
    {
        p.setPen(QPen(Qt::white,1));
        if(n->getType()==Map::PosType::Path) p.setBrush(Qt::white);
        if(n->getType()==Map::PosType::Exit) p.setBrush(Qt::red);
        if(n->getType()==Map::PosType::Entrance) p.setBrush(Qt::cyan);
        if(n->getType() == Map::PosType::Walked) p.setBrush(Qt::green);
        QRectF r{n->getPos().x()-m_nodeRadius,n->getPos().y()- m_nodeRadius, m_nodeRadius*2, m_nodeRadius*2};
        p.drawEllipse(r);
        if(n->getType()==Map::PosType::Exit) p.setPen(QPen(Qt::white,1));
        else p.setPen(QPen(Qt::black,1));
        QString str = QString::number(n->getId()+1);
        p.drawText(r, Qt::AlignCenter, str);
    }
    p.setPen(QPen(Qt::white,1));
    std::unordered_map<Node*,std::vector<Node*>>& edgesList = m_graph.getEdges();
    for(const std::pair<Node*,std::vector<Node*>>& currNode : edgesList) {
        Node* node1 = currNode.first;
        for(Node* node2 : currNode.second) {
            int x1 = node1->getPos().x(), x2 = node2->getPos().x(), y1 = node1->getPos().y(), y2 = node2->getPos().y();
            double x1final, y1final, x2final, y2final;
            std::pair<double,double> results1 = calculateIntersection(x1,y1,x2,y2,m_nodeRadius);
            x1final = results1.first;
            y1final = results1.second;
            std::pair<double,double> results2 = calculateIntersection(x2,y2,x1,y1,m_nodeRadius);
            x2final = results2.first;
            y2final = results2.second;
            p.drawLine(x1final,y1final,x2final,y2final);
        }
    }
    }
    else {
        std::vector<std::vector<Map::PosType>> matrix = m_map.getMatrix();
        if(matrix.size() <= 0) return;
        if(matrix[0].size() <= 0) return;
        QRect screenRectangle = QApplication::primaryScreen()->geometry();
        QPoint screenCenter(screenRectangle.center());
        p.setPen(QPen(Qt::black,2));
        p.setBrush(Qt::white);
        QPointF currPoint = QPointF(screenCenter.x()-matrix[0].size()*(m_posWidth/2),screenCenter.y()-matrix.size()*(m_posHeight/2));
        for(int i = 0; i < matrix.size(); ++i) {
            for(int j = 0; j < matrix[0].size(); ++j) {
                if(matrix[i][j] == Map::PosType::Wall) p.setBrush(Qt::black);
                if(matrix[i][j] == Map::PosType::Path) p.setBrush(Qt::white);
                if(matrix[i][j] == Map::PosType::Exit) p.setBrush(Qt::red);
                if(matrix[i][j] == Map::PosType::Entrance) p.setBrush(Qt::cyan);
                if(matrix[i][j] == Map::PosType::Walked) p.setBrush(Qt::green);
                if(matrix[i][j] == Map::PosType::JustWalked) p.setBrush(Qt::yellow);
                if(matrix[i][j] == Map::PosType::JustExited) p.setBrush(Qt::magenta);
                p.drawRect(QRectF(currPoint,QPointF(currPoint.x() + m_posWidth, currPoint.y() + m_posHeight)));
                currPoint = QPointF(currPoint.x() + m_posWidth ,currPoint.y());
            }
            currPoint = QPointF(screenCenter.x()-matrix[0].size()*(m_posWidth/2), currPoint.y() + m_posHeight);
        }
    }
}

void MainWindow::placeNodes()
{
    std::vector<Node*>& nodes = m_graph.getNodes();
    if(nodes.size() < 1) return;
    QRect screenRectangle = QApplication::primaryScreen()->geometry();
    QPoint screenCenter(screenRectangle.center());
    double centerX = screenCenter.x(), centerY = screenCenter.y();
    double circlePerimeter = nodes.size()*m_nodeRadius*2*2;
    double circleRadius = circlePerimeter/(2*M_PI);
    double prevX = centerX-circleRadius;
    double prevY = centerY;
    nodes[0]->setPos(QPointF(prevX, prevY));
    double omega = 0, omega1 = 0;
    double angleIncrement = (2* M_PI) / nodes.size();

    for(int i = 1; i < nodes.size(); ++i) {
        omega = atan2(prevY - centerY, prevX - centerX);
        omega1 = omega + angleIncrement;
        prevX = centerX + circleRadius * cos(omega1);
        prevY = centerY + circleRadius * sin(omega1);
        nodes[i]->setPos(QPointF(prevX,prevY));
    }

}

std::pair<double, double> MainWindow::calculateIntersection(double x1, double y1, double x2, double y2, double radius)
{
    double x1final, y1final;
    if (x1 == x2) {
        double yv1 = y1 + radius;
        double yv2 = y1 - radius;

        if (std::abs(y2 - yv1) < std::abs(y2 - yv2)) {
            y1final = yv1;
        } else {
            y1final = yv2;
        }
        x1final = x1;
    }
    else {
        double m = double(y2 - y1) / double(x2 - x1);
        double b = y1 - m * x1;

        double A = 1 + m * m;
        double B = 2 * (m * (b - y1) - x1);
        double C = (x1 * x1) + (b - y1) * (b - y1) - radius * radius;

        double delta = B * B - 4 * A * C;

        if (delta < 0) {
            double yv1 = y1 + radius;
            double yv2 = y1 - radius;

            if (std::abs(y2 - yv1) < std::abs(y2 - yv2)) {
                y1final = yv1;
            } else {
                y1final = yv2;
            }
            x1final = x1;
            return std::pair<double,double>(x1final, y1final);
        }

        double sqrtDelta = std::sqrt(delta);
        double xv1 = (-B + sqrtDelta) / (2 * A);
        double xv2 = (-B - sqrtDelta) / (2 * A);

        double yv1 = m * xv1 + b;
        double yv2 = m * xv2 + b;

        double dist1 = std::pow(x2 - xv1, 2) + std::pow(y2 - yv1, 2);
        double dist2 = std::pow(x2 - xv2, 2) + std::pow(y2 - yv2, 2);

        if (dist1 < dist2) {
            x1final = xv1;
            y1final = yv1;
        } else {
            x1final = xv2;
            y1final = yv2;
        }
    }

    return std::pair<double,double>(x1final,y1final);
}





void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2) m_showMap = true;
    else m_showMap = false;
    update();
}


void MainWindow::on_pushButton_clicked()
{
    std::vector<std::list<Node*>> paths = m_graph.findPaths();
    std::vector<std::vector<Map::PosType>>& matrix = m_map.getMatrix();
    std::unordered_map<Node*,std::pair<int,int>> lookupCoordTable = m_graph.getLookupCoordTable();
    for(const std::list<Node*>& currPath : paths) {
        for(Node* currNode : currPath) {
            std::pair<int,int>& currCoord = lookupCoordTable[currNode];
            if(currNode->getType() != Map::PosType::Entrance) matrix[currCoord.first][currCoord.second] = Map::PosType::JustWalked;
            if(currNode == currPath.back()) matrix[currCoord.first][currCoord.second] = Map::PosType::JustExited;
            delay(150);
            update();
        }
        delay(500);
        for(Node* currNode : currPath) {
            std::pair<int,int>& currCoord = lookupCoordTable[currNode];
            matrix[currCoord.first][currCoord.second] = currNode->getType();
            delay(50);
            update();
        }
        update();
    }
    update();
}


void MainWindow::on_pushButton_2_clicked()
{
    m_graph.reset();
    m_map.reset();
    QString filePath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "\\..\\..\\..\\labyrinth.txt");
    m_map.buildFromFile(filePath.toStdString());
    m_graph.buildFromMap(m_map);
    placeNodes();
    update();
}






void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    m_nodeRadius = 30*(((double)position*2/3+33)/100);
    m_posWidth = 160*(((double)position*2/3+33)/100);
    m_posHeight = 120*(((double)position*2/3+33)/100);
    placeNodes();

    qDebug() << position;
    update();
}

void MainWindow::delay(int ms)
{
    QTime afterDelayTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < afterDelayTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

