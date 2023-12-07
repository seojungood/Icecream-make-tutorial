#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <cstdlib>

#include "churn.h"
#include "model.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->model = &model;

    // Set the background for Ingredient Screen
    QGraphicsScene* scene = new QGraphicsScene(0,0,800,600, ui->addIngredientsGraphicsView);
    scene->setBackgroundBrush(Qt::white);
    ui->addIngredientsGraphicsView->setScene(scene);
    ui->addIngredientsGraphicsView->setEnabled(false);

    // Set the background for End Screen
    QGraphicsScene* scene2 = new QGraphicsScene(0,0,800,600, ui->endScreenGraphicsView);
    scene2->setBackgroundBrush(Qt::white);
    ui->endScreenGraphicsView->setScene(scene2);
    ui->endScreenGraphicsView->setEnabled(false);

    // Connection to world simulation
    connect(&model, &Model::sendBodiesList, this, &MainWindow::updateRects);

    // Screen switching connections
    connect(ui->screen04Churn, &Churn::churningComplete, &model, &Model::incrementScreen); // Move from Churn to End Screen
    connect(ui->screen04Churn, &Churn::churningComplete, this, &MainWindow::on_change_to_End_Screen); // Shoot confetti

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
            graphicsRects[i]->setBrush(QBrush(Qt::black));
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
    ui->buttonCream->setIcon(QIcon(":/Resources/Sprites/cream.png"));
    ui->buttonMilk->setIcon(QIcon(":/Resources/Sprites/milk.png"));
    ui->buttonSugar->setIcon(QIcon(":/Resources/Sprites/sugar.png"));
    ui->buttonSalt->setIcon(QIcon(":/Resources/Sprites/salt.png"));
    ui->buttonVanilla->setIcon(QIcon(":/Resources/Sprites/vanilla.png"));
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
    rect->setPen(Qt::NoPen);
    rect->setBrush(model->bodyTexture);
    ui->addIngredientsGraphicsView->scene()->addItem(rect);
    graphicsRects.push_back(rect);

    //Add box into the world
    b2Body* body = model->world.CreateBody(&model->bodyDef); // Add body to world
    body->CreateFixture(&model->fixtureDef); // Add fixture to body
    model->bodies.push_back(body);
}

void MainWindow::addBodyToEndScreen(){
    // Create the rectangle in graphics view
    QGraphicsRectItem* rect = new QGraphicsRectItem(0,0, 100/8, 100/8);
    rect->setPen(Qt::NoPen);
    rect->setBrush(model->colors[std::rand()%6]);
    ui->endScreenGraphicsView->scene()->addItem(rect);
    graphicsRects.push_back(rect);

    // Recenter spawn point
    model->bodyDef.position.Set(0,0);

    // Resize boxes
    model->dynamicBox.SetAsBox(1.0f/8, 1.0f/8);

    //Add box into the world
    b2Body* body = model->world.CreateBody(&model->bodyDef); // Add body to world

    body->ApplyLinearImpulse( b2Vec2((std::rand()%15)-7,25), body->GetWorldCenter(), true );
    body->CreateFixture(&model->fixtureDef); // Add fixture to body
    model->bodies.push_back(body);
}

// Temporary. Used to test.
void MainWindow::on_pushButton_clicked()
{
    if (!model->cleanedWorld){
        // Clear bodies in world.
        for(auto body : model->bodies)
        {
            // Currenct body is the world ground, do not remove it.
            if(body->GetType() == b2_staticBody){
                continue;
            }
            model->world.DestroyBody(body);
        }
        //model->bodies.clear();
        model->cleanedWorld = true;
    }

    for(int i=0; i<250; ++i){
        addBodyToEndScreen();
    }
}

void MainWindow::on_change_to_End_Screen(){
    if (!model->cleanedWorld){
        // Clear bodies in world.
        for(auto body : model->bodies)
        {
            // Currenct body is the world ground, do not remove it.
            if(body->GetType() == b2_staticBody){
                continue;
            }
            model->world.DestroyBody(body);
        }
        //model->bodies.clear();
        model->cleanedWorld = true;
    }

    for(int i=0; i<250; ++i){
        addBodyToEndScreen();
    }
}

