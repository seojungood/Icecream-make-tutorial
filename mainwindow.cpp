#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QStackedWidget>
#include "model.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene* scene = new QGraphicsScene(0,0,500,500, ui->graphicsView);
    scene->setBackgroundBrush(Qt::yellow);
    ui->graphicsView->setScene(scene);

    scene->addItem(rect);

    connect(&model, &Model::sendNewRecPos, this, &MainWindow::updateRectPos);

    connect(ui->buttonNext, &QPushButton::clicked, &model, &Model::incrementScreen);
    connect(ui->buttonPrevious, &QPushButton::clicked, &model, &Model::decrementScreen);
    connect(&model, &Model::setScreenToSwitch, ui->gameScreens, &QStackedWidget::setCurrentIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateRectPos(int x, int y, int angle){
    rect->setPos(x,y);
    //rect->setRotation(-(angle * 360.0) / (2 * 3.14159265));
    rect->setBrush(QBrush(Qt::black));
}


void MainWindow::on_buttonPrevious_clicked()
{

}


void MainWindow::on_buttonNext_clicked()
{

}

