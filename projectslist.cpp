#include "projectslist.h"

ProjectsList::ProjectsList()
{
}

ProjectsList::~ProjectsList()
{
    clearProjectsList();
}

void ProjectsList::appendProjectItem()
{
    ProjectItem *projectItem = new ProjectItem;
    m_ProjectsList.append(projectItem);
}

void ProjectsList::appendProjectItem(ProjectItem *projectItem)
{
    m_ProjectsList.append(projectItem);
}

void ProjectsList::deleteProjectItem(int itemNumber)
{
    if(itemNumber < m_ProjectsList.size())
    {
        //qDebug() << "ProjectsList::deleteProjectItem - m_ProjectsList.size()" << m_ProjectsList.size();
        //qDebug() << "ProjectsList::deleteProjectItem - itemNumber" << itemNumber;
        delete m_ProjectsList.at(itemNumber);
        m_ProjectsList.removeAt(itemNumber);
    }else
        qDebug() << "Function <deleteProjectItem>: ItemNumber excceds m_ProjectsList size.";
}

void ProjectsList::listToXml(QDomDocument &domDocument)
{
    //QDomDocument domDocument;
    QDomElement rootItem =  domDocument.createElement("Projects");
    domDocument.appendChild(rootItem);

    int projectsListSize  = m_ProjectsList.size();
    for (int i=0; i < projectsListSize; i++)
    {
        QDomElement currentElement = domDocument.createElement("Project");
        rootItem.appendChild(currentElement);
        addElement("ProjectName", m_ProjectsList.at(i)->projectName(), currentElement ,domDocument);
        addElement("Customer", m_ProjectsList.at(i)->customer(), currentElement ,domDocument);
        addElement("ProjectFolder", m_ProjectsList.at(i)->projectFolder(), currentElement ,domDocument);

        QDomElement currentFileSetElement = domDocument.createElement("FileSet");
        currentElement.appendChild(currentFileSetElement);
        int fileSetListSize = m_ProjectsList.at(i)->fileInfoListSize();

        for (int j=0; j < fileSetListSize; j++)
        {
            addElement("FileName", m_ProjectsList.at(i)->fileInfoListItem(FileInfo::File, j), currentFileSetElement ,domDocument);
            addElement("Date", m_ProjectsList.at(i)->fileInfoListItem(FileInfo::Date, j), currentFileSetElement ,domDocument);
            //addElement("Type", m_ProjectsList.at(i)->fileInfoListItem(FileInfo::Type, j), currentFileSetElement ,domDocument);
            addElement("Comment", m_ProjectsList.at(i)->fileInfoListItem(FileInfo::Comment, j), currentFileSetElement ,domDocument);
        }
    }
    //return domDocument;
}

void ProjectsList::setListFromXml(const QDomDocument &domDocument)
{
    QDomElement rootItem;
    QDomNodeList projectsList;
    int projectsListSize;
    QDomNodeList fileSetList;
    int  fileSetListSize;

    rootItem = domDocument.firstChildElement();
    projectsList = rootItem.elementsByTagName("Project");
    projectsListSize = projectsList.size();

    for (int i=0; i < projectsListSize; i++)
    {
        QDomNode currentNode = projectsList.at(i);

        QString text = currentNode.firstChildElement("ProjectName").text();
        m_ProjectsList.at(i)->setProjectName(text);

        text = currentNode.firstChildElement("Customer").text();
        m_ProjectsList.at(i)->setCustomer(text);

        text = currentNode.firstChildElement("ProjectFolder").text();
        m_ProjectsList.at(i)->setProjectFolder(text);

        fileSetList = currentNode.toElement().elementsByTagName("FileSet");
        fileSetListSize = fileSetList.size();

        for (int j=0; j < fileSetListSize; j++)
        {
            FileInfoItem *fileInfoItem = new FileInfoItem;
            QDomNode currentNode = fileSetList.at(j);

            QString text = currentNode.firstChildElement("FileName").text();
            fileInfoItem->setFileName(text);

            text = currentNode.firstChildElement("Date").text();
            QDate date;
            date.fromString(text);
            fileInfoItem->setDate(date);

            text = currentNode.firstChildElement("Comment").text();
            fileInfoItem->setComment(text);

            m_ProjectsList.at(i)->appendFileInfoListItem(fileInfoItem);
        }
    }
}

int ProjectsList::getProjectsListSize()
{
    return m_ProjectsList.size();
}

ProjectItem *ProjectsList::projectItemAt(int itemNumber)
{
    return m_ProjectsList.at(itemNumber);
}

void ProjectsList::clearProjectsList()
{
    int listSize;
    listSize = m_ProjectsList.size();

    for(int i = listSize; i > 0 ; i--)
    {
        deleteProjectItem(i);
        qDebug() << "<clearProjectsList> : deleted item in the m_ProjectsList:" << i;
    }
}

void ProjectsList::addElement(QString tagString, QString textString, QDomElement &parentElement,QDomDocument &domDocument)
{
    QDomElement element = domDocument.createElement(tagString);
    QDomText textNode = domDocument.createTextNode(textString);

    parentElement.appendChild(element);
    element.appendChild(textNode);
}

