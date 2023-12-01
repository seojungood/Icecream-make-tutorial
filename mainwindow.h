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

    std::vector<QGraphicsRectItem*> graphicsRects;


public slots:
    void updateRects(b2Body*);

private slots:

private:
    Ui::MainWindow *ui;
    void initializeImages();
    Model* model;

};
#endif // MAINWINDOW_H
