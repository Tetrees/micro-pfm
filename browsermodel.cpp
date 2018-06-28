#include "browsermodel.h"

BrowserModel::BrowserModel(ProjectsList *projectList, QObject *parent)
    :QAbstractTableModel(parent)
{
    setProjectData(projectList);
}

BrowserModel::~BrowserModel()
{
    qDebug() << "Inside <BrowserModel> destructor: m_ProjectsListSize=" << m_ProjectsListSize;

    int tempProjectsListSize = m_ProjectsListSize;
    for(int row = 0; row < tempProjectsListSize; row++)
    {
        removeProject(0);
    }
}

int BrowserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_ProjectsListSize;
}

int BrowserModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return BROWSER_COL_NUMBER;
}

QVariant BrowserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole || role == Qt::EditRole || role==Qt::ToolTipRole)
    {
        switch(col)
        {
        case BrowserColProject:
            return m_ProjectsList->projectItemAt(row)->projectName(); //Project
        case BrowserColCustomer:
            return m_ProjectsList->projectItemAt(row)->customer(); //Customer
        case BrowserColProjectFolder:
            return m_ProjectsList->projectItemAt(row)->projectFolder(); //ProjectFolder
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        switch(col)
        {
        case BrowserColProject:
            return Qt::AlignRight + Qt::AlignVCenter;
        case BrowserColCustomer:
            return Qt::AlignRight + Qt::AlignVCenter;
        }
    }
    return  QVariant();
}

bool BrowserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;

    int row = index.row();
    int col = index.column(); //0=project; 1=customer; 2=project folder

    if (role == Qt::EditRole)
    {
        switch(col)
        {
        case BrowserColProject:
            m_ProjectsList->projectItemAt(row)->setProjectName(value.toString());

            emit dataChanged(index,index);
            return true;
        case BrowserColCustomer:
            m_ProjectsList->projectItemAt(row)->setCustomer(value.toString());
            emit dataChanged(index,index);
            return true;
        case BrowserColProjectFolder:
            m_ProjectsList->projectItemAt(row)->setProjectFolder(value.toString());
            emit dataChanged(index,index);
            return true;
        }
    }
    return BrowserModel::setData(index, value, role);
}

bool BrowserModel::insertRows(int row, int count, const QModelIndex &parent, ProjectItem *projectItem)
{
    Q_UNUSED(parent);
    if(count < 1)
        return false;

    layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), row, row+count-1);
    addProject(projectItem);
    endInsertRows();
    layoutChanged();
    return true;
}

bool BrowserModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(count < 1)
        return false;

    layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), row, row+count-1);
    addProject();
    endInsertRows();
    layoutChanged();
    return true;
}

bool BrowserModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    if(count < 1)
        return false;

    layoutAboutToBeChanged();
    beginRemoveRows(QModelIndex(), row, row+count-1);

    for(int i=0; i<count; i++)
    {
        removeProject(row+i);
    }

    endRemoveRows();
    layoutChanged();
    return true;
}

QVariant BrowserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role ==Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case BrowserColProject:
                return QString("Project");
            case BrowserColCustomer:
                return QString("Customer");
            case BrowserColProjectFolder:
                return QString("Project Folder");
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags BrowserModel::flags(const QModelIndex &index) const
{
    //qDebug() << "inside BrowserModel::flags row=" << index.row();
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;
    if (index.column()==BrowserColProjectFolder)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QStringList BrowserModel::mimeTypes() const
{
    QStringList types;
    types << "text/uri-list";
    return types;
}

bool BrowserModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    row=parent.row();
    column=parent.column();

    qDebug() << "Inside BrowserModel::dropMimeData - column:" << column;
    qDebug() << "Inside BrowserModel::dropMimeData - row:" << row;

    if (!data->hasUrls())
        return false;

    QList<QUrl> droppedUrls = data->urls();

    if (droppedUrls.size() > 1)
        return false;

    QString localPath = droppedUrls[0].toLocalFile();///if dropped more than 1 items, others are ignored
    QFileInfo fileInfo(localPath);
    QString filePathInfo = fileInfo.absoluteFilePath();
    qDebug() << "Inside BrowserModel::dropMimeData - filePathInfo:" << filePathInfo;

    if(column == -1)
    {
        row = projectsListSize();
        column = BrowserColProjectFolder;
        insertRows(row, 1, QModelIndex());
    }

    QModelIndex idx = index(row, column, QModelIndex());
    setData(idx, filePathInfo);

    return true;
}

Qt::DropAction BrowserModel::supportedDropActions()
{
    return Qt::CopyAction;
}

void BrowserModel::setProjectData(ProjectsList *projectsList)
{
    m_ProjectsList = projectsList;
    m_ProjectsListSize = projectsList->getProjectsListSize();
}

void BrowserModel::addProject(ProjectItem *projectItem)
{
    m_ProjectsList->appendProjectItem(projectItem);
    m_ProjectsListSize = m_ProjectsList->getProjectsListSize();
}

void BrowserModel::addProject()
{
    m_ProjectsList->appendProjectItem();
    m_ProjectsListSize = m_ProjectsList->getProjectsListSize();
}

void BrowserModel::removeProject(int row)
{
    m_ProjectsList->deleteProjectItem(row);
    m_ProjectsListSize = m_ProjectsList->getProjectsListSize();
}

int BrowserModel::projectsListSize()
{
    return m_ProjectsListSize;
}

ProjectItem *BrowserModel::projectItemAt(int itemNumber)
{
    return m_ProjectsList->projectItemAt(itemNumber);
}

void BrowserModel::updateBrowserModel(QDomDocument xmlDocument)
{
    QString nodeText;
    QDomElement xmlRoot = xmlDocument.firstChildElement();

    //read projects
    QDomNodeList projects = xmlRoot.elementsByTagName("ProjectItem");

    layoutAboutToBeChanged();

    beginResetModel();
    int tempProjectsListSize = m_ProjectsListSize;
    for(int row = 0; row < tempProjectsListSize; row++)
    {
        removeProject(0);
        qDebug() << "BrowserModel::updateBrowserModel row:" << row;
    }
    endResetModel();

    for(int i = 0; i < projects.count(); i++)
    {
        ProjectItem *newItem = new ProjectItem();

        nodeText = projects.at(i).toElement().firstChildElement("ProjectName").text();
        newItem->setProjectName(nodeText);

        nodeText = projects.at(i).toElement().firstChildElement("Customer").text();
        newItem->setCustomer(nodeText);

        nodeText = projects.at(i).toElement().firstChildElement("ProjectFolder").text();
        newItem->setProjectFolder(nodeText);

        insertRows(i ,1,QModelIndex(),newItem);

        QDomNodeList fileSetList = projects.at(i).toElement().firstChildElement("FileSetList").elementsByTagName("FileSet");
        qDebug() << "fileSetListSize=" << fileSetList.count();

        for(int j=0; j < fileSetList.count(); j++)
        {
            FileInfoItem *fileInfoItem = new FileInfoItem;

            nodeText = fileSetList.at(j).toElement().firstChildElement("File").text();
            fileInfoItem->setFileName(nodeText);
            //qDebug() << "File=" << j <<" name="<< nodeText;

            nodeText = fileSetList.at(j).toElement().firstChildElement("Date").text();
            QDate date = QDate::fromString(nodeText, Qt::TextDate);
            fileInfoItem->setDate(date);

            nodeText = fileSetList.at(j).toElement().firstChildElement("Comment").text();
            fileInfoItem->setComment(nodeText);

            projectItemAt(i)->appendFileInfoListItem(fileInfoItem);
        }
    }

    layoutChanged();
}


