#ifndef TASKITEM_H
#define TASKITEM_H

#include <QDateTime>
#include <QList>
#include <QPair>
#include <QString>

class TaskItem
{
public:
    explicit TaskItem(const QString &name=QString(), bool done = false, TaskItem *parent = nullptr);
    ~TaskItem() {
        qDeleteAll(m_children);
    }

    QString name() const {
        return m_name;
    }

    void setName(const QString &name) {
        m_name = name;
    }

    bool isDone() const {
        return m_done;
    }

    void setDone(bool done) {
        m_done = done;
    }

    QList<QPair<QDateTime, QDateTime> > dateTimes() const {
        return m_dateTimes;
    }

    void addDateTime(const QDateTime &start, const QDateTime &end) {
        m_dateTimes << qMakePair(start, end);
    }

    QString todaysTime() const;
    QString totalTime() const;
    void incrementLastEndTime(int msec);

    TaskItem *parent() const {
        return m_parent;
    }

    TaskItem *childAt(int row) const {
        return m_children.value(row);
    }

    int rowOfChild(TaskItem *child) const {
        return m_children.indexOf(child);
    }
    int childCount() const {
        return m_children.count();
    }

    bool hasChildren() const {
        return !m_children.isEmpty();
    }

    QList<TaskItem*> children() const {
        return m_children;
    }

    void insertChild(int row, TaskItem *item) {
        item->m_parent = this;
        m_children.insert(row, item);
    }

    void addChild(TaskItem *item) {
        item->m_parent = this;
        m_children << item;
    }

    void swapChildren(int oldRow, int newRow) {
        m_children.swapItemsAt(oldRow, newRow);
    }

    TaskItem* takeChild(int row);
private:
    int minutesForTask(bool onlyForToday) const;

private:
    QString m_name;
    bool m_done;
    QList<QPair<QDateTime, QDateTime> > m_dateTimes;
    TaskItem *m_parent;
    QList<TaskItem*> m_children;
};

#endif // TASKITEM_HPP
