#include "projectitem.h"

ProjectItem::ProjectItem()
{
    m_ProjectName.clear();
    m_Customer.clear();
    m_ProjectFolder.clear();
}

ProjectItem::~ProjectItem()
{
    int listSize;
    listSize = m_FileInfoList.size();

    for(int i = listSize-1; i >= 0 ; i--)
    {
        deleteFileInfoListItem(i);
        qDebug() << "<ProjectItem> destructor: deleted item in the m_FileInfoList:" << i;
    }
}

void ProjectItem::setProjectName(QString projectName)
{
    m_ProjectName = projectName;
}

void ProjectItem::setCustomer(QString customer)
{
    m_Customer = customer;
}

void ProjectItem::setProjectFolder(QString projectFolder)
{
    m_ProjectFolder = projectFolder;
}

void ProjectItem::setFileInfoListItem(QString data, FileInfo::Option dataType, int itemNumber)
{
    if(itemNumber <= m_FileInfoList.size())
    {
        switch(dataType)
        {
        case FileInfo::File:
            m_FileInfoList.at(itemNumber)->setFileName(data);
            break;
        case FileInfo::Comment:
            m_FileInfoList.at(itemNumber)->setComment(data);
            break;
        case FileInfo::Date:
            QDate date;
            m_FileInfoList.at(itemNumber)->setDate(date.fromString(data));
        };
    }else
        qDebug()<< "Function <setFileInfoListItem> :itemNumber excceds m_FileInfoList size.";
}

QString ProjectItem::projectName()
{
    return m_ProjectName;
}

QString ProjectItem::customer()
{
    return m_Customer;
}

QString ProjectItem::projectFolder()
{
    return m_ProjectFolder;
}

QString ProjectItem::fileInfoListItem(FileInfo::Option dataType, int itemNumber)
{
    if(itemNumber <= m_FileInfoList.size())
    {
        switch(dataType)
        {
        case FileInfo::File:
            return m_FileInfoList.at(itemNumber)->fileName();
        case FileInfo::Comment:
            return m_FileInfoList.at(itemNumber)->comment();
        case FileInfo::Date:
            return m_FileInfoList.at(itemNumber)->date().toString();
        };
    }
    qDebug() << "Function <fileInfoListItem>: ItemNumber excceds m_FileInfoList size.";
    return QString();
}

int ProjectItem::fileInfoListSize()
{
    return m_FileInfoList.size();
}

void ProjectItem::appendFileInfoListItem()
{
    FileInfoItem *fileInfoItem = new FileInfoItem;
    m_FileInfoList.append(fileInfoItem);
}

void ProjectItem::appendFileInfoListItem(FileInfoItem *fileInfoItem)
{
    m_FileInfoList.append(fileInfoItem);
}

void ProjectItem::deleteFileInfoListItem(int itemNumber)
{
    if(itemNumber <= m_FileInfoList.size())
    {
        delete m_FileInfoList.at(itemNumber);
        m_FileInfoList.removeAt(itemNumber);
    }else
        qDebug() << "Function <deleteFileInfoListItem>: ItemNumber excceds m_FileInfoList size.";
}

