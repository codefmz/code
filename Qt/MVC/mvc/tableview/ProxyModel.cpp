#include "ProxyModel.h"
#include "Base.h"


ProxyModel::ProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    minZipcode = maxZipcode = InvalidZipCode;
}

void ProxyModel::setState(const QString &state)
{
    if (state != this->state) {
        this->state = state;
        invalidateFilter();
    }
}

void ProxyModel::setCounty(const QString &county)
{
    if (county != this->county) {
        this->county = county;
        invalidateFilter();
    }
}

void ProxyModel::setMinZipcode(quint32 minZipcode)
{
    this->minZipcode = minZipcode;
    invalidateFilter();
}

void ProxyModel::setMaxZipcode(quint32 maxZipcode)
{
    this->maxZipcode = maxZipcode;
    invalidateFilter();
}

void ProxyModel::clearFilters()
{
    minZipcode = maxZipcode = InvalidZipCode;
    county.clear();
    state.clear();
    invalidateFilter();
}

bool ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (minZipcode != InvalidZipCode || maxZipcode != InvalidZipCode) {
        QModelIndex index = sourceModel()->index(sourceRow, ZipCode, sourceParent);
        if (minZipcode != InvalidZipCode && sourceModel()->data(index).toInt() < minZipcode) {
            return false;
        }
        if (maxZipcode != InvalidZipCode && sourceModel()->data(index).toInt() > maxZipcode) {
            return false;
        }
    }

    if (!county.isEmpty()) {
        QModelIndex index = sourceModel()->index(sourceRow, County, sourceParent);
        if (county != sourceModel()->data(index).toString()) {
            return false;
        }
    }

    if (!state.isEmpty()) {
        QModelIndex index = sourceModel()->index(sourceRow, State, sourceParent);
        if (state != sourceModel()->data(index).toString()) {
            return false;
        }
    }
    return true;
}
