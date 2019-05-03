#include "mainwindow.h"
#include <QApplication>
#include "robotarm.h"

int main(int argc, char *argv[])
{
    RobotArm robot(3,3);

//    robot.run_kinematics();
    robot.run_inverse_kinematics();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
