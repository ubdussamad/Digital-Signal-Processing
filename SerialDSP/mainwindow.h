#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <time.h>
#include <iostream>

#define LOG std::cout <<

#define SHELL_BASH      1
#define DEBUG           1
#define ENABLE_WARNING  1
#define ENABLE_INFO     1
#define ENABLE_ERROR    1
#define ENABLE_SPECIAL  1

#include <SerialReader/SerialReader.hpp>
#include <fifo.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT  


public slots:
    void doTheDoogie();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



    void addPoint (double x , double y);
    void clrData ();


    fifoVector<double> qv_y;
    QVector<double> qv_x;
    double graphMAX;
    bool plotRangeChanged;
    QTimer *graphUpdateTimer;
    long int counter;
    serialReader* sr;
    bool init_flag;

private slots:
    void on_btnPlot_clicked();

    void on_btnClr_clicked();


private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
