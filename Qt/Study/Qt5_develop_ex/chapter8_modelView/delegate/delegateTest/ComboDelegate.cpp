﻿#include "ComboDelegate.h"
#include "QComboBox"
ComboDelegate::ComboDelegate(QObject *parent)
    : QItemDelegate{parent}
{

}

QWidget *ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox * editor = new QComboBox(parent);
    editor->setStyleSheet("QComboBox{color:rgb(30,144,255)}");
    editor->addItem("Doctor");
    editor->addItem("Engineer");
    editor->addItem("Soldier");
    editor->addItem("Lawyer");
    editor->addItem("Farmer");
    editor->installEventFilter(const_cast<ComboDelegate*>(this));
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index).toString();
    QComboBox * box = static_cast<QComboBox*>(editor);
    int i = box->findText(str);
    box->setCurrentIndex(i);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox * box = static_cast<QComboBox*>(editor);
    QString str = box->currentText();
    model->setData(index, str);
}

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
