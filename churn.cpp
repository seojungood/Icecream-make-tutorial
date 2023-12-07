#include "churn.h"
#include "ui_churn.h"

Churn::Churn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Churn)
{
    ui->setupUi(this);

    QFont font( "Arial", 20, QFont::Bold);
    ui->rotationsLabel->setFont(font);

    ui->handle->setPixmap(QPixmap(":/Resources/Sprites/handle.png"));
    handleOffset = float(ui->handle->width()) / 2.0f;

}

Churn::~Churn()
{
    delete ui;
}

void Churn::mouseMoveEvent(QMouseEvent *eventMove)
{
    float distanceFromCenter = calculateDistance(400.0f, 300.0f, float(eventMove->position().x()), float(eventMove->position().y()));
    float distanceFromHandle = calculateDistance(ui->handle->x() + handleOffset, ui->handle->y() + handleOffset, float(eventMove->position().x()), float(eventMove->position().y()));
    if ((distanceFromCenter < 170) && (distanceFromCenter > 110) && (distanceFromHandle < 50))
    {
        ui->handle->move(eventMove->position().x() - handleOffset, eventMove->position().y() - handleOffset);
        update();
    }

    if ((calculateDistance(ui->handle->x() + handleOffset, ui->handle->y() + handleOffset, 400, 165) < 40) && (touchedBottom))
    {
        rotations += 1;
        ui->rotationsLabel->setText(QString::number(rotations));
        touchedBottom = false;

        if (rotations >= 1) // Should be at least 5!!!
        {
            emit churningComplete();
        }
    }

    if (calculateDistance(ui->handle->x() + handleOffset, ui->handle->y() + handleOffset, 400, 435) < 40)
    {
        touchedBottom = true;
    }
}

float Churn::calculateDistance(float x1, float y1, float x2, float y2)
{
    int a = std::abs(x1 - x2);
    int b = std::abs(y1 - y2);

    return std::sqrt((a * a) + (b * b));
}

void Churn::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawImage(QRect(225, 245, 350, 350), QImage(":/Resources/Sprites/churner.png"));
    painter.drawImage(QRect(250, 150, 300, 300), QImage(":/Resources/Sprites/churnRing.png"));
//    painter.drawImage(QRect(280, 180, 240, 240), QImage(":/Resources/Sprites/whiteCircle.png"));

    painter.setPen(QPen(Qt::gray, 16));
    painter.drawLine(400, 300, ui->handle->x() + handleOffset, ui->handle->y() + handleOffset);

    painter.drawImage(QRect(385, 285, 30, 30), QImage(":/Resources/Sprites/bolt.png"));
}
