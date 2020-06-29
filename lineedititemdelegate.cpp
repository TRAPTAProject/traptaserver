#include "lineedititemdelegate.h"
#include <QLineEdit>
#include <QDebug>

LineEditItemDelegate::LineEditItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    _archerTableModel = 0;
}

QWidget* LineEditItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QLineEdit* lineEdit = new QLineEdit(parent);
    return lineEdit;
}

void LineEditItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (_archerTableModel==0) return;
    QLineEdit* lineEditor = (QLineEdit*)editor;
    lineEditor->setText(_archerTableModel->data(index, Qt::DisplayRole).toString());
}

void LineEditItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void LineEditItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (_archerTableModel==0) return;
    QLineEdit* lineEditor = (QLineEdit*)editor;
    _archerTableModel->setData(index, QVariant(lineEditor->text()));
}

void LineEditItemDelegate::setArcherTableModel(ArcherTableModel* model) {
    _archerTableModel = model;
}
