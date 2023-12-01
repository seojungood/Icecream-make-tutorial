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

    this->model = &model;

    // Set the background
    QGraphicsScene* scene = new QGraphicsScene(0,0,800,600, ui->graphicsView);
    scene->setBackgroundBrush(Qt::white);
    ui->graphicsView->setScene(scene);

    // Create the rectangles in graphics view
    for(int i = 0; i < model.numberBodies; ++i)
    {
        QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,100,100);
        rect->setFlag(QGraphicsItem::ItemIsMovable, true);
        rect->setPen(Qt::NoPen);
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

void MainWindow::updateRects(b2Body* bodies){
    int i = 0;
    while(bodies)
    {
        // Currenct body is the world ground, do not draw it.
        if(bodies->GetType() == b2_staticBody){
            bodies = bodies->GetNext();
            ++i;
            continue;
        }

        // Position and angle of the body in the world.
        b2Vec2 position = bodies->GetPosition();
        float32 angle = bodies->GetAngle();

        // Update cooresponding graphic boxes (position, rotation, texture).
        graphicsRects[i]->setPos(300+position.x*40, 600+(-position.y*60)); // Transform from world to Graphics view coordinates

        // Rotate about center
        QPointF center = QPointF(graphicsRects[i]->rect().center());
        QTransform t;
        t.translate(center.x(), center.y());
        t.rotate(-(angle * 360.0) / (2 * 3.14159265));
        t.translate(-center.x(), -center.y());
        graphicsRects[i]->setTransform(t);

        // Draw the texture onto the box in the graphics view.
        graphicsRects[i]->setBrush(model->bodyTexture);

        // Iterate to next body in world and graphics rect.
        bodies = bodies->GetNext();
        ++i;
    }
}

void MainWindow::initializeImages()
{
    ui->labelFrontPot->setPixmap(QPixmap(":/Resources/Sprites/spriteFrontPot.png"));
}


