#ifndef STANDARDTABLEMODEL_H
#define STANDARDTABLEMODEL_H

#include <QStandardItemModel>

class StandardTableModel : public QStandardItemModel
{
public:
    StandardTableModel(QObject *parent = nullptr);
private:
    void initialize();
    void clear();
public:
    void save(const QString &fileName = "");
    void load(const QString &fileName);
    QString getFileName() const;
    void setFileName(const QString &newFileName);
private:
    QString fileName;
};

#endif // STANDARDTABLEMODEL_H
