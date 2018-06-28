#ifndef FILESETPROXYMODEL_H
#define FILESETPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>
#include "constDefinitions.h"

class FileSetProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FileSetProxyModel(QObject *parent = 0);
    void setFilteringColumns(int filterColumn);

private:
    int filter;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

};

#endif // FILESETPROXYMODEL_H
