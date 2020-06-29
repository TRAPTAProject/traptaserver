#ifndef LINEEDITITEMDELEGATE_H
#define LINEEDITITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "archertablemodel.h"

class LineEditItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LineEditItemDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void setArcherTableModel(ArcherTableModel* model);

private:

    ArcherTableModel* _archerTableModel;
};

#endif // LINEEDITITEMDELEGATE_H
