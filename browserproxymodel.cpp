#include "browserproxymodel.h"

BrowserProxyModel::BrowserProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void BrowserProxyModel::setFilteringColumns(int filterColumn)
{
    filter = filterColumn;
}


bool BrowserProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, BrowserColProject, sourceParent);//ProjectName
    QModelIndex index1 = sourceModel()->index(sourceRow, BrowserColCustomer, sourceParent);//Customer
    QModelIndex index2 = sourceModel()->index(sourceRow, BrowserColProjectFolder, sourceParent);//ProjectFolder

    //Filter List: "All" << "Project" << "Customer" << "Project Folder"
    bool filterResult = false;

    switch(filter)
    {
    case 0:
        filterResult = sourceModel()->data(index0).toString().contains(filterRegExp())
                       || sourceModel()->data(index1).toString().contains(filterRegExp())
                       || sourceModel()->data(index2).toString().contains(filterRegExp());
        break;
    case 1:
        filterResult = sourceModel()->data(index0).toString().contains(filterRegExp());
        break;
    case 2:
        filterResult = sourceModel()->data(index1).toString().contains(filterRegExp());
        break;
    case 3:
        filterResult = sourceModel()->data(index2).toString().contains(filterRegExp());
        break;
    }
    return filterResult;
}

bool BrowserProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QString leftData = sourceModel()->data(left).toString();
    QString rightData = sourceModel()->data(right).toString();

    return QString::localeAwareCompare(leftData, rightData) < 0;
}
