#include "model.h"
#include <QTimer>
#include <QDebug>

Model::Model(QObject *parent)
    : QObject{parent}, world(b2Vec2(0.0f, -10.0f))
{    
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -20.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the dynamic body. We set its position and call the body factory.
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = 1;
    bodyDef.position.Set((0), 10.0f);

    // Define another box shape for our dynamic body.
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    fixtureDef.shape = &dynamicBox;
    fixtureDef.restitution = 0.9;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    // Add boxes into the world
    for(int i = 0; i < numberBodies; i++){
        b2Body* body = world.CreateBody(&bodyDef); // Add body to world
        body->CreateFixture(&fixtureDef); // Add fixture to body

        bodies.push_back(body);
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Model::updateWorldSlot);
    timer->start(1000/60);
}

void Model::updateWorldSlot(){
    // Instruct the world to perform a single step of simulation.
    world.Step(timeStep, velocityIterations, positionIterations);

    //emit sendBodies(world.GetBodyList());
    emit sendBodiesList(bodies);
}

void Model::incrementScreen()
{
    screenIndex++;
    if (screenIndex > 5) {
        screenIndex = 0;
    }
    emit setScreenToSwitch(screenIndex);
}

void Model::decrementScreen()
{
    screenIndex--;
    if (screenIndex < 0) {
        screenIndex = 5;
    }
    emit setScreenToSwitch(screenIndex);
}

void Model::getIngredientClicked(std::string ingredient)
{
    ingredientsAdded++;
    if (ingredientsAdded == 5) {
        qDebug() << "All ingredients added";
        QTimer::singleShot(2000, this, &Model::incrementScreen);
    }
    emit sendIngredientClicked(ingredient);
}
