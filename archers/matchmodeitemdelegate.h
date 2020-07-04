#ifndef MATCHMODEITEMDELEGATE_H
#define MATCHMODEITEMDELEGATE_H

#include <QAbstractItemDelegate>
#include <QStyledItemDelegate>
#include "matchtablemodel.h"

class MatchModeItemDelegate : public QStyledItemDelegate
{

    Q_OBJECT

public:
    MatchModeItemDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void setMatchTableModel(MatchTableModel* model);
    ~MatchModeItemDelegate();

private:
    MatchTableModel* _matchTableModel;


};

#endif // MATCHMODEITEMDELEGATE_H
