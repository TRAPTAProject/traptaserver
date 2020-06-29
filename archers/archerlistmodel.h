#ifndef ARCHERLISTMODEL_H
#define ARCHERLISTMODEL_H

#include <QObject>
#include "spreadsheet/spreadsheet.h"

class DataModel;
class Archer;


class ArcherListModel : public SpreadSheet
{
    Q_OBJECT

public:
    explicit ArcherListModel(DataModel* dataModel);
    ~ArcherListModel();


public slots:
    void setCurrentShootId(int shootId);
    void requestCheckedChange(int rowIndex, int columnIndex, bool checked);
    void requestComboIndexChange(int rowIndex, int columnIndex, int index);
    void requestTextChange(int rowIndex, int columnIndex, const QString &text);
    void sortByColumn(int index, bool asc);

protected:
    void requestAction(int rowIndex, int columnIndex);
    int rowCount() const;

    QString text(int rowIndex, int columnIndex) const;
    bool checked(int rowIndex, int columnIndex) const;

private:

    DataModel* _dataModel;
    int _currentShootId;
    QList<Archer*> _archerList;


};

#endif // ARCHERLISTMODEL_H
