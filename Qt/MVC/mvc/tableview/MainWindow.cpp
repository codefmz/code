#include "MainWindow.h"
#include "Base.h"
#include "AQP.h"
#ifdef CUSTOM_MODEL
#include "TableModel.h"
#else
#include "StandardTableModel.h"
#endif
#include "UniqueProxyModel.h"
#include "ProxyModel.h"
#include "ItemDelegate.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QFileDialog>
#include <QLabel>
#include <QSpinBox>
#include <QTableView>
#include <QHeaderView>
#include <QApplication>
#include <QLayout>
#include <QStatusBar>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), loading(false)
{
#ifdef CUSTOM_MODEL
    model = new TableModel(this);
#else
    model = new StandardTableModel(this);
#endif
    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(model);

    createWidgets();
    createComBoxModels();
    createLayout();
    createConnections();
#ifdef CUSTOM_MODEL
    setWindowTitle(tr("%1 (Custom Model)[*]").arg(QApplication::applicationName()));
#else
    setWindowTitle(tr("%1 (QStandardItemModel)[*]").arg(QApplication::applicationName()));
#endif
    statusBar()->showMessage(tr("Ready"), StatusTimeout);
}

void MainWindow::createWidgets()
{
    buttonBox = new QDialogButtonBox;
    buttonBox->setOrientation(Qt::Horizontal);
    quitButton = buttonBox->addButton(tr("Quit"), QDialogButtonBox::ApplyRole);
    addButton = buttonBox->addButton(tr("Add"), QDialogButtonBox::ActionRole);
    addButton->setEnabled(false);
    deleteButton = buttonBox->addButton(tr("Delete..."), QDialogButtonBox::ActionRole);
    deleteButton->setEnabled(false);
    loadButton = buttonBox->addButton(tr("Load..."), QDialogButtonBox::AcceptRole);
    saveButton = buttonBox->addButton(tr("&Save"), QDialogButtonBox::AcceptRole);
    saveButton->setEnabled(false);
    buttonBox->setCenterButtons(true);

    filterSelectGroupBox = new QGroupBox(tr("Filter or Select"));
    dontFilterOrSelectRadioButton = new QRadioButton(tr("Don't Filter or Select"));
    filterRadioButton = new QRadioButton(tr("Filter"));
    filterRadioButton->setChecked(true);
    selectByCriteriaRadioButton = new QRadioButton(tr("Select by Criteria"));

    minZipLabel = new QLabel(tr("Min. Zip:"));
    minZipSpinBox = new QSpinBox;
    minZipLabel->setBuddy(minZipSpinBox);
    minZipSpinBox->setRange(MinZipCode, MaxZipCode);
    minZipSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    maxZipLabel = new QLabel(tr("Max. Zip:"));
    maxZipSpinBox = new QSpinBox;
    maxZipLabel->setBuddy(maxZipSpinBox);
    maxZipSpinBox->setRange(MinZipCode, MaxZipCode);
    maxZipSpinBox->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    maxZipSpinBox->setValue(MaxZipCode);

    countyGroupBox = new QGroupBox(tr("County"));
    countyGroupBox->setCheckable(true);
    countyGroupBox->setChecked(false);
    countyComboBox = new QComboBox;
    stateGroupBox = new QGroupBox(tr("State"));
    stateGroupBox->setCheckable(true);
    stateGroupBox->setChecked(false);
    stateComboBox = new QComboBox;

    tableView = new QTableView;
    tableView->setModel(proxyModel);
    tableView->setItemDelegate(new ItemDelegate(this));
    tableView->verticalHeader()->setDefaultAlignment(Qt::AlignVCenter|Qt::AlignRight);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::createLayout()
{
    QHBoxLayout *groupBoxTopLayout = new QHBoxLayout;
    groupBoxTopLayout->addWidget(dontFilterOrSelectRadioButton);
    groupBoxTopLayout->addWidget(filterRadioButton);
    groupBoxTopLayout->addWidget(selectByCriteriaRadioButton);
    groupBoxTopLayout->addWidget(minZipLabel);
    groupBoxTopLayout->addWidget(minZipSpinBox);
    groupBoxTopLayout->addWidget(maxZipLabel);
    groupBoxTopLayout->addWidget(maxZipSpinBox);

    QHBoxLayout *comboBoxLayout = new QHBoxLayout;
    comboBoxLayout->addWidget(countyComboBox);
    countyGroupBox->setLayout(comboBoxLayout);

    comboBoxLayout = new QHBoxLayout;
    comboBoxLayout->addWidget(stateComboBox);
    stateGroupBox->setLayout(comboBoxLayout);

    QHBoxLayout *groupBoxBottomLayout = new QHBoxLayout;
    groupBoxBottomLayout->addWidget(countyGroupBox);
    groupBoxBottomLayout->addWidget(stateGroupBox);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addLayout(groupBoxTopLayout);
    groupBoxLayout->addLayout(groupBoxBottomLayout);
    filterSelectGroupBox->setLayout(groupBoxLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(buttonBox);
    layout->addWidget(tableView);
    layout->addWidget(filterSelectGroupBox);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    loadButton->setFocus();
}

void MainWindow::load()
{
    QString filename(model->getFileName());
    QString dir(filename.isEmpty() ? QString(".") : QFileInfo(filename).canonicalPath());
    filename = QFileDialog::getOpenFileName(this, tr("%1 - Open").arg(QApplication::applicationName()),
        dir, tr("%1 (*.dat)").arg(QApplication::applicationName()));
    if (filename.isEmpty()) {
        return;
    }

    enableButtons(false);
    QMetaObject::invokeMethod(this, "load", Qt::QueuedConnection, Q_ARG(QString, filename));
}

void MainWindow::load(const QString &fileName)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    loading = true;
    try {
        model->load(fileName);
        tableView->resizeColumnsToContents();
        QHeaderView *header = tableView->horizontalHeader();
        header->setSortIndicatorShown(true);
        header->setSortIndicator(0, Qt::AscendingOrder);
        createComBoxModels();
        setWindowModified(false);
        setWindowTitle(tr("%1 - %2[*]").arg(QApplication::applicationName()).arg(QFileInfo(fileName).fileName()));
        statusBar()->showMessage(tr("Loaded %n zipcode(s) from %1", "", model->rowCount()).arg(fileName), StatusTimeout);
    } catch (AQP::Error &error) {
        AQP::warning(this, tr("Error"), tr("Failed to load %1: %2").arg(fileName).arg(QString::fromUtf8(error.what())));
    }
    loading = false;
    tableView->setFocus();
    radioButtonClicked();
    enableButtons(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::enableButtons(bool enable)
{
    foreach(QPushButton *button, QList<QPushButton*>() << loadButton << saveButton << addButton << deleteButton) {
        button->setEnabled(enable);
        button->repaint(); // update isn't "strong" enough on non-Mac
    }
}

bool MainWindow::save()
{
    try {
        model->save();
        setWindowModified(false);
        setWindowTitle(tr("%1 - %2[*]").arg(QApplication::applicationName())
            .arg(QFileInfo(model->getFileName()).fileName()));
        statusBar()->showMessage(tr("Saved %1").arg(model->getFileName()), StatusTimeout);
        return true;
    } catch (AQP::Error &error) {
        AQP::warning(this, tr("Error"), tr("Failed to save %1: %2").arg(model->getFileName())
            .arg(QString::fromUtf8(error.what())));
        return false;
    }
}

void MainWindow::addZipCode()
{
    dontFilterOrSelectRadioButton->click();
#ifdef CUSTOM_MODEL
    if (!model->insertRow(model->rowCount())) {
        return;
    }
#else
    QList<QStandardItem *> items;
    QStandardItem *zipItem = new QStandardItem;
    zipItem->setData(MinZipCode, Qt::EditRole);
    items << zipItem;
    for (int i = 0; i < model->columnCount() - 1; ++i) {
        items << new QStandardItem(tr("Unknown"));
    }
    model->appendRow(items);
#endif

    tableView->scrollToBottom();
    tableView->setFocus();
    QModelIndex index = proxyModel->index(proxyModel->rowCount() - 1, ZipCode);
    tableView->setCurrentIndex(index);
    tableView->edit(index);
}

void MainWindow::deleteZipcode()
{
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    if (!selectionModel->hasSelection()) {
        return;
    }
    QModelIndex index = proxyModel->mapToSource(selectionModel->currentIndex());
    if (!index.isValid()) {
        return;
    }

    int zipcode = model->data(model->index(index.row(), ZipCode)).toInt();
    if (!AQP::okToDelete(this, tr("Delete Zipcode"), tr("Delete Zipcode %1?").arg(zipcode, 5, 10, QChar('0')))) {
        return;
    }

    bool filtered = filterRadioButton->isChecked();
    bool selected = selectByCriteriaRadioButton->isChecked();
    QString county = countyGroupBox->isChecked() ? countyComboBox->currentText() : QString();
    QString state = stateGroupBox->isChecked() ? stateComboBox->currentText() : QString();

    dontFilterOrSelectRadioButton->click();
    model->removeRow(index.row(), index.parent());

    createComBoxModels();
    if (!county.isEmpty()) {
        countyComboBox->setCurrentIndex(countyComboBox->findText(county));
    }
    if (!state.isEmpty()) {
        stateComboBox->setCurrentIndex(stateComboBox->findText(state));
    }
    if (filtered) {
        filterRadioButton->click();
    } else if (selected) {
        selectByCriteriaRadioButton->click();
    }
}

void MainWindow::updateUi()
{
    if (loading || dontFilterOrSelectRadioButton->isChecked()) {
        return;
    }

    if (filterRadioButton->isChecked()) {
        restoreFilters();
    } else {
        performSelection();
    }
}

void MainWindow::radioButtonClicked()
{
    if (dontFilterOrSelectRadioButton->isChecked()) {
        proxyModel->clearFilters();
        QItemSelectionModel *selectionModel = tableView->selectionModel();
        selectionModel->clearSelection();
    } else {
        updateUi();
    }
}

void MainWindow::selectionChanged()
{
    if (filterRadioButton->isChecked()) {
        return;
    }
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    if (selectionModel->selection().indexes().count() <= 1) {
        dontFilterOrSelectRadioButton->setChecked(true);
    }
}

void MainWindow::createComBoxModels()
{
    createComBoxModel(countyComboBox, County);
    createComBoxModel(stateComboBox, State);
}

void MainWindow::createComBoxModel(QComboBox *comboBox, int column)
{
    delete comboBox->model();

    UniqueProxyModel *uniqueProxyModel = new UniqueProxyModel(column, this);
    uniqueProxyModel->setSourceModel(model);
    uniqueProxyModel->sort(column, Qt::AscendingOrder);
    comboBox->setModel(uniqueProxyModel);
    comboBox->setModelColumn(column); /* 显示哪一个列的model */
}

bool MainWindow::okToClearData()
{
    if (isWindowModified()) {
        return AQP::okToClearData(&MainWindow::save, this, tr("Unsaved changes"), tr("Save unsaved changes?"));
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToClearData()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::createConnections()
{
#ifdef CUSTOM_MODEL
    connect(model,SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(setDirty()));
#else
    connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(setDirty()));
#endif
    connect(model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(setDirty()));
    connect(model, SIGNAL(modelReset()), this, SLOT(setDirty()));

    connect(countyGroupBox, SIGNAL(toggled(bool)), this, SLOT(updateUi()));
    connect(countyComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateUi()));
    connect(stateGroupBox, SIGNAL(toggled(bool)), this, SLOT(updateUi()));
    connect(stateComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateUi()));
    connect(minZipSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateUi()));
    connect(maxZipSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateUi()));

    QList<QRadioButton *> radioBtns;
    radioBtns << dontFilterOrSelectRadioButton << filterRadioButton << selectByCriteriaRadioButton;

    foreach (QRadioButton *radioBtn, radioBtns) {
        connect(radioBtn, SIGNAL(clicked()), this, SLOT(radioButtonClicked()));
    }

    /* table */
    connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectionChanged()));
    connect(tableView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
        this, SLOT(selectionChanged()));
    connect(tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), tableView, SLOT(sortByColumn(int)));

    connect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addZipCode()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteZipcode()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

