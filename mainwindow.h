#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
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
    void updateRects(std::vector<b2Body*>);
    void updateIngredientButtonClicked(std::string);

private slots:
    void on_buttonCream_clicked();
    void on_buttonMilk_clicked();
    void on_buttonSugar_clicked();
    void on_buttonSalt_clicked();
    void on_buttonVanilla_clicked();
    void on_pushButton_clicked();

    void on_change_to_End_Screen();

    void on_exitGameButton_clicked();
    void on_chillMixtureButton_clicked();

private:
    Ui::MainWindow *ui;
    void resetAddIngredientsScreen();
    void initializeAddIngredientsScreen();
    void addBodyToWorld();
    void addBodyToEndScreen();
    Model* model;

signals:
    void onIngredientButtonClicked(std::string);
    void onChillingComplete();

};
#endif // MAINWINDOW_H
