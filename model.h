#ifndef MODEL_H
#define MODEL_H

#include <QMainWindow>
#include <QObject>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <vector>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    b2World world;
    std::vector<b2Body *> bodies;
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2PolygonShape dynamicBox;

    QTimer *timer;
    QTimer *screenSwitchDelayTimer;
    QBrush bodyTexture;

    std::vector<QColor> colors = {Qt::red, Qt::blue, Qt::yellow, Qt::green, Qt::magenta, Qt::cyan};
    bool cleanedWorld{false};
    int screenIndex{0};

    int ingredientsAdded{0};

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

signals:
    void sendBodies(b2Body *);
    void sendBodiesList(std::vector<b2Body *>);
    void setScreenToSwitch(int);
    void sendIngredientClicked(std::string);
    void resetAddIngredientsScreen();
    void openFridgeSignal();

public slots:
    void updateWorldSlot();
    void incrementScreen();
    void decrementScreen();
    void getIngredientClicked(std::string);
    void handleChillingComplete();
};

#endif // MODEL_H
