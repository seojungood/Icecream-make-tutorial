#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateRectPos(int x, int y, int angle){
    rect->setPos(x,y);
    //rect->moveBy(x,y);
}

