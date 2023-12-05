#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>

#include "model.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->model = &model;

    // Set the background
    QGraphicsScene* scene = new QGraphicsScene(0,0,800,600, ui->addIngredientsGraphicsView);
    scene->setBackgroundBrush(Qt::white);
    ui->addIngredientsGraphicsView->setScene(scene);
    ui->addIngredientsGraphicsView->setEnabled(false);


    // Delete Later.l;
    // Create the rectangles in graphics view
    for(int i = 0; i < model.numberBodies; ++i)
    {
        QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,100,100);
        rect->setFlag(QGraphicsItem::ItemIsMovable, true);
        rect->setPen(Qt::NoPen);
        graphicsRects.push_back(rect);
        scene->addItem(rect);
    }

    connect(&model, &Model::sendBodiesList, this, &MainWindow::updateRects);

    // Screen switching connections
    connect(ui->buttonNext, &QPushButton::clicked, &model, &Model::incrementScreen);
    connect(ui->buttonPrevious, &QPushButton::clicked, &model, &Model::decrementScreen);
    connect(&model, &Model::setScreenToSwitch, ui->gameScreens, &QStackedWidget::setCurrentIndex);

    // Add Ingredients connections
    connect(this, &MainWindow::onIngredientButtonClicked, &model, &Model::getIngredientClicked);
    connect(&model, &Model::sendIngredientClicked, this, &MainWindow::updateIngredientButtonClicked);

    initializeImages();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateRects(std::vector<b2Body*> bodies){

    for(int i=0; i<bodies.size(); ++i){
        // Currenct body is the world ground, do not draw it.
        if(bodies[i]->GetType() == b2_staticBody){
            continue;
        }

        // Position and angle of the body in the world.
        b2Vec2 position = bodies[i]->GetPosition();
        float32 angle = bodies[i]->GetAngle();

        // Update cooresponding graphic boxes (position, rotation, texture).
        graphicsRects[i]->setPos(300+position.x*40, 600+(-position.y*50)); // Transform from world to Graphics view coordinates

        // Rotate about center
        QPointF center = QPointF(graphicsRects[i]->rect().center());
        QTransform t;
        t.translate(center.x(), center.y());
        t.rotate(-(angle * 360.0) / (2 * 3.14159265));
        t.translate(-center.x(), -center.y());
        graphicsRects[i]->setTransform(t);
    }
}

void MainWindow::updateIngredientButtonClicked(std::string ingredient)
{
    if (ingredient == "cream") {
        ui->buttonCream->setVisible(false);
        ui->buttonCream->setEnabled(false);
    }
    else if (ingredient == "milk") {
        ui->buttonMilk->setVisible(false);
        ui->buttonMilk->setEnabled(false);
    }
    else if (ingredient == "salt") {
        ui->buttonSalt->setVisible(false);
        ui->buttonSalt->setEnabled(false);
    }
    else if (ingredient == "sugar") {
        ui->buttonSugar->setVisible(false);
        ui->buttonSugar->setEnabled(false);
    }
    else if (ingredient == "vanilla") {
        ui->buttonVanilla->setVisible(false);
        ui->buttonVanilla->setEnabled(false);
    }
}

void MainWindow::initializeImages()
{
    ui->labelFrontPot->setPixmap(QPixmap(":/Resources/Sprites/spriteFrontPot.png"));
}


void MainWindow::on_buttonCream_clicked()
{
    // Set texture
    model->bodyTexture.setTextureImage(QImage(":/Resources/Sprites/cream.png").scaled(100,100));

    addBodyToWorld();
    emit onIngredientButtonClicked("cream");
}


void MainWindow::on_buttonMilk_clicked()
{
    // Set texture
    model->bodyTexture.setTextureImage(QImage(":/Resources/Sprites/milk.png").scaled(100,100));

    addBodyToWorld();
    emit onIngredientButtonClicked("milk");
}


void MainWindow::on_buttonSugar_clicked()
{
    // Set texture
    model->bodyTexture.setTextureImage(QImage(":/Resources/Sprites/sugar.png").scaled(100,100));

    addBodyToWorld();
    emit onIngredientButtonClicked("sugar");
}


void MainWindow::on_buttonSalt_clicked()
{
    // Set texture
    model->bodyTexture.setTextureImage(QImage(":/Resources/Sprites/salt.png").scaled(100,100));

    addBodyToWorld();
    emit onIngredientButtonClicked("salt");
}


void MainWindow::on_buttonVanilla_clicked()
{
    // Set texture
    model->bodyTexture.setTextureImage(QImage(":/Resources/Sprites/vanilla.png").scaled(100,100));

    addBodyToWorld();
    emit onIngredientButtonClicked("vanilla");
}

void MainWindow::addBodyToWorld(){
    // Create the rectangle in graphics view
    QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,100,100);
    rect->setFlag(QGraphicsItem::ItemIsMovable, true);
    rect->setPen(Qt::NoPen);
    rect->setBrush(model->bodyTexture);
    graphicsRects.push_back(rect);
    ui->addIngredientsGraphicsView->scene()->addItem(rect);

    //Add box into the world
    b2Body* body = model->world.CreateBody(&model->bodyDef); // Add body to world
    body->CreateFixture(&model->fixtureDef); // Add fixture to body
    model->bodies.push_back(body);
}
