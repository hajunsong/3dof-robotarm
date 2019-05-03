#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <iostream>
#include <vector>
using namespace std;

#include "qcustomplot.h"

class Data{
public:
    vector<vector< double> > d;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Data data[2];
    void plotting(QCustomPlot* plot, vector<double> x, vector<double> y, QString xlabel, QString ylabel);
    void plotting(QCustomPlot* plot, vector<double> x1, vector<double> y1, vector<double> x2, vector<double> y2, QString xlabel, QString ylabel, QString title1, QString title2, bool legend_flag=false);
};

#endif // MAINWINDOW_H
