#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "archers/archertablemodel.h"

class ComboboxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ComboboxItemDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void setArcherTableModel(ArcherTableModel* model);

private:

    ArcherTableModel* _archerTableModel;

};

#endif // COMBOBOXITEMDELEGATE_H
