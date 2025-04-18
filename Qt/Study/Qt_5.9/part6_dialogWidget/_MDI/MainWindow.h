﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFormDoc.h>
#include <QMdiArea>
#include  <QPainter>
#include  <QFileDialog>
#include <QCloseEvent>
#include <QDebug>
#include <MdiArea.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class Object :public QObject
//{
//    Q_OBJECT
//    bool eventFilter(QObject * watched, QEvent * event);
//};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actDoc_New_triggered();

    void on_actDoc_Open_triggered();

    void on_actViewMode_triggered(bool checked);

    void on_actCascade_triggered();

    void on_actTile_triggered();

    void on_actCloseALL_triggered();


private:
    Ui::MainWindow *ui;
    MdiArea * mdiArea;


};
#endif // MAINWINDOW_H
