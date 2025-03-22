#include "StandardTableModel.h"
#include "AQP.h"
#include "Base.h"
#include <exception>
#include <QFile>

StandardTableModel::StandardTableModel(QObject *parent) : QStandardItemModel(parent)
{
    initialize();
}

QString StandardTableModel::getFileName() const
{
    return fileName;
}

void StandardTableModel::setFileName(const QString &newFileName)
{
    fileName = newFileName;
}

void StandardTableModel::initialize()
{
    setHorizontalHeaderLabels(QStringList() << tr("ZipCode") << tr("Post Office")
        << tr("Country") << tr("State"));
}

void StandardTableModel::clear()
{
    QStandardItemModel::clear();
    initialize();
}

void StandardTableModel::save(const QString &saveFileName)
{
    if (!saveFileName.isEmpty()) {
        fileName = saveFileName;
    }

    if (fileName.isEmpty()) {
        throw AQP::Error("no filename specified!");
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        throw AQP::Error(QString("open fileName %1 fail.").arg(fileName));
    }

    QDataStream out(&file);
    out << MagicNumber << FormatNumber;
    out.setVersion(QDataStream::Qt_4_5);
    for (int row = 0; row < rowCount(); ++row) {
        out << static_cast<quint16>(item(row, ZipCode)->data(Qt::EditRole).toUInt()) <<
            item(row, PostOffice)->text() << item(row, County)->text() << item(row, State)->text();
    }
}

void StandardTableModel::load(const QString &loadFileName)
{
    if (!loadFileName.isEmpty()) {
        fileName = loadFileName;
    }

    if (fileName.isEmpty()) {
        throw AQP::Error("no filename specified!");
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        throw AQP::Error(file.errorString());
    }

    QDataStream in(&file);
    qint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != MagicNumber) {
        throw AQP::Error("unrecognized file type");
    }
    qint16 formatVersionNumber;
    in >> formatVersionNumber;
    if (formatVersionNumber > FormatNumber) {
        throw AQP::Error("file version is too new");
    }
    in.setVersion(QDataStream::Qt_4_5);
    clear();

    quint16 zipcode;
    QString postOffice;
    QString county;
    QString state;
    QMultiMap<quint16, QList<QStandardItem*>> itemsForZipcode;
    while (!in.atEnd()) {
        in >> zipcode >> postOffice >> county >> state;
        QList<QStandardItem *> items;
        QStandardItem *item = new QStandardItem;
        item->setData(zipcode, Qt::EditRole);
        items << item;
        foreach (const QString &text, QStringList() << postOffice << county << state) {
            items << new QStandardItem(text);
        }
        itemsForZipcode.insert(zipcode, items);
    }

    QMapIterator<quint16, QList<QStandardItem *>> i(itemsForZipcode);
    while (i.hasNext()) {
        appendRow(i.next().value());
    }
}


