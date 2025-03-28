#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include "graph.h"

#include <QMainWindow>

#include <QPaintEvent>
#include <QPainter>
#include <QString>
#include <QDir>
#include <cmath>


#include "map.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




    void paintEvent(QPaintEvent* ev) override;


private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_sliderMoved(int position);

private:
    void delay(int ms);
    Ui::MainWindow *ui;

    Graph m_graph;
    Map m_map;

    double m_nodeRadius = 15;
    double m_posWidth = 80;
    double m_posHeight = 60;
    bool m_showMap;

    void placeNodes();
    std::pair<double, double> calculateIntersection(double x1, double y1, double x2, double y2, double radius);

};
#endif // MAINWINDOW_H
