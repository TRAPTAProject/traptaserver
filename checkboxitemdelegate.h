#ifndef CHECKBOXITEMDELEGATE_H
#define CHECKBOXITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "archers/archertablemodel.h"

class CheckboxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CheckboxItemDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void setArcherTableModel(ArcherTableModel* model);

private:
    ArcherTableModel* _archerTableModel;
};

#endif // CHECKBOXITEMDELEGATE_H
