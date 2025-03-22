﻿#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>

struct ZipCodeItem;

class TableModel : public QAbstractTableModel
{
public:
    explicit TableModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int=Qt::EditRole) override {
        return false;
    }
    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex()) override;
    QString getFileName() const {
        return m_filename;
    }
    void load(const QString &filename=QString());
    void save(const QString &filename=QString());
private:
    QString m_filename;
    QList<ZipCodeItem> zipcodes;
};

#endif // TABLEMODEL_H
