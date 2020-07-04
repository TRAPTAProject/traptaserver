#include "matchmodeitemdelegate.h"
#include <QComboBox>

MatchModeItemDelegate::MatchModeItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}


void MatchModeItemDelegate::setMatchTableModel(MatchTableModel* model) {
    _matchTableModel = model;
}


QWidget* MatchModeItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (_matchTableModel==0) return 0;
    QComboBox* combo = new QComboBox(parent);
    combo->addItem("Sets");
    combo->addItem("Points");
    int mode = _matchTableModel->data(index, Qt::UserRole).toInt();
    combo->setCurrentIndex(mode);
    return combo;
}

void MatchModeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (_matchTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;
    combobox->setCurrentIndex(_matchTableModel->data(index, Qt::UserRole).toInt());
}

void MatchModeItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void MatchModeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (_matchTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;
    _matchTableModel->setData(index, QVariant(combobox->currentIndex()));
}

MatchModeItemDelegate::~MatchModeItemDelegate()
{

}

