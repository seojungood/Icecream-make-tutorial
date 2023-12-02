#include "churn.h"
#include "ui_churn.h"

Churn::Churn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Churn)
{
    ui->setupUi(this);

    QFont font( "Arial", 20, QFont::Bold);
    ui->rotationsLabel->setFont(font);

    ui->handle->setPixmap(QPixmap(":/Resources/Sprites/whiteCircle.png"));
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
    if ((distanceFromCenter < 160) && (distanceFromCenter > 120) && (distanceFromHandle < 30))
    {
        ui->handle->move(eventMove->position().x() - handleOffset, eventMove->position().y() - handleOffset);
        update();
    }

    if ((calculateDistance(ui->handle->x() + handleOffset, ui->handle->y() + handleOffset, 400, 165) < 30) && (touchedBottom))
    {
        rotations += 1;
        ui->rotationsLabel->setText(QString::number(rotations));
        touchedBottom = false;

        if (rotations >= 20)
        {
            emit churningComplete();
        }
    }

    if (calculateDistance(ui->handle->x() + handleOffset, ui->handle->y() + handleOffset, 400, 435) < 30)
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

    painter.drawImage(QRect(250, 150, 300, 300), QImage(":/Resources/Sprites/greenCircle.png"));
    painter.drawImage(QRect(280, 180, 240, 240), QImage(":/Resources/Sprites/whiteCircle.png"));

    painter.setPen(QPen(Qt::black, 16));
    painter.drawLine(400, 300, ui->handle->x() + handleOffset, ui->handle->y() + handleOffset);
}
