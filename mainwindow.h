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


public slots:
    void updateRectPos(int, int, int);

private slots:
    void on_buttonPrevious_clicked();
    void on_buttonNext_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
