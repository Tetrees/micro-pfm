#ifndef BROWSERMODEL_H
#define BROWSERMODEL_H

#include <Qt>
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>
#include <QStringList>
#include <QDebug>
#include "projectslist.h"
#include "constDefinitions.h"

#define BROWSER_COL_NUMBER 3

class BrowserModel : public QAbstractTableModel
{
   Q_OBJECT
public:
    BrowserModel(ProjectsList *projectList, QObject *parent = 0);
    ~BrowserModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertRows(int row, int count, const QModelIndex & parent, ProjectItem *projectItem );
    bool insertRows(int row, int count, const QModelIndex & parent);
    bool removeRows(int row, int count, const QModelIndex &index = QModelIndex());
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    bool dropMimeData( const QMimeData *data, Qt::DropAction action,
                       int row, int column, const QModelIndex &parent);
    Qt::DropAction supportedDropActions();
    void setProjectData(ProjectsList *projectsList);
    void addProject(ProjectItem *projectItem);
    void addProject();
    void removeProject(int row);
    int projectsListSize();
    ProjectItem* projectItemAt(int itemNumber);
    void updateBrowserModel(QDomDocument xmlDocument);

private:
    ProjectsList *m_ProjectsList;
    int m_ProjectsListSize;
};

#endif // BROWSERMODEL_H
