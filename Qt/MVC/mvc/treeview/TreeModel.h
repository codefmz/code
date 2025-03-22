#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QIcon>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#include "TaskItem.h"

class QMimeData;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = nullptr) : QAbstractItemModel(parent), timedItem(nullptr),
        rootItem(nullptr), cutItem(nullptr) {
    }

    ~TreeModel() {
        if (rootItem != nullptr) {
            delete rootItem;
            rootItem = nullptr;
        }

        if (cutItem != nullptr) {
            delete cutItem;
            cutItem = nullptr;
        }
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool setHeaderData(int, Qt::Orientation, const QVariant&, int=Qt::EditRole) override {
        return false;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex()) override;

    Qt::DropActions supportedDragActions() const override {
        return Qt::MoveAction;
    }

    Qt::DropActions supportedDropActions() const override {
        return Qt::MoveAction;
    }

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column,
        const QModelIndex &parent) override;

    bool isChecked(const QModelIndex &index) const;
    bool hasCutItem() const {
        return cutItem != nullptr;
    }

    QModelIndex moveUp(const QModelIndex &index);
    QModelIndex moveDown(const QModelIndex &index);
    QModelIndex cut(const QModelIndex &index);
    QModelIndex paste(const QModelIndex &index);
    QModelIndex promote(const QModelIndex &index);
    QModelIndex demote(const QModelIndex &index);

    void setTimedItem(const QModelIndex &index);
    void clearTimedItem();
    bool isTimedItem(const QModelIndex &index);
    void addDateTimeToTimedItem(const QDateTime &start, const QDateTime &end);
    void setIconForTimedItem(const QIcon &icon=QIcon());
    void incrementEndTimeForTimedItem(int msec);

    void clear();
    QString filename() const {
        return m_filename;
    }
    void setFilename(const QString &filename) {
        m_filename = filename;
    }

    void load(const QString &filename=QString());
    void save(const QString &filename=QString());
    QStringList pathForIndex(const QModelIndex &index) const;
    QModelIndex indexForPath(const QStringList &path) const;
signals:
    void stopTiming();
private:
    TaskItem *itemForIndex(const QModelIndex &index) const;
    void readTasks(QXmlStreamReader *reader, TaskItem *task);
    void writeTaskAndChildren(QXmlStreamWriter *writer, TaskItem *task) const;
    void announceItemChanged(TaskItem *item);
    QModelIndex indexForPath(const QModelIndex &parent, const QStringList &path) const;
    QModelIndex moveItem(TaskItem *parent, int oldRow, int newRow);

private:
    QString m_filename;
    QIcon m_icon;
    TaskItem *timedItem;
    TaskItem *rootItem;
    TaskItem *cutItem;
};
#endif // TREEMODEL_HPP
