#ifndef BASE_H
#define BASE_H

#include <QDebug>

const qint32 MagicNumber = 1516859459;
const qint16 FormatNumber = 100;
const int MaxColumns = 4;

const int MinZipCode = 0;
const int MaxZipCode = 99999;
const int InvalidZipCode = MinZipCode - 1;
const int StatusTimeout = 100;

enum Colmn {
    ZipCode,
    PostOffice,
    County,
    State
};

#endif // BASE_H
