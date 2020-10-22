#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->yAxis->setLabel("Value");
    ui->plot->xAxis->setLabel("timeBase");
    ui->plot->xAxis->setRange(0, 300);
    ui->plot->yAxis->setRange(-5, 5);
    for ( this->counter = 0; this->counter < 300;  this->counter++ ) {
        this->qv_x.append(this->counter);
     }

    this->sr = new serialReader("/dev/ttyACM0", 00);
    if ( this->sr->setSerialProperties(115200,true,0,0) ) {
        this->init_flag = false;
      }
    else{
        this->init_flag  =true;
      }


    srand (time(NULL));
    this->graphUpdateTimer  = new QTimer(this);
    connect( graphUpdateTimer, SIGNAL(timeout()), this, SLOT(doTheDoogie()) );
    this->graphUpdateTimer->start(20);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPoint(double x,double y) {
  this->qv_y.append_(y);
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

  ui->plot->yAxis->setRange( ui->spinX->value(),ui->spinY->value());
}

void MainWindow::doTheDoogie() {
  char cxr[1024];
  this->sr->readSerial(cxr , 1024);
  addPoint( 0, atof(cxr) );
  ui->plot->graph(0)->setData(qv_x , qv_y);
  if (this->plotRangeChanged) {
//      ui->plot->yAxis->setRange(-5, this->graphMAX + (0.8*this->graphMAX));
      this->plotRangeChanged = false;
  }
  ui->plot->replot();
  ui->plot->update();
}

void MainWindow::on_btnClr_clicked(){}
