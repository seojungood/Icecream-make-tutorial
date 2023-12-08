#ifndef BOIL_H
#define BOIL_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QImage>

namespace Ui {
class Boil;
}

class Boil : public QWidget {
    Q_OBJECT
public:
    explicit Boil(QWidget *parent = nullptr);
    ~Boil();

public slots:
    void onBoilButtonClicked();
    void boilGameClear();
    void updateBoilingBar();
    void resetBoil();

signals:
    void boilSucceeded();

private:
    QPushButton *boilButton;
    QTimer *timer;
    Ui::Boil *ui;
    QImage imageToDraw;

    void paintEvent(QPaintEvent *event);
    bool isBoiling;
    int boilingPosition;
    int margin;
    int barWidth;

};

#endif // BOIL_H
