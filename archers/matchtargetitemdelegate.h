#ifndef MATCHTARGETITEMDELEGATE_H
#define MATCHTARGETITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include "matchtablemodel.h"

class MatchTargetItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    MatchTargetItemDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void setMatchTableModel(MatchTableModel* model);
    ~MatchTargetItemDelegate();

private:
    MatchTableModel* _matchTableModel;

};

#endif // MATCHTARGETITEMDELEGATE_H
