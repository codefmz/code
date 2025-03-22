#include "RichTextLineEdit.h"
#include "RichTextDelegate.h"
#include <QModelIndex>
#include <QPainter>
#include <QPixmapCache>
#include <QDebug>

RichTextDelegate::RichTextDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    checkbox = new QCheckBox;
    checkbox->setFixedSize(qRound(1.3 * checkbox->sizeHint().height()), checkbox->sizeHint().height());
    label = new QLabel;
    label->setTextFormat(Qt::RichText);
    label->setWordWrap(false);
}

void RichTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool selected = option.state & QStyle::State_Selected;
    QPalette palette(option.palette);
    palette.setColor(QPalette::Active, QPalette::Window, selected ?
        option.palette.highlight().color() : option.palette.base().color());
    palette.setColor(QPalette::Active, QPalette::WindowText, selected ?
        option.palette.highlightedText().color() : option.palette.text().color());

    int yOffset = checkbox->height() < option.rect.height() ? (option.rect.height() - checkbox->height()) / 2 : 0;
    QRect checkboxRect(option.rect.x(), option.rect.y() + yOffset, checkbox->width(), checkbox->height());
    checkbox->setPalette(palette);
    bool checked = index.model()->data(index, Qt::CheckStateRole).toInt() == Qt::Checked;
    checkbox->setChecked(checked);
    QRect labelRect(option.rect.x() + checkbox->width(), option.rect.y(), option.rect.width() - checkbox->width(),
        option.rect.height());
    label->setPalette(palette);
    label->setFixedSize(qMax(0, labelRect.width()), labelRect.height());
    QString html = index.model()->data(index, Qt::DisplayRole).toString();
    label->setText(html);
    QString checkboxKey = QString("CHECKBOX:%1.%2").arg(selected).arg(checked);
    paintWidget(painter, checkboxRect, checkboxKey, checkbox);
    QString labelKey = QString("LABEL:%1.%2.%3x%4").arg(selected).arg(html).arg(labelRect.width()).arg(labelRect.height());
    paintWidget(painter, labelRect, labelKey, label);
}

void RichTextDelegate:: paintWidget(QPainter *painter, const QRect &rect, const QString &cacheKey, QWidget *widget) const
{
    QPixmap pixmap(widget->size());
    if (!QPixmapCache::find(cacheKey, &pixmap)) {
        widget->render(&pixmap);
        QPixmapCache::insert(cacheKey, pixmap);
    }
    painter->drawPixmap(rect, pixmap);
}

QSize RichTextDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString html = index.model()->data(index, Qt::DisplayRole) .toString();
    document.setDefaultFont(option.font);
    document.setHtml(html);
    return QSize(document.idealWidth(), option.fontMetrics.height());
}

QWidget *RichTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex&) const
{
    RichTextLineEdit *editor = new RichTextLineEdit(parent);
    editor->viewport()->setFixedHeight(option.rect.height());
    connect(editor, SIGNAL(returnPressed()), this, SLOT(closeAndCommitEditor()));
    return editor;
}

void RichTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString html = index.model()->data(index, Qt::DisplayRole).toString();
    RichTextLineEdit *lineEdit = qobject_cast<RichTextLineEdit*>(editor);
    Q_ASSERT(lineEdit);
    lineEdit->setHtml(html);
}

void RichTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    RichTextLineEdit *lineEdit = qobject_cast<RichTextLineEdit*>(editor);
    Q_ASSERT(lineEdit);
    model->setData(index, lineEdit->toSimpleHtml());
}

void RichTextDelegate::closeAndCommitEditor()
{
    RichTextLineEdit *lineEdit = qobject_cast<RichTextLineEdit*>(sender());
    Q_ASSERT(lineEdit);
    emit commitData(lineEdit);//通知代理编辑完毕，调用setModelData 方法
    emit closeEditor(lineEdit);
}
