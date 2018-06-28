#ifndef PROJECTSLIST_H
#define PROJECTSLIST_H

#include <QList>
#include <QString>
#include <QDomDocument>
#include <QDebug>
#include "projectitem.h"

class ProjectsList
{
public:
    ProjectsList();
    ~ProjectsList();
    void appendProjectItem();
    void appendProjectItem(ProjectItem *projectItem);
    void deleteProjectItem(int itemNumber);
    void listToXml(QDomDocument &domDocument);
    void setListFromXml(const QDomDocument &domDocument);
    int getProjectsListSize();
    ProjectItem *projectItemAt(int itemNumber);

private:
    QList<ProjectItem *> m_ProjectsList;
    void clearProjectsList();
    void addElement(QString tagString, QString textString, QDomElement &parentElement,QDomDocument &domDocument);
};

#endif // PROJECTSLIST_H
