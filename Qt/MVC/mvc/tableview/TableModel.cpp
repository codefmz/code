#include "ZipCodeItem.h"
#include "TableModel.h"
#include "Base.h"
#include "AQP.h"
#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QFontMetrics>
#include <QStyleOptionComboBox>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
    return theFlags;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= zipcodes.count() ||
        index.column() < 0 || index.column() >= MaxColumns) {
        return QVariant();
    }

    const ZipCodeItem &item = zipcodes.at(index.row());
    if (role == Qt::SizeHintRole) {
        QStyleOptionComboBox option;
        switch (index.column()) {
            case ZipCode: {
                option.currentText = QString::number(MaxZipCode);
                const QString header = headerData(ZipCode, Qt::Horizontal, Qt::DisplayRole).toString();
                if (header.length() > option.currentText.length()) {
                    option.currentText = header;
                }
                break;
            }
            case PostOffice:
                option.currentText = item.postOffice;
                break;
            case County:
                option.currentText = item.county;
                break;
            case State:
                option.currentText = item.state;
                break;
            default:
                Q_ASSERT(false);
        }
        QFontMetrics fontMetrics(data(index, Qt::FontRole).value<QFont>());
        option.fontMetrics = fontMetrics;
        QSize size(fontMetrics.width(option.currentText), fontMetrics.height());
        return qApp->style()->sizeFromContents(QStyle::CT_ComboBox, &option, size);
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case ZipCode:
                return item.zipcode;
            case PostOffice:
                return item.postOffice;
            case County:
                return item.county;
            case State:
                return item.state;
            default:
                Q_ASSERT(false);
        }
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case ZipCode:
                return tr("Zipcode");
            case PostOffice:
                return tr("Post Office");
            case County:
                return tr("County");
            case State:
                return tr("State");
            default:
                Q_ASSERT(false);
        }
    }

    return section + 1;
}

int TableModel::rowCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : zipcodes.count();
}

int TableModel::columnCount(const QModelIndex &index) const
{
    return index.isValid() ? 0 : MaxColumns;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || index.row() < 0 ||
        index.row() >= zipcodes.count() || index.column() < 0 || index.column() >= MaxColumns) {
        return false;
    }

    ZipCodeItem &item = zipcodes[index.row()];
    switch (index.column()) {
        case ZipCode: {
            bool ok;
            int zipcode = value.toInt(&ok);
            if (!ok || zipcode < MinZipCode || zipcode > MaxZipCode) {
                return false;
            }
            item.zipcode = zipcode;
            break;
        }
        case PostOffice:
            item.postOffice = value.toString();
            break;
        case County:
            item.county = value.toString();
            break;
        case State:
            item.state = value.toString();
            break;
        default:
            Q_ASSERT(false);
    }

    emit dataChanged(index, index);

    return true;
}

bool TableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        zipcodes.insert(row, ZipCodeItem());
    }
    endInsertRows();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        zipcodes.removeAt(row);
    }
    endRemoveRows();
    return true;
}

void TableModel::load(const QString &filename)
{
    if (!filename.isEmpty()) {
        m_filename = filename;
    }

    if (m_filename.isEmpty()) {
        throw AQP::Error(tr("no filename specified"));
    }

    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly)) {
        throw AQP::Error(file.errorString());
    }

    QDataStream in(&file);
    qint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != MagicNumber) {
        throw AQP::Error(tr("unrecognized file type"));
    }

    qint16 formatVersionNumber;
    in >> formatVersionNumber;
    if (formatVersionNumber > FormatNumber) {
        throw AQP::Error(tr("file format version is too new"));
    }

    in.setVersion(QDataStream::Qt_4_5);
    zipcodes.clear();

    beginResetModel();
    ZipCodeItem item;
    while (!in.atEnd()) {
        in >> item;
        zipcodes << item;
    }
    qSort(zipcodes);
    endResetModel();
}

void TableModel::save(const QString &filename)
{
    if (!filename.isEmpty()) {
        m_filename = filename;
    }

    if (m_filename.isEmpty()) {
        throw AQP::Error(tr("no filename specified"));
    }

    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly)) {
        throw AQP::Error(file.errorString());
    }

    QDataStream out(&file);
    out << MagicNumber << FormatNumber;
    out.setVersion(QDataStream::Qt_4_5);
    QListIterator<ZipCodeItem> i(zipcodes);
    while (i.hasNext()) {
        out << i.next();
    }
}
