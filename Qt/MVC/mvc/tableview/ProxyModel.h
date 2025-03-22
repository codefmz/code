#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>

class ProxyModel : public QSortFilterProxyModel
{
public:
    explicit ProxyModel(QObject *parent = nullptr);

    void setState(const QString &state);
    void setCounty(const QString &county);
    void setMinZipcode(quint32 minZipcode);
    void setMaxZipcode(quint32 maxZipcode);
    void clearFilters();
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    quint32 minZipcode;
    quint32 maxZipcode;
    QString state;
    QString county;
};

#endif // PROXYMODEL_H
