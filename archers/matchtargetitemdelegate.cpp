#include "matchtargetitemdelegate.h"
#include <QComboBox>

MatchTargetItemDelegate::MatchTargetItemDelegate(QObject *parent) :
                                                 QStyledItemDelegate(parent)
{

}



void MatchTargetItemDelegate::setMatchTableModel(MatchTableModel* model) {
    _matchTableModel = model;
}


QWidget* MatchTargetItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QComboBox* combo = new QComboBox(parent);
    QStringList list;
    list << "---";
    for (int i=1; i<80; i++) list << QString::number(i);
    combo->addItems(list);
    return combo;
}

void MatchTargetItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (_matchTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;
    combobox->setCurrentIndex(_matchTableModel->data(index, Qt::UserRole).toInt());
}

void MatchTargetItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void MatchTargetItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (_matchTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;
    _matchTableModel->setData(index, QVariant(combobox->currentIndex()));
}


MatchTargetItemDelegate::~MatchTargetItemDelegate()
{

}

