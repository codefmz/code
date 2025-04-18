﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimeLine>
#include <QTimer>

class QAction;
class QStandardItem;
class StandardItem;
class QModelIndex;
class QTreeView;
#ifdef CUSTOM_MODEL
class TreeModel;
#else
class StandardTreeModel;
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=0);
public slots:
    void stopTiming();
protected:
    void closeEvent(QCloseEvent*);
private slots:
    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void editAdd();
    void editDelete();
#ifdef CUSTOM_MODEL
    void editCut();
    void editPaste();
    void editMoveUp();
    void editMoveDown();
    void editPromote();
    void editDemote();
#endif
    void editStartOrStop(bool start);
    void editHideOrShowDoneTasks(bool hide);
    void setDirty(bool dirty=true) { setWindowModified(dirty); }
    void load(const QString &filename, const QStringList &taskPath=QStringList());
    void timeout();
    void updateIcon(int frame);
    void updateUi();

private:
    void createModelAndView();
    void createActions();
    void createMenusAndToolBar();
    void createConnections();
    bool okToClearData();
    void setCurrentIndex(const QModelIndex &index);
#ifdef CUSTOM_MODEL
    void hideOrShowDoneTask(bool hide, const QModelIndex &index);
#else
    void hideOrShowDoneTask(bool hide, QStandardItem *item);
#endif
private:
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileQuitAction;
    QAction *editAddAction;
    QAction *editDeleteAction;
#ifdef CUSTOM_MODEL
    QAction *editCutAction;
    QAction *editPasteAction;
    QAction *editMoveUpAction;
    QAction *editMoveDownAction;
    QAction *editPromoteAction;
    QAction *editDemoteAction;
#endif
    QAction *editStartOrStopAction;
    QAction *editHideOrShowDoneTasksAction;

    QTreeView *treeView;
#ifdef CUSTOM_MODEL
    TreeModel *model;
#else
    StandardTreeModel *model;
    StandardItem *timedItem;
#endif
    QTimer timer;
    QTimeLine iconTimeLine;
    QTime timedTime;
    int currentIcon;
};

#endif // MAINWINDOW_HPP

