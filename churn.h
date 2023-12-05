#ifndef CHURN_H
#define CHURN_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QLine>

namespace Ui {
class Churn;
}

class Churn : public QWidget
{
    Q_OBJECT

public:
    explicit Churn(QWidget *parent = nullptr);
    ~Churn();

private:
    int rotations = 0;
    bool touchedBottom = false;
    float handleOffset = 0;

    Ui::Churn *ui;
    void mouseMoveEvent(QMouseEvent *eventMove);
    float calculateDistance(float x1, float y1, float x2, float y2);
    void paintEvent(QPaintEvent *event);

signals:
    void churningComplete();
};

#endif // CHURN_H
