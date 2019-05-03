#include "mainwindow.h"
#include "ui_mainwindow.h"

void load_data(QString file_name, vector< vector<double> > *data) {
    FILE *fp_in;
    const int buffer = 1000000;
    char *ptr, basic[buffer];
    fp_in = fopen(file_name.toUtf8(), "r");
    uint i = 0, j = 0;
    while (fgets(basic, buffer, fp_in) != nullptr)
    {
        ptr = strtok(basic, "\t");
        j = 0;
        while (ptr != nullptr) {
            if (i == 0){
                vector<double> data_temp;
                data_temp.push_back(atof(ptr));
                data->push_back(data_temp);
            }
            else{
                data->at(j).push_back(atof(ptr));
            }
            ptr = strtok(nullptr, "\t");
            j++;
        }
        i = 1;
    }
    fclose(fp_in);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    load_data("../data/kinematics_result.txt", &data[0].d);
//    load_data("../data/hj_kinematics_result.txt", &data[1].d);

    load_data("../data/inverse_kinematics_result.txt", &data[0].d);
    load_data("../data/hj_inverse_kinematics_result.txt", &data[1].d);

    // result plot
    plotting(ui->plot1, data[0].d[1], data[0].d[2], data[1].d[0], data[1].d[1], "Time [sec]", "Angle [rad]", "recurdyn", "analysis");
    plotting(ui->plot2, data[0].d[1], data[0].d[3], data[1].d[0], data[1].d[2], "Time [sec]", "Angle [rad]", "recurdyn", "analysis");
    plotting(ui->plot3, data[0].d[1], data[0].d[4], data[1].d[0], data[1].d[3], "Time [sec]", "Angle [rad]", "recurdyn", "analysis", true);
    plotting(ui->plot4, data[0].d[1], data[0].d[5], data[1].d[0], data[1].d[4], "Time [sec]", "Position [meter]", "recurdyn", "analysis");
    plotting(ui->plot5, data[0].d[1], data[0].d[6], data[1].d[0], data[1].d[5], "Time [sec]", "Position [meter]", "recurdyn", "analysis");
    plotting(ui->plot6, data[0].d[1], data[0].d[7], data[1].d[0], data[1].d[6], "Time [sec]", "Position [meter]", "recurdyn", "analysis");

    // error plot
    if (data[0].d[0].size() == data[1].d[0].size()){
        vector<double> err[6];
        for(uint i = 0; i < 6; i++){
            for(uint j = 0; j < 2001; j++){
                err[i].push_back(data[0].d[i + 2][j] - data[1].d[i + 1][j]);
            }
        }
        plotting(ui->plot1_err, data[0].d[1], err[0], "Time [sec]", "Angle [rad]");
        plotting(ui->plot2_err, data[0].d[1], err[1], "Time [sec]", "Angle [rad]");
        plotting(ui->plot3_err, data[0].d[1], err[2], "Time [sec]", "Angle [rad]");
        plotting(ui->plot4_err, data[0].d[1], err[3], "Time [sec]", "Position [meter]");
        plotting(ui->plot5_err, data[0].d[1], err[4], "Time [sec]", "Position [meter]");
        plotting(ui->plot6_err, data[0].d[1], err[5], "Time [sec]", "Position [meter]");
    }
}

void MainWindow::plotting(QCustomPlot* plot, vector<double> x_vec, vector<double> y_vec, QString xlabel, QString ylabel)
{
    QVector<double> x, y;
    for (double vec : x_vec) x.push_back(vec);
    for (double vec : y_vec) y.push_back(vec);

    plot->addGraph();
    QPen pen0;
    pen0.setColor(QColor(255, 0, 0, 255));
    pen0.setWidth(2);
    plot->graph(0)->setPen(pen0);
    plot->graph(0)->setData(x,y);

    vector<double> x_temp, y_temp;
    x_temp = x.toStdVector();
    y_temp = y.toStdVector();
    sort(x_temp.begin(), x_temp.end());
    sort(y_temp.begin(), y_temp.end());

    plot->xAxis->setRange(x_temp.front(), x_temp.back());
    plot->yAxis->setRange(y_temp.front(), y_temp.back());

    plot->xAxis->setLabel(xlabel);
    plot->yAxis->setLabel(ylabel);

    plot->replot();

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::plotting(QCustomPlot* plot,
                          vector<double> x1_vec, vector<double> y1_vec,
                          vector<double> x2_vec, vector<double> y2_vec,
                          QString xlabel, QString ylabel,
                          QString name1, QString name2, bool legend_flag)
{
    plot->addGraph();
    QPen pen0, pen1;
    pen0.setColor(QColor(255, 0, 0, 255));
    pen0.setWidth(2);
    plot->graph(0)->setPen(pen0);
    plot->addGraph();
    pen1.setColor(QColor(0, 0, 255, 255));
    pen1.setWidthF(2);
    pen1.setStyle(Qt::DashLine);
    plot->graph(1)->setPen(pen1);

    QVector<double> x1, y1, x2, y2;
    for (double vec : x1_vec) x1.push_back(vec);
    for (double vec : y1_vec) y1.push_back(vec);
    for (double vec : x2_vec) x2.push_back(vec);
    for (double vec : y2_vec) y2.push_back(vec);

    plot->graph(0)->setData(x1,y1);
    plot->graph(1)->setData(x2,y2);
    plot->graph(0)->setName(name1);
    plot->graph(1)->setName(name2);

    vector<double> x_temp1, y_temp1;
    x_temp1 = x1.toStdVector();
    y_temp1 = y1.toStdVector();
    sort(y_temp1.begin(), y_temp1.end());

    vector<double> x_temp2, y_temp2;
    x_temp2 = x2.toStdVector();
    y_temp2 = y2.toStdVector();
    sort(y_temp2.begin(), y_temp2.end());

    double x_min, x_max, y_min, y_max;
    x_min = x_temp1.front() < x_temp2.front() ? x_temp1.front() : x_temp2.front();
    x_max = x_temp1.back() > x_temp2.back() ? x_temp1.back() : x_temp2.back();
    y_min = y_temp1.front() < y_temp2.front() ? y_temp1.front() : y_temp2.front();
    y_max = y_temp1.back() > y_temp2.back() ? y_temp1.back() : y_temp2.back();

    plot->xAxis->setRange(x_min, x_max);
    plot->yAxis->setRange(y_min, y_max);

    plot->xAxis->setLabel(xlabel);
    plot->yAxis->setLabel(ylabel);

    plot->replot();

    plot->legend->setVisible(legend_flag);
    plot->legend->setFont(QFont("Times", 9));
    plot->legend->setRowSpacing(-3);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

MainWindow::~MainWindow()
{
    delete ui;
}
