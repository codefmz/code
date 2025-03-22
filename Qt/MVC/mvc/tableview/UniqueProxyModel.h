#ifndef UNIQUEPROXYMODEL_H
#define UNIQUEPROXYMODEL_H

#include <QSet>
#include <QSortFilterProxyModel>
#include <QDebug>

class UniqueProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit UniqueProxyModel(int column, QObject *parent = nullptr)
        : QSortFilterProxyModel(parent), Column(column) {};
    void setSourceModel(QAbstractItemModel *sourceModel) override;
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private slots:
    void clearCache() {
        qDebug() <<" UniqueProxyModel clearCache";
        cache.clear();
    }
private:
    const int Column;
    mutable QSet<QString> cache;
};

#endif // UNIQUEPROXYMODEL_H
