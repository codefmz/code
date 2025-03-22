#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QTableView;
#ifdef CUSTOM_MODEL
class TableModel;
#else
class StandardTableModel;
#endif
class ProxyModel;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool okToClearData();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void load();
    void load(const QString &fileName);
    bool save();
    void addZipCode();
    void deleteZipcode();
    void setDirty() {
        setWindowModified(true);
    }
    void updateUi();
    void radioButtonClicked();
    void selectionChanged();
private:
    void createWidgets();
    void createLayout();
    void createComBoxModels();
    void createComBoxModel(QComboBox *comboBox, int column);
    void createConnections();
    void restoreFilters();
    void performSelection();
    void reportFilterEffect();
    void enableButtons(bool enable);
    bool matchingColumn(const QString &value, int row, int column);
private:
    QTableView *tableView;
    QDialogButtonBox *buttonBox;
#ifdef CUSTOM_MODEL
    TableModel *model;
#else
    StandardTableModel *model;
#endif
    QGroupBox *filterSelectGroupBox;
    QComboBox *countyComboBox;
    QGroupBox *countyGroupBox;
    QComboBox *stateComboBox;
    QGroupBox *stateGroupBox;
    QRadioButton *dontFilterOrSelectRadioButton;
    QRadioButton *filterRadioButton;
    QRadioButton *selectByCriteriaRadioButton;
    QLabel *minZipLabel;
    QLabel *maxZipLabel;
    QSpinBox *minZipSpinBox;
    QSpinBox *maxZipSpinBox;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *quitButton;
    ProxyModel *proxyModel;
    bool loading;
};

#endif // MAINWINDOW_H
