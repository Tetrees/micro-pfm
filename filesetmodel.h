#ifndef FILESETMODEL_H
#define FILESETMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QMimeData>
#include <QUrl>
#include <QStringList>
#include "projectitem.h"
#include "constDefinitions.h"

#define FILE_BROWSER_COL_NUMBER 3

class FileSetModel : public QAbstractTableModel
{
   Q_OBJECT
public:
    FileSetModel(ProjectItem *projectItem, QObject *parent = 0);
    ~FileSetModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role= Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertRows(int row, int count, const QModelIndex & parent, QList<FileInfoItem *> *fileInfoList );
    bool insertRows(int row, int count, const QModelIndex & parent);
    bool removeRows(int row, int count, const QModelIndex &index = QModelIndex());
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    bool dropMimeData( const QMimeData *data, Qt::DropAction action,
                       int row, int column, const QModelIndex &parent);
    void setFilesData(ProjectItem *projectItem);
    int fileInfoListSize();
    QString projectFolder();
    void resetModel();

private:
    ProjectItem *m_ProjectItem;
    int m_FileInfoListSize;

    void addFile(FileInfoItem *fileInfoItem);
    void addFile();
    void removeFile(int row);
};

#endif // FILESETMODEL_H
