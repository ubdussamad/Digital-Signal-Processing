#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph();
//    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
//    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->plot->yAxis->setLabel("Value");
    ui->plot->xAxis->setLabel("timeBase");
    ui->plot->xAxis->setRange(0, 100);
    ui->plot->yAxis->setRange(-50, 1);
    for ( this->counter = 0; this->counter < 100;  this->counter++ ) {
        this->qv_x.append(this->counter);
      }
    srand (time(NULL));
    this->graphUpdateTimer  = new QTimer(this);
    connect( graphUpdateTimer, SIGNAL(timeout()), this, SLOT(doTheDoogie()) );
    this->graphUpdateTimer->start(10);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPoint(double x,double y) {
//    LOG "Add point called!\n\n";
    this->qv_y.append_(y);
//    this->qv_x.append_(x);
//    LOG "Data appended.\n\n";
    if (this->graphMAX < y) {
        this->graphMAX = y;
        this->plotRangeChanged = true;
    }
    else { this->plotRangeChanged = false;}

    // On every chage , if the FIFO buffer is full just delete the last element add add new one.


}

void MainWindow::clrData(){}

void MainWindow::on_btnPlot_clicked()
{
//    LOG "button clicked!\n\n";
    addPoint( 0, rand() % 100 + 1  );
//    LOG "Add point exited!\n\n";
    ui->plot->graph(0)->setData(qv_x , qv_y);
    if (this->plotRangeChanged) {
        ui->plot->yAxis->setRange(-10, this->graphMAX + (0.8*this->graphMAX));
        this->plotRangeChanged = false;
    }
    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::doTheDoogie() {
//  LOG "button clicked!\n\n";
//  addPoint( 0, rand() % 100 + 1  );
//  LOG "Add point exited!\n\n";
//  ui->plot->graph(0)->setData(qv_x , qv_y);
//  if (this->plotRangeChanged) {
//      ui->plot->yAxis->setRange(-10, this->graphMAX + (0.2*this->graphMAX));
//      this->plotRangeChanged = false;
//  }
//  ui->plot->replot();
//  ui->plot->update();
  this->on_btnPlot_clicked();
}

void MainWindow::on_btnClr_clicked(){}
