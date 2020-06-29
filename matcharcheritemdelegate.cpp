#include "matcharcheritemdelegate.h"
#include <QComboBox>

MatchArcherItemDelegate::MatchArcherItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}


void MatchArcherItemDelegate::setMatchTableModel(MatchTableModel* model) {
    _matchTableModel = model;
}


QWidget* MatchArcherItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QComboBox* combo = new QComboBox(parent);
    return combo;
}

void MatchArcherItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (_matchTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;

    int playerIndex = 0;
    if (index.column()==6) playerIndex = 1;
    QList<Archer*> archerList = _matchTableModel->archerComboList(index.row(), playerIndex); // get ordered archer name list
    int archerId = _matchTableModel->data(index, Qt::UserRole).toInt();
    // look for archer
    int listIndex = 0;
    int counter = 0;
    QStringList nameList;
    foreach (Archer* archer, archerList) {
        if (archer->id()==archerId) {
            listIndex = counter;
        }
        nameList << archer->name();
        counter++;
    }

    combobox->addItems(nameList);
    combobox->setCurrentIndex(listIndex);
}

void MatchArcherItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void MatchArcherItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (_matchTableModel==0) return;
    QComboBox* combobox = (QComboBox*)editor;
    int playerIndex = 0;
    if (index.column()==6) playerIndex = 1;
    QList<Archer*> archerList = _matchTableModel->archerComboList(index.row(), playerIndex); // get ordered archer name list
    _matchTableModel->setData(index, QVariant(archerList.at(combobox->currentIndex())->id()));
}


MatchArcherItemDelegate::~MatchArcherItemDelegate()
{

}

