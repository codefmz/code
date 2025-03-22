#ifndef SPINBOX_H
#define SPINBOX_H

#include "Base.h"
#include <QSpinBox>

class ZipCodeSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit ZipCodeSpinBox(QWidget *parent) : QSpinBox(parent) {
        setRange(MinZipCode, MaxZipCode);
        setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    }

protected:
    QString textFromValue(int value) const {
        return QString("%1").arg(value, 5, 10, QChar('0'));
    }
};

#endif // SPINBOX_H
