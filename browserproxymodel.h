#ifndef BROWSERPROXYMODEL_H
#define BROWSERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "constDefinitions.h"

class BrowserProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    BrowserProxyModel(QObject *parent = 0);
    void setFilteringColumns(int filterColumn);

private:
    int filter;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

};

#endif // BROWSERPROXYMODEL_H
