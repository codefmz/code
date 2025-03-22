#ifndef STANDARDTREEMODEL_H
#define STANDARDTREEMODEL_H

#include <QStandardItemModel>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>

class QStandardItem;
class StandardItem;

class StandardTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum Insert {
        AtTopLevel,
        AsSibling,
        AsChild
    };
    explicit StandardTreeModel(QObject *parent = nullptr);

    QString filename() const {
        return m_filename;
    }
    void setFilename(const QString &filename) {
        m_filename = filename;
    }
    void clear();
    void load(const QString &filename=QString());
    void save(const QString &filename=QString());
    QStandardItem *insertNewTask(Insert insert, const QString &name, const QModelIndex &index);
    QStringList pathForIndex(const QModelIndex &index) const;
    QStandardItem *itemForPath(const QStringList &path) const;
private:
    void initialize();
    void calculateTotalsFor(QStandardItem *root);
    QStandardItem *itemForPath(QStandardItem *root, const QStringList &path) const;
    StandardItem *createNewTask(QStandardItem *root, const QString &name, bool checked);
    void writeTaskAndChildren(QXmlStreamWriter *writer, QStandardItem *item);
private:
    QString m_filename;
};

#endif // STANDARDTREEMODEL_H
