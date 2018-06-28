#include "filesetproxymodel.h"

FileSetProxyModel::FileSetProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void FileSetProxyModel::setFilteringColumns(int filterColumn)
{
    filter = filterColumn;
}

bool FileSetProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, FilesColFiles, sourceParent);//File
    QModelIndex index1 = sourceModel()->index(sourceRow, FilesColDate, sourceParent);//Date
    QModelIndex index2 = sourceModel()->index(sourceRow, FilesColComments, sourceParent);//Comment

    //Filter List: "All" << "File" << "Date" <<  Comment
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

bool FileSetProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QString leftData = sourceModel()->data(left).toString();
    QString rightData = sourceModel()->data(right).toString();

    if (left.column() == FilesColDate) //if data is Date
    {
        QDate leftDate = QDate::fromString(leftData, "dd/MM/yyyy");
        QDate rightDate = QDate::fromString(rightData, "dd/MM/yyyy");
        return leftDate < rightDate;
    }

    return QString::localeAwareCompare(leftData, rightData) < 0;
}