void MainWindow::restoreFilters()
{
    proxyModel->setMinZipcode(minZipSpinBox->value());
    proxyModel->setMaxZipcode(maxZipSpinBox->value());
    proxyModel->setCounty(countyGroupBox->isChecked() ? countyComboBox->currentText() : QString());
    proxyModel->setState(stateGroupBox->isChecked() ? stateComboBox->currentText() : QString());

    reportFilterEffect();
}

void MainWindow::performSelection()
{
    proxyModel->clearFilters();
    int minZipcode = minZipSpinBox->value();
    int maxZipcode = maxZipSpinBox->value();
    QString county = countyGroupBox->isChecked() ? countyComboBox->currentText() : QString();
    QString state = stateGroupBox->isChecked() ? stateComboBox->currentText() : QString();

    QItemSelection selection;
    int firstSelectedRow = -1;
    for (int row = 0; row < proxyModel->rowCount(); ++row) {
        QModelIndex index = proxyModel->index(row, ZipCode);
        int zipcode = proxyModel->data(index).toInt();
        if (zipcode < minZipcode || zipcode > maxZipcode) {
            continue;
        }
        if (!matchingColumn(county, row, County)) {
            continue;
        }
        if (!matchingColumn(state, row, State)) {
            continue;
        }
        if (firstSelectedRow == -1) {
            firstSelectedRow = row;
        }
        QItemSelection rowSelection(index, index);
        selection.merge(rowSelection, QItemSelectionModel::Select);
    }

    QItemSelectionModel *selectionModel = tableView->selectionModel();
    selectionModel->clearSelection();
    selectionModel->select(selection, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    if (firstSelectedRow != -1) {
        tableView->scrollTo(proxyModel->index(firstSelectedRow, 0));
    }
    statusBar()->showMessage(tr("Selected %L1 out of %Ln zipcode(s)", "",
        model->rowCount()).arg(selection.count()), StatusTimeout);
}

void MainWindow::reportFilterEffect()
{
    if (loading) {
        return;
    }

    statusBar()->showMessage(tr("Filtered %L1 out of %Ln zipcode(s)", "",
        model->rowCount()).arg(proxyModel->rowCount()), StatusTimeout);
}

bool MainWindow::matchingColumn(const QString &value, int row, int column)
{
    if (value.isEmpty()) {
        return true;
    }
    QModelIndex index = proxyModel->index(row, column);

    return value == proxyModel->data(index).toString();
}
