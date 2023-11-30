#include "model.h"
#include <QTimer>
#include <QDebug>

Model::Model(QObject *parent)
    : QObject{parent}, world(b2Vec2(0.0f, -10.0f))
{    
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

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
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    //bodyDef.position.Set(0.0f, 4.0f);
    //body = world.CreateBody(&bodyDef); // Add a body to the world

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box restitution (bounciness)
    fixtureDef.restitution = 0.9;
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 0.3f;

    // Add the shape to the body.
    //body->CreateFixture(&fixtureDef); // Set properties of the body

    // Add boxes into the world
    for(int i = 0; i < numberBodies; i++){
        bodyDef.position.Set((0), 10.0f);
        body = world.CreateBody(&bodyDef); // Add body to world
        body->CreateFixture(&fixtureDef); // Add fixture to body
    }


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Model::updateWorldSlot);
    timer->start(1000/60);

}

void Model::updateWorldSlot(){
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world.Step(timeStep, velocityIterations, positionIterations);


    // Now print the position and angle of the body.
    b2Vec2 position = body->GetPosition();
    float32 angle = body->GetAngle();

    //printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

    emit sendBodies(world.GetBodyList());
    //emit sendNewRecPos(position.x*100, -position.y*100, angle*100);
}

void Model::incrementScreen()
{
    screenIndex++;
    if (screenIndex > 5) {
        screenIndex = 0;
    }
    qDebug() << "INCREMENT";
    emit setScreenToSwitch(screenIndex);
}

void Model::decrementScreen()
{
    screenIndex--;
    if (screenIndex < 0) {
        screenIndex = 5;
    }
    qDebug() << "DECREMENT";
    emit setScreenToSwitch(screenIndex);
}
