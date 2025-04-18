﻿#include "MainWindow.h"
#include "AQP.h"
#include "Base.h"
#include "RichTextDelegate.h"
#ifdef CUSTOM_MODEL
#include "TreeModel.h"
#else
#include "StandardItem.h"
#include "StandardTreeModel.h"
#endif
#include <QAbstractButton>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHash>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QStandardItem>
#include <QTimer>
#include <QToolBar>
#include <QTreeView>
#include <QDebug>

const int StatusTimeout = AQP::MSecPerSecond * 10;
const QString FilenameSetting("Filename");
const QString GeometrySetting("Geometry");
const QString CurrentTaskPathSetting("CurrentTaskPath");
const int FirstFrame = 0;
const int LastFrame = 4;

namespace {
    QAction *createAction(const QString &icon, const QString &text, QObject *parent,
        const QKeySequence &shortcut = QKeySequence())
    {
        QAction *action = new QAction(QIcon(icon), text, parent);
        if (!shortcut.isEmpty()) {
            action->setShortcut(shortcut);
        }
        return action;
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
#ifndef CUSTOM_MODEL
    timedItem(nullptr),
#endif
currentIcon(0)
{
    createModelAndView();
    createActions();
    createMenusAndToolBar();
    createConnections();
//    AQP::accelerateMenu(menuBar());
#ifdef CUSTOM_MODEL
    setWindowTitle(tr("%1 (Custom Model)[*]").arg(QApplication::applicationName()));
#else
    setWindowTitle(tr("%1 (QStandardItemModel)[*]").arg(QApplication::applicationName()));
#endif
    statusBar()->showMessage(tr("Ready"), StatusTimeout);

    timer.setInterval(333);
    iconTimeLine.setDuration(5000);
    iconTimeLine.setFrameRange(FirstFrame, LastFrame + 1);
    iconTimeLine.setLoopCount(0);
    iconTimeLine.setCurveShape(QTimeLine::LinearCurve);
    QSettings settings("config.ini", QSettings::IniFormat);
    restoreGeometry(settings.value(GeometrySetting).toByteArray());
    QString filename = settings.value(FilenameSetting).toString();
    if (filename.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(fileNew()));
    } else {
        QMetaObject::invokeMethod(this, "load", Qt::QueuedConnection, Q_ARG(QString, filename),
            Q_ARG(QStringList, settings.value(CurrentTaskPathSetting).toStringList()));
    }
}

void MainWindow::createModelAndView()
{
    treeView = new QTreeView(this);
#ifdef CUSTOM_MODEL
    model = new TreeModel(this);
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
#else
    model = new StandardTreeModel(this);
#endif
    treeView->setAllColumnsShowFocus(true);
    treeView->setItemDelegateForColumn(0, new RichTextDelegate);
    treeView->setModel(model);
    setCentralWidget(treeView);
}

void MainWindow::createActions()
{
    fileNewAction = createAction(":/images/filenew.png", tr("New"), this, QKeySequence::New);
    fileOpenAction = createAction(":/images/fileopen.png", tr("Open..."), this, QKeySequence::Open);
    fileSaveAction = createAction(":/images/filesave.png", tr("Save"),this, QKeySequence::Save);
    fileSaveAsAction = createAction(":/images/filesave.png", tr("Save As..."), this, QKeySequence::SaveAs);
    fileQuitAction = createAction(":/images/filequit.png", tr("Quit"), this);
    fileQuitAction->setShortcuts(QKeySequence::Quit);
    editAddAction = createAction(":/images/editadd.png", tr("Add..."), this, QKeySequence(tr("Ctrl+A")));
    editDeleteAction = createAction(":/images/editdelete.png", tr("Delete..."), this, QKeySequence::Delete);

#ifdef CUSTOM_MODEL
    editCutAction = createAction(":/images/editcut.png", tr("Cut"), this, QKeySequence::Cut);
    editPasteAction = createAction(":/images/editpaste.png", tr("Paste"), this, QKeySequence::Paste);
    editMoveUpAction = createAction(":/images/editup.png", tr("Move Up"), this, QKeySequence(tr("Ctrl+Up")));
    editMoveDownAction = createAction(":/images/editdown.png", tr("Move Down"), this, QKeySequence(tr("Ctrl+Down")));
    editPromoteAction = createAction(":/images/editpromote.png", tr("Promote"), this, QKeySequence(tr("Ctrl+Left")));
    editDemoteAction = createAction(":/images/editdemote.png", tr("Demote"), this, QKeySequence(tr("Ctrl+Right")));
#endif
    editStartOrStopAction = createAction(":/images/0.png", tr("S&tart"), this, QKeySequence(tr("Ctrl+T")));
    editStartOrStopAction->setCheckable(true);
    editStartOrStopAction->setChecked(false);
    editHideOrShowDoneTasksAction = new QAction(tr("Hide Done Tasks"), this);
    editHideOrShowDoneTasksAction->setCheckable(true);
    editHideOrShowDoneTasksAction->setChecked(false);
}

void MainWindow::createMenusAndToolBar()
{
    setUnifiedTitleAndToolBarOnMac(true);

    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    QList<QAction*> actList;
    actList << fileNewAction << fileOpenAction << fileSaveAction << fileSaveAsAction;
    foreach (QAction *action, actList) {
        fileMenu->addAction(action);
        if (action != fileSaveAsAction) {
            fileToolBar->addAction(action);
        }
        if (action == fileSaveAction || action == fileSaveAsAction) {
            action->setEnabled(false);
        }
    }
    fileMenu->addSeparator();
    fileMenu->addAction(fileQuitAction);

    QAction *emptyAction = 0;
    QMenu *editMenu = menuBar()->addMenu(tr("Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    QList<QAction*> actList2;
#ifdef CUSTOM_MODEL
    actList2 << editAddAction << editDeleteAction << emptyAction << editCutAction << editPasteAction << emptyAction
        << editMoveUpAction << editMoveDownAction << editPromoteAction << editDemoteAction << emptyAction
        << editStartOrStopAction << editHideOrShowDoneTasksAction;
#else
    actList2 << editAddAction << editDeleteAction << emptyAction << editStartOrStopAction << editHideOrShowDoneTasksAction;

#endif
    foreach (QAction *action, actList2) {
        if (action == emptyAction) {
            editMenu->addSeparator();
            editToolBar->addSeparator();
            continue;
        }
        if (action != editHideOrShowDoneTasksAction) {
            editToolBar->addAction(action);
        } else {
            editMenu->addSeparator();
            editMenu->addAction(action);
        }
    }
}

void MainWindow::createConnections()
{
    connect(treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(updateUi()));
#ifdef CUSTOM_MODEL
    connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(setDirty()));
    connect(model, SIGNAL(stopTiming()), this, SLOT(stopTiming()));
#else
    connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(setDirty()));
#endif // CUSTOM_MODEL
    connect(model, SIGNAL(rowsRemoved(const QModelIndex&,int,int)), this, SLOT(setDirty()));
    connect(model, SIGNAL(modelReset()), this, SLOT(setDirty()));

    QHash<QAction*, QString> slotForAction;
    slotForAction[fileNewAction] = SLOT(fileNew());
    slotForAction[fileOpenAction] = SLOT(fileOpen());
    slotForAction[fileSaveAction] = SLOT(fileSave());
    slotForAction[fileSaveAsAction] = SLOT(fileSaveAs());
    slotForAction[fileQuitAction] = SLOT(close());
    slotForAction[editAddAction] = SLOT(editAdd());
    slotForAction[editDeleteAction] = SLOT(editDelete());
#ifdef CUSTOM_MODEL
    slotForAction[editCutAction] = SLOT(editCut());
    slotForAction[editPasteAction] = SLOT(editPaste());
    slotForAction[editMoveUpAction] = SLOT(editMoveUp());
    slotForAction[editMoveDownAction] = SLOT(editMoveDown());
    slotForAction[editPromoteAction] = SLOT(editPromote());
    slotForAction[editDemoteAction] = SLOT(editDemote());
#endif
    QHashIterator<QAction*, QString> i(slotForAction);
    while (i.hasNext()) {
        i.next();
        connect(i.key(), SIGNAL(triggered()), this, qPrintable(i.value()));
    }

    connect(editStartOrStopAction, SIGNAL(triggered(bool)), this, SLOT(editStartOrStop(bool)));
    connect(editHideOrShowDoneTasksAction, SIGNAL(triggered(bool)), this, SLOT(editHideOrShowDoneTasks(bool)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(&iconTimeLine, SIGNAL(frameChanged(int)), this, SLOT(updateIcon(int)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    stopTiming();
    if (okToClearData()) {
        QSettings settings("config.ini", QSettings::IniFormat);
        settings.setValue(GeometrySetting, saveGeometry());
        settings.setValue(FilenameSetting, model->filename());
        settings.setValue(CurrentTaskPathSetting, model->pathForIndex(treeView->currentIndex()));
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::okToClearData()
{
    if (isWindowModified()) {
        return AQP::okToClearData(&MainWindow::fileSave, this, tr("Unsaved changes"), tr("Save unsaved changes?"));
    }
    return true;
}

void MainWindow::fileNew()
{
    if (!okToClearData()) {
        return;
    }
    model->clear();
    model->setFilename(QString());
    setDirty(false);
    setWindowTitle(tr("%1 - Unnamed[*]").arg(QApplication::applicationName()));
    updateUi();
}

void MainWindow::updateUi()
{
    fileSaveAction->setEnabled(isWindowModified());
    int rows = model->rowCount();
    fileSaveAsAction->setEnabled(isWindowModified() || rows);
    editHideOrShowDoneTasksAction->setEnabled(rows);
    bool enable = treeView->currentIndex().isValid();
    QList<QAction *> actList;
#ifdef CUSTOM_MODEL
    actList << editDeleteAction << editMoveUpAction << editMoveDownAction << editCutAction
            << editPromoteAction << editDemoteAction;
#else
    actList << editDeleteAction << editStartOrStopAction;
#endif
    foreach (QAction *action, actList) {
        action->setEnabled(enable);
#ifdef CUSTOM_MODEL
        editStartOrStopAction->setEnabled(rows);
        editPasteAction->setEnabled(model->hasCutItem());
#endif
    }
}

void MainWindow::fileOpen()
{
    if (!okToClearData()) {
        return;
    }
    QString filename = model->filename();
    QString dir(filename.isEmpty() ? QString(".") : QFileInfo(filename).canonicalPath());
    filename = QFileDialog::getOpenFileName(this, tr("%1 - Open").arg(QApplication::applicationName()), dir, tr("Timelogs (*.tlg)"));
    if (!filename.isEmpty()) {
        load(filename);
    }
}

void MainWindow::load(const QString &filename, const QStringList &taskPath)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    try {
        model->load(filename);
        if (!taskPath.isEmpty()) {
#ifdef CUSTOM_MODEL
            setCurrentIndex(model->indexForPath(taskPath));
#else
            QStandardItem *item = model->itemForPath(taskPath);
            if (item) {
                setCurrentIndex(item->index());
            }
#endif
        }
        for (int column = 0; column < model->columnCount(); ++column) {
            treeView->resizeColumnToContents(column);
        }
        setDirty(false);
        setWindowTitle(tr("%1 - %2[*]").arg(QApplication::applicationName()).arg(QFileInfo(filename).fileName()));
        statusBar()->showMessage(tr("Loaded %1").arg(filename), StatusTimeout);
    } catch (AQP::Error &error) {
        AQP::warning(this, tr("Error"), tr("Failed to load %1: %2").arg(filename).arg(QString::fromUtf8(error.what())));
    }
    updateUi();
    editHideOrShowDoneTasks(editHideOrShowDoneTasksAction->isChecked());
    treeView->setFocus();
    QApplication::restoreOverrideCursor();
}

void MainWindow::setCurrentIndex(const QModelIndex &index)
{
    if (index.isValid()) {
        treeView->scrollTo(index);
        treeView->setCurrentIndex(index);
    }
}

bool MainWindow::fileSave()
{
    bool saved = false;
    if (model->filename().isEmpty()) {
        saved = fileSaveAs();
    } else {
        try {
            model->save();
            setDirty(false);
            setWindowTitle(tr("%1 - %2[*]").arg(QApplication::applicationName()).arg(QFileInfo(model->filename()).fileName()));
            statusBar()->showMessage(tr("Saved %1").arg(model->filename()), StatusTimeout);
            saved = true;
        } catch (AQP::Error &error) {
            AQP::warning(this, tr("Error"), tr("Failed to save %1: %2").arg(model->filename()).arg(QString::fromUtf8(error.what())));
        }
    }
    updateUi();
    return saved;
}

bool MainWindow::fileSaveAs()
{
    QString filename = model->filename();
    QString dir = filename.isEmpty() ? "." : QFileInfo(filename).path();
    filename = QFileDialog::getSaveFileName(this, tr("%1 - Save As").arg(QApplication::applicationName()), dir,
        tr("%1 (*.tlg)").arg(QApplication::applicationName()));
    if (filename.isEmpty()) {
        return false;
    }
    if (!filename.toLower().endsWith(".tlg")) {
        filename += ".tlg";
    }
    model->setFilename(filename);
    return fileSave();
}


void MainWindow::editAdd()
{
#ifndef CUSTOM_MODEL
    QModelIndex index = treeView->currentIndex();
    StandardTreeModel::Insert insert = StandardTreeModel::AtTopLevel;

    if (index.isValid()) {
        QStandardItem *item = model->itemFromIndex(index);
        std::unique_ptr<QMessageBox> messageBox = std::make_unique<QMessageBox>(this);
        messageBox->setWindowModality(Qt::WindowModal);
        messageBox->setIcon(QMessageBox::Question);
        messageBox->setWindowTitle(tr("%1 - Add Task").arg(QApplication::applicationName()));
        messageBox->setText(tr("<p>Add at the top level or as a sibling or child of\n'%1'?").arg(item->text()));
        messageBox->addButton(tr("&Top Level"), QMessageBox::AcceptRole);
        QAbstractButton *siblingButton = messageBox->addButton(tr("&Sibling"), QMessageBox::AcceptRole);
        QAbstractButton *childButton = messageBox->addButton(tr("C&hild"), QMessageBox::AcceptRole);
        messageBox->setDefaultButton(qobject_cast<QPushButton*>(childButton));
        messageBox->addButton(QMessageBox::Cancel);
        messageBox->exec();
        if (messageBox->clickedButton() == messageBox->button(QMessageBox::Cancel)) {
            return;
        }
        if (messageBox->clickedButton() == childButton) {
            insert = StandardTreeModel::AsChild;
        } else if (messageBox->clickedButton() == siblingButton) {
            insert = StandardTreeModel::AsSibling;
        }
    }

    QStandardItem *item = model->insertNewTask(insert, tr("New Task"), index);
    if (item) {
        QModelIndex index = item->index();
        setCurrentIndex(index);
        treeView->edit(index);
        setDirty();
        updateUi();
    }
#else
    QModelIndex index = treeView->currentIndex();
    if (model->insertRow(0, index)) {
        index = model->index(0, 0, index);
        setCurrentIndex(index);
        treeView->edit(index);
        setDirty();
        updateUi();
    }
#endif
}

void MainWindow::editDelete()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid()) {
        return;
    }
#ifdef CUSTOM_MODEL
    QString name = model->data(index).toString();
    int rows = model->rowCount(index);
    if (model->isTimedItem(index)) {
        stopTiming();
    }
#else
    QStandardItem *item = model->itemFromIndex(index);
    if (item == timedItem) {
        stopTiming();
    }
    QString name = item->text();
    int rows = item->rowCount();
#endif
    QString message;
    if (rows == 0) {
        message = tr("<p>Delete '%1'").arg(name);
    } else if (rows == 1) {
        message = tr("<p>Delete '%1' and its child (and grandchildren etc.)").arg(name);
    } else if (rows > 1) {
        message = tr("<p>Delete '%1' and its %2 children (and grandchildren etc.)").arg(name).arg(rows);
    }

    if (!AQP::okToDelete(this, tr("Delete"), message)) {
        return;
    }
    model->removeRow(index.row(), index.parent());
    setDirty();
    updateUi();
}

void MainWindow::stopTiming()
{
    if (editStartOrStopAction->isChecked()) {
        editStartOrStopAction->trigger(); // stop the clock
    }
}

#ifdef CUSTOM_MODEL
void MainWindow::editCut()
{
    QModelIndex index = treeView->currentIndex();
    if (model->isTimedItem(index)) {
        stopTiming();
    }
    setCurrentIndex(model->cut(index));
    editPasteAction->setEnabled(model->hasCutItem());
}

void MainWindow::editPaste()
{
    setCurrentIndex(model->paste(treeView->currentIndex()));
    editHideOrShowDoneTasks(editHideOrShowDoneTasksAction->isChecked());
}

void MainWindow::editMoveUp()
{
    treeView->setCurrentIndex(model->moveUp(treeView->currentIndex()));
    editHideOrShowDoneTasks(editHideOrShowDoneTasksAction->isChecked());
}

void MainWindow::editMoveDown()
{
    treeView->setCurrentIndex(model->moveDown(treeView->currentIndex()));
    editHideOrShowDoneTasks(editHideOrShowDoneTasksAction->isChecked());
}

void MainWindow::editPromote()
{
    QModelIndex index = treeView->currentIndex();
    if (model->isTimedItem(index)) {
        stopTiming();
    }
    setCurrentIndex(model->promote(index));
    editHideOrShowDoneTasks(editHideOrShowDoneTasksAction->isChecked());
}

void MainWindow::editDemote()
{
    QModelIndex index = treeView->currentIndex();
    if (model->isTimedItem(index)) {
        stopTiming();
    }
    treeView->setCurrentIndex(model->demote(index));
    editHideOrShowDoneTasks(editHideOrShowDoneTasksAction->isChecked());
}
#endif // CUSTOM_MODEL

void MainWindow::editStartOrStop(bool start)
{
    timer.stop();
    iconTimeLine.stop();
    if (start) { // start the clock if there's a current task
        QModelIndex index = treeView->currentIndex();
        if (!index.isValid()) {
            editStartOrStopAction->setChecked(false);
            start = false;
        } else {
            QIcon icon(":/images/0.png");
            QDateTime now = QDateTime::currentDateTime();
#ifdef CUSTOM_MODEL
            model->setTimedItem(index);
            model->addDateTimeToTimedItem(now, now);
            model->setIconForTimedItem(icon);
#else
            timedItem = static_cast<StandardItem*>(model->itemFromIndex(index));
            Q_ASSERT(timedItem);
            timedItem->addDateTime(now, now);
            timedItem->todayItem()->setIcon(icon);
#endif
            editStartOrStopAction->setIcon(icon);
            setWindowIcon(icon);
            timedTime.restart();
            timer.start();
            iconTimeLine.start();
        }
    } else { // stop the clock
        timeout(); // update to now
#ifdef CUSTOM_MODEL
        model->setIconForTimedItem();
        model->clearTimedItem();
#else
        if (timedItem) {
            timedItem->todayItem()->setIcon(QIcon());
            timedItem = nullptr;
        }
#endif
        setWindowIcon(QIcon(":/images/icon.png"));
        editStartOrStopAction->setIcon(QIcon(":/images/0.png"));
    }
    editStartOrStopAction->setText(start ? tr("S&top") : tr("S&tart"));
    editStartOrStopAction->setIcon(QIcon(start ? tr(":/images/4.png") : tr(":/images/0.png")));
}


void MainWindow::timeout()
{
#ifdef CUSTOM_MODEL
    model->incrementEndTimeForTimedItem(timedTime.elapsed());
    timedTime.restart();
#else
    Q_ASSERT(timedItem);
    timedItem->incrementLastEndTime(timedTime.elapsed());
    timedTime.restart();
    StandardItem *item = timedItem;
    while (item) {
        item->todayItem()->setText(item->todaysTime());
        item->totalItem()->setText(item->totalTime());
        item = static_cast<StandardItem*>(item->parent());
    }
#endif
}

void MainWindow::updateIcon(int frame)
{
    if (frame > LastFrame) {
        return;
    }
    QIcon icon(QString(":/images/%1.png").arg(frame));
#ifdef CUSTOM_MODEL
    model->setIconForTimedItem(icon);
#else
    Q_ASSERT(timedItem);
    timedItem->todayItem()->setIcon(icon);
#endif
    editStartOrStopAction->setIcon(icon);
    setWindowIcon(icon);
}

void MainWindow::editHideOrShowDoneTasks(bool hide)
{
#ifdef CUSTOM_MODEL
    hideOrShowDoneTask(hide, QModelIndex());
#else
    hideOrShowDoneTask(hide, model->invisibleRootItem());
#endif
}

#ifndef CUSTOM_MODEL
void MainWindow::hideOrShowDoneTask(bool hide, QStandardItem *item)
{
    QModelIndex index = item->parent() ? item->parent()->index() : QModelIndex();
    bool hideThisOne = hide && (item->checkState() == Qt::Checked);
    treeView->setRowHidden(item->row(), index, hideThisOne);
    if (!hideThisOne) {
        for (int row = 0; row < item->rowCount(); ++row) {
            hideOrShowDoneTask(hide, item->child(row, 0));
        }
    }
}
#else
void MainWindow::hideOrShowDoneTask(bool hide, const QModelIndex &index)
{
    bool hideThisOne = hide && model->isChecked(index);
    if (index.isValid()) {
        treeView->setRowHidden(index.row(), index.parent(), hideThisOne);
    }
    if (!hideThisOne) {
        for (int row = 0; row < model->rowCount(index); ++row) {
            hideOrShowDoneTask(hide, model->index(row, 0, index));
        }
    }
}
#endif
