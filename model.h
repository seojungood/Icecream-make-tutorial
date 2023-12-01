#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMainWindow>
#include <Box2D/Box2D.h>
#include <QVector>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    b2World world;
    b2Body* body;
    QTimer *timer;
    QBrush bodyTexture;

    int numberBodies = {15};
    int screenIndex {0};

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

signals:
    void sendBodies(b2Body*);
    void setScreenToSwitch(int);

public slots:
    void updateWorldSlot();
    void incrementScreen();
    void decrementScreen();
};

#endif // MODEL_H
