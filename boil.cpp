#include "boil.h"
#include "ui_boil.h"

Boil::Boil(QWidget *parent)
    : QWidget(parent)
    , isBoiling(false)
    , boilingPosition(0)
    , ui(new Ui::Boil)
    , kitchenImage(":/Resources/Sprites/kitchen.jpg")
    , imageToDraw(":/Resources/Sprites/potOnStove1.png")
{
    ui->setupUi(this);

    boilButton = new QPushButton("Heat", this);
    boilButton->setGeometry(650, 485, 100, 50);
    boilButton->setStyleSheet(
        "background-color: #ff0000; color: #ffffff; border: none; border-radius: 10px;");

    connect(boilButton, &QPushButton::clicked, this, &Boil::onBoilButtonClicked);

    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, this, &Boil::updateBoilingBar);
}

Boil::~Boil()
{
    delete boilButton;
    delete timer;
    delete ui;
}

void Boil::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawImage(QRect(0, 0, width(), height()), kitchenImage);

    // Margin on both sides
    margin = 200;

    // Calculate the width of the bar without margins
    barWidth = width() - 2 * margin;

    painter.drawImage(QRect(225, 100, 350, 350), imageToDraw);

    // Draw the background bar
    painter.fillRect(margin, 500, barWidth, 20, Qt::white);
    painter.fillRect(margin + 0.6 * barWidth, 500, 0.3 * barWidth, 20, Qt::green);
    painter.fillRect(margin + 0.9 * barWidth, 500, 0.1 * barWidth, 20, Qt::red);

    // Draw the boiling bar with margin
    if (isBoiling) {
        boilingPosition = (boilingPosition + 1) % (barWidth - 15);
        painter.fillRect(margin + boilingPosition - 5, 500, 20, 20, Qt::black);
    }
}
void Boil::onBoilButtonClicked()
{
    if (!isBoiling) {
        isBoiling = true;
        timer->start(10); // Update bar every 10 milliseconds
    } else {
        // Check if the boiling bar reaches the green section
        if (boilingPosition + 200 >= margin + 0.6 * barWidth
            && boilingPosition + 200 <= margin + 0.9 * barWidth) {
            isBoiling = false;
            imageToDraw = QImage(":/Resources/Sprites/potOnStove2.png");
            repaint();
            QTimer::singleShot(2000, this, &Boil::boilGameClear);
            QTimer::singleShot(2000, this, &Boil::resetBoil);
        }
    }
}

void Boil::updateBoilingBar()
{
    if (isBoiling) {
        boilingPosition++;
        if (boilingPosition >= barWidth + margin) {
            boilingPosition = margin;
        }
        repaint();
    }
}

// Moves onto next screen
void Boil::boilGameClear()
{
    emit boilSucceeded();
}

void Boil::resetBoil()
{
    imageToDraw = QImage(":/Resources/Sprites/potOnStove1.png");
    margin = 200;
    barWidth = width() - 2 * margin;
}
