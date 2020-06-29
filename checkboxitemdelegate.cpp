#include "checkboxitemdelegate.h"
#include <QDebug>
#include <QCheckBox>

CheckboxItemDelegate::CheckboxItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}


void CheckboxItemDelegate::setArcherTableModel(ArcherTableModel* model) {
    _archerTableModel = model;
}


QWidget* CheckboxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QCheckBox* checkbox = new QCheckBox(parent);
    return checkbox;
}

void CheckboxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (_archerTableModel==0) return;
    QCheckBox* checkbox = (QCheckBox*)editor;
    checkbox->setChecked(_archerTableModel->data(index, Qt::UserRole).toBool());
}

void CheckboxItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void CheckboxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (_archerTableModel==0) return;
    QCheckBox* checkbox = (QCheckBox*)editor;
    _archerTableModel->setData(index, QVariant(checkbox->isChecked()));
}
