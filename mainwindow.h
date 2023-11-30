#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsRectItem* rect = new QGraphicsRectItem(250,500,100,100);

    std::vector<QGraphicsRectItem*> graphicsRects;

    void drawGround();
    void drawBoxes();

public slots:
    void updateRectPos(int, int, int);
    void updateRects(b2Body*);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
