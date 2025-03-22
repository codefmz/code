#ifndef RICHTEXTDELEGATE_H
#define RICHTEXTDELEGATE_H

#include <QCheckBox>
#include <QLabel>
#include <QStyledItemDelegate>
#include <QTextDocument>

class QModelIndex;
class QPainter;
class QStyleOptionViewItem;

class RichTextDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RichTextDelegate(QObject *parent=0);

    ~RichTextDelegate() {
        delete checkbox;
        delete label;
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
private slots:
    void closeAndCommitEditor();

private:
    void paintWidget(QPainter *painter, const QRect &rect, const QString &cacheKey, QWidget *widget) const;
private:
    QCheckBox *checkbox;
    QLabel *label;
    mutable QTextDocument document;
};

#endif // RICHTEXTDELEGATE_HPP
