#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QPushButton>
#include <QStackedWidget>
#include "model.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the background
    QGraphicsScene* scene = new QGraphicsScene(0,0,800,600, ui->graphicsView);
    scene->setBackgroundBrush(Qt::black);
    ui->graphicsView->setScene(scene);

    for(int i = 0; i < model.numberBodies; ++i)
    {
        QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,100,100);
        graphicsRects.push_back(rect);
        scene->addItem(rect);
    }

    connect(&model, &Model::sendBodies, this, &MainWindow::updateRects);
    // Screen switching connections
    connect(ui->buttonNext, &QPushButton::clicked, &model, &Model::incrementScreen);
    connect(ui->buttonPrevious, &QPushButton::clicked, &model, &Model::decrementScreen);
    connect(&model, &Model::setScreenToSwitch, ui->gameScreens, &QStackedWidget::setCurrentIndex);

    initializeImages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawGround(){
    // Draw Ground
    graphicsRects[0]->setPos(0, 0);
    graphicsRects[0]->setBrush(QBrush(Qt::white));
}

void MainWindow::drawBoxes(){

}

void MainWindow::updateRectPos(int x, int y, int angle){
    rect->setPos(x,y);
    //rect->setRotation(-(angle * 360.0) / (2 * 3.14159265));
    rect->setBrush(QBrush(Qt::black));
}

void MainWindow::updateRects(b2Body* bodies){
    int i = 0;

    //qDebug()<<"looping";
    while(bodies){
        // Currenct body is the world ground
        if(bodies->GetType() == b2_staticBody){
            bodies = bodies->GetNext();
            ++i;
            continue;
        }

        // Position and angle of the body.
        b2Vec2 position = bodies->GetPosition();
        float32 angle = bodies->GetAngle();

        // Update cooresponding graphic boxes

        QBrush q;
        //q.setTextureImage(QImage("C:Users\andre\Downloads\pixel-strawberry.jpg").scaled(100,100));

        graphicsRects[i]->setPos(300+position.x*40, 600+(-position.y*60)); //Offset and scale to transform from world to Graphics view
        graphicsRects[i]->setBrush(QBrush(Qt::white));
        //graphicsRects[i]->setBrush(q);

        // Rotate about center
        QPointF center = QPointF(graphicsRects[i]->rect().center());
        QTransform t;
        t.translate(center.x(), center.y());
        t.rotate(-(angle * 360.0) / (2 * 3.14159265));
        t.translate(-center.x(), -center.y());
        graphicsRects[i]->setTransform(t);

        // Iterate to next body in world and graphics rect.
        bodies = bodies->GetNext();
        ++i;
    }
}

void MainWindow::initializeImages()
{
    ui->labelFrontPot->setPixmap(QPixmap(":/Resources/Sprites/spriteFrontPot.png"));
}


