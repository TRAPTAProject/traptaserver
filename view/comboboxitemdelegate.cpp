#include "comboboxitemdelegate.h"
#include <QComboBox>
#include <QDebug>
#include "utils/util.h"

ComboboxItemDelegate::ComboboxItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}


void ComboboxItemDelegate::setArcherTableModel(ArcherTableModel* model) {
    _archerTableModel = model;
}


QWidget* ComboboxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QComboBox* combo = new QComboBox(parent);
    QStringList list;
    for (int i=0; i<280; i++) list << Util::targetLabelFromPosition(i);
    combo->addItems(list);
    return combo;
}

void ComboboxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (_archerTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;
    combobox->setCurrentIndex(_archerTableModel->data(index, Qt::UserRole).toInt());
}

void ComboboxItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void ComboboxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (_archerTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;
    _archerTableModel->setData(index, QVariant(combobox->currentIndex()));
}
