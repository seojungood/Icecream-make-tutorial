#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMainWindow>
#include <Box2D/Box2D.h>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    b2World world;
    b2Body* body;
    QTimer *timer;

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

signals:
    void sendNewRecPos(int,int,int);

public slots:
    void updateWorldSlot();
};

#endif // MODEL_H
