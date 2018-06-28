#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QList>
#include <QDate>
#include <QString>
#include <QDebug>
#include "fileinfoitem.h"
#include "FileInfo.h"

class ProjectItem
{
public:
    ProjectItem();
    ~ProjectItem();

    //setters
    void setProjectName(QString projectName);
    void setCustomer(QString customer);
    void setProjectFolder(QString projectFolder);
    void setFileInfoListItem(QString data, FileInfo::Option dataType, int itemNumber);

    //getters
    QString projectName();
    QString customer();
    QString projectFolder();
    QString fileInfoListItem(FileInfo::Option dataType, int itemNumber);
    int fileInfoListSize();

    void appendFileInfoListItem();
    void appendFileInfoListItem(FileInfoItem *fileInfoItem);
    void deleteFileInfoListItem(int itemNumber);

private:
    QString m_ProjectName;
    QString m_Customer;
    QString m_ProjectFolder;
    QList<FileInfoItem *> m_FileInfoList;
};

#endif // PROJECTITEM_H
