#include "filesetmodel.h"

FileSetModel::FileSetModel(ProjectItem *projectItem, QObject *parent)
    :QAbstractTableModel(parent)
{
     setFilesData(projectItem);
}

FileSetModel::~FileSetModel()
{
    qDebug() << "Inside <FileSetModel> destructor";

    for(int i = 0; i < m_FileInfoListSize; i++)
    {
        qDebug() << "<FileSetModel> destructor: Before delete item in the m_ProjectItem: " << i;
        //qDebug() << "<FileSetModel> destructor: m_FileInfoListSize = "<< m_FileInfoListSize;
        m_ProjectItem->deleteFileInfoListItem(0);
        qDebug() << "<FileSetModel> destructor: After to delete item in the m_ProjectItem: " << i;
    }
}

int FileSetModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_FileInfoListSize;
}

int FileSetModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return FILE_BROWSER_COL_NUMBER;
}

QVariant FileSetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole || role == Qt::EditRole || role==Qt::ToolTipRole)
    {
        switch(col)
        {
        case FilesColFiles:
            return m_ProjectItem->fileInfoListItem(FileInfo::File, row); //File/Folder
        case FilesColDate:
        {
            QString dateString = m_ProjectItem->fileInfoListItem(FileInfo::Date, row);
            QDate date  = QDate::fromString(dateString);
            return date.toString("dd/MM/yyyy"); //Date
        }
        case FilesColComments:
            return m_ProjectItem->fileInfoListItem(FileInfo::Comment, row); //Comment
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        switch(col)
        {
        case FilesColComments:
            return Qt::AlignRight + Qt::AlignVCenter;
        case FilesColDate:
            return Qt::AlignCenter;
        }
    }

    return  QVariant();
}

bool FileSetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;

    int row = index.row();
    int col = index.column();

    if (role == Qt::EditRole)
    {
        switch(col)
        {
        case FilesColFiles:
        {
            m_ProjectItem->setFileInfoListItem(value.toString(),FileInfo::File, row);
            QFileInfo fileInfo(value.toString());
            QDateTime fileModDate;
            fileModDate = fileInfo.created();
            qDebug()<< "fileModDate.toString() = "<< fileModDate.toString();
            m_ProjectItem->setFileInfoListItem(fileModDate.date().toString(),FileInfo::Date, row);
            emit dataChanged(index,index);
            return true;
        }
        case FilesColDate:
            m_ProjectItem->setFileInfoListItem(value.toString(),FileInfo::Date, row);
            emit dataChanged(index,index);
            return true;
        case FilesColComments:
            m_ProjectItem->setFileInfoListItem(value.toString(),FileInfo::Comment, row);
            emit dataChanged(index,index);
            return true;
        }
    }
    return FileSetModel::setData(index, value, role);
}

bool FileSetModel::insertRows(int row, int count, const QModelIndex &parent, QList<FileInfoItem *> *fileInfoList)
{
    Q_UNUSED(parent);

    if(count < 1)
        return false;

    layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), row, row+count-1);

    for(int i = 0;  i < count; i++)
    {
        addFile(fileInfoList->at(i));
    }

    endInsertRows();
    layoutChanged();
    return true;
}

bool FileSetModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if(count < 1)
        return false;

    layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), row, row+count-1);

    for(int i = 0;  i < count; i++)
    {
        addFile();
    }

    endInsertRows();
    layoutChanged();
    return true;
}

bool FileSetModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);

    if(count < 1)
        return false;

    layoutAboutToBeChanged();
    beginRemoveRows(QModelIndex(), row, row+count-1);

    for(int i=0; i<count; i++)
    {
        removeFile(row+i);
    }

    endRemoveRows();
    layoutChanged();
    return true;
}

QVariant FileSetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role ==Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case FilesColFiles:
                return QString("File/Folder");
            case FilesColDate:
                return QString("Date");
            case FilesColComments:
                return QString("Comment");
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags FileSetModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;
    if (index.column() == FilesColFiles)//drop only for files column
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QStringList FileSetModel::mimeTypes() const
{
    QStringList types;
    types << "text/uri-list";
    return types;
}

bool FileSetModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    qDebug() << "FileSetModel::dropMimeData: parent.row()=" << parent.row();
    qDebug() << "FileSetModel::dropMimeData: parent.column()=" << parent.column();
    qDebug() << "FileSetModel::dropMimeData: row=" << row;
    qDebug() << "FileSetModel::dropMimeData: column=" << column;

    if (!data->hasUrls())
        return false;

    QList<QUrl> droppedUrls = data->urls();
    QStringList filePathInfoList;
    QString localPath;
    QFileInfo fileInfo;
    int rowToInsert, columnToInsert;

    if(parent.column() > -1 && droppedUrls.size() == 1) //only one url droped on files column
    {
        localPath = droppedUrls[0].toLocalFile();
        fileInfo.setFile(localPath);
        QModelIndex idx = index(parent.row(), parent.column(), QModelIndex());
        setData(idx, fileInfo.absoluteFilePath());
        return true;
    }

    for(int i = 0; i < droppedUrls.size(); i++)
    {
        localPath = droppedUrls[i].toLocalFile();
        fileInfo.setFile(localPath);
        filePathInfoList.append(fileInfo.absoluteFilePath());


        rowToInsert = fileInfoListSize();
        qDebug() << "FileSetModel::dropMimeData: fileInfoListSize=" << fileInfoListSize();
        columnToInsert = FilesColFiles;
        insertRows(rowToInsert, 1, QModelIndex());

        QModelIndex idx = index(rowToInsert, columnToInsert, QModelIndex());
        setData(idx, filePathInfoList.at(i));
    }
    return true;
}

void FileSetModel::setFilesData(ProjectItem *projectItem)
{
    layoutAboutToBeChanged();
    m_ProjectItem = projectItem;
    m_FileInfoListSize = m_ProjectItem->fileInfoListSize();
    layoutChanged();
}

int FileSetModel::fileInfoListSize()
{
    return m_FileInfoListSize;
}

QString FileSetModel::projectFolder()
{
    return m_ProjectItem->projectFolder();
}

void FileSetModel::resetModel()
{
    this->layoutAboutToBeChanged();
    this->beginResetModel();

    m_FileInfoListSize = 0;

    this->endResetModel();
    this->layoutChanged();
}


void FileSetModel::addFile(FileInfoItem *fileInfoItem)
{
    m_ProjectItem->appendFileInfoListItem(fileInfoItem);
    m_FileInfoListSize = m_ProjectItem->fileInfoListSize();
}

void FileSetModel::addFile()
{
    m_ProjectItem->appendFileInfoListItem();
    m_FileInfoListSize = m_ProjectItem->fileInfoListSize();
}

void FileSetModel::removeFile(int row)
{
    m_ProjectItem->deleteFileInfoListItem(row);
    m_FileInfoListSize = m_ProjectItem->fileInfoListSize();
}
