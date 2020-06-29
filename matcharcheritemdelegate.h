#ifndef MATCHARCHERITEMDELEGATE_H
#define MATCHARCHERITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include "matchtablemodel.h"

class MatchArcherItemDelegate : public QStyledItemDelegate
{

    Q_OBJECT

public:
    MatchArcherItemDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void setMatchTableModel(MatchTableModel* model);
    ~MatchArcherItemDelegate();

private:
    MatchTableModel* _matchTableModel;


};

#endif // MATCHARCHERITEMDELEGATE_H
