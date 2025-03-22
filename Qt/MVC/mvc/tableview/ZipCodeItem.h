#ifndef ZIPCODEITEM_H
#define ZIPCODEITEM_H

#include "Base.h"
#include <QString>
#include <QDataStream>

struct ZipCodeItem
{
    explicit ZipCodeItem(int zipcode_= InvalidZipCode, const QString &postOffice_=QString(),
        const QString &county_=QString(), const QString &state_=QString()) : zipcode(zipcode_),
        postOffice(postOffice_), county(county_), state(state_) {}

    bool operator<(const ZipCodeItem &other) const {
        return zipcode != other.zipcode ?
            zipcode < other.zipcode : postOffice < other.postOffice;
    }

    int zipcode;
    QString postOffice;
    QString county;
    QString state;
};

inline QDataStream &operator<<(QDataStream &out, const ZipCodeItem &item)
{
    out << static_cast<quint16>(item.zipcode) << item.postOffice
        << item.county << item.state;
    return out;
}


inline QDataStream &operator>>(QDataStream &in, ZipCodeItem &item)
{
    quint16 zipcode;
    in >> zipcode >> item.postOffice >> item.county >> item.state;
    item.zipcode = static_cast<int>(zipcode);
    return in;
}

#endif // ZIPCODEITEM_H
