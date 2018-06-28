#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_BrowserProxyModel = new BrowserProxyModel(this);
    m_FileSetProxyModel = new FileSetProxyModel(this);
    m_CalendarDelegate = new CalendarDelegate(this);
    m_currentFileSetRow = -1;
    m_isSaved = true;
    m_isNewXML = true;
    recentFiles.clear();
    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_BrowserProxyModel;
    delete m_FileSetProxyModel;
}

void MainWindow::initiateDialogData(BrowserModel *browserModel, FileSetModel *fileSetModel)
{
    //initiating model and filters
    m_BrowserModel = browserModel;
    m_BrowserProxyModel->setSourceModel(m_BrowserModel);
    ui->tableView_ProjectsBrowser->setModel(m_BrowserProxyModel);

    m_FileSetModel = fileSetModel;
    m_FileSetProxyModel->setSourceModel(m_FileSetModel);
    ui->tableView_FilesBrowser->setModel(m_FileSetProxyModel);
    ui->tableView_FilesBrowser->setItemDelegate(m_CalendarDelegate);

    //initiating projects browser
    ui->tableView_ProjectsBrowser->setAcceptDrops(true);
    ui->tableView_ProjectsBrowser->setDropIndicatorShown(true);

    ui->tableView_ProjectsBrowser->setColumnWidth(BrowserColProject, 250);
    ui->tableView_ProjectsBrowser->setColumnWidth(BrowserColCustomer, 250);
    ui->tableView_ProjectsBrowser->horizontalHeader()->setResizeMode(BrowserColProjectFolder, QHeaderView::Stretch);

    ui->tableView_ProjectsBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView_ProjectsBrowser, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenuProjectsBrowser(const QPoint&)));

    //initiating files browser
    ui->tableView_FilesBrowser->setAcceptDrops(true);
    ui->tableView_FilesBrowser->setDropIndicatorShown(true);

    ui->tableView_FilesBrowser->setColumnWidth(FilesColComments,250+250-90);
    ui->tableView_FilesBrowser->setColumnWidth(FilesColDate, 90);
    ui->tableView_FilesBrowser->horizontalHeader()->setResizeMode(FilesColFiles, QHeaderView::Stretch);

    ui->tableView_FilesBrowser->horizontalHeader()->setSortIndicator(FilesColDate,Qt::DescendingOrder);

    ui->tableView_FilesBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView_FilesBrowser, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenuFilesBrowser(const QPoint&)));

    //initiating filter selection comboboxes
    initiateFilterCombobox(ui->comboBox_ProjectsBrowser, SetBrowserFilter);
    initiateFilterCombobox(ui->comboBox_Files, SetFilesFilter);
    m_BrowserProxyModel->setFilteringColumns(0);
    m_FileSetProxyModel->setFilteringColumns(0);

    connect(m_BrowserModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(setFalseSave()));
    connect(m_FileSetModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(setFalseSave()));

    loadSettings();
    this->setWindowTitle(tr(PROGRAM_NAME" - %1").arg("NewXML"));
}

int MainWindow::proxyToModel(const QModelIndex &index, QSortFilterProxyModel *proxyModel)
{
    int row = -1;
    row = proxyModel->mapToSource(index).row();

    //qDebug() << "Inside: proxyToModel: index.row()=" << index.row();
    //qDebug() << "Inside: proxyToModel: proxyModel->mapToSource(index).row()=" << row;
    return row;
}

int MainWindow::modelToProxy(int modelRow, QAbstractTableModel *model, QSortFilterProxyModel *proxyModel)
{
    QModelIndex index = model->index(modelRow,0);
    int proxyRow = proxyModel->mapFromSource(index).row();

    //qDebug() << "Inside: modelToProxy: modelRow =" << modelRow;
    //qDebug() << "Inside: modelToProxy: proxyRow =" << proxyRow;
    return proxyRow;
}

void MainWindow::createActions()
{
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }
}

void MainWindow::createMenus()
{
    for (int i = 0; i < MaxRecentFiles; ++i)
        ui->fileMenu->addAction(recentFileActs[i]);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
        event->ignore();
    saveSettings();
}

void MainWindow::fetchDataFromFile(QString fileName, QStringList &fetchList)
{
    QString fileNamePath = QCoreApplication::applicationDirPath();
    QFile dataFile(fileNamePath.append(fileName));

    if(!dataFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0,"Warning","Error opening data file.");
        return;
    }
    fetchList.clear();
    QTextStream tsDataFile(&dataFile);
    tsDataFile.setCodec("UTF-8");

    while (!tsDataFile.atEnd())
    {
        QString line = tsDataFile.readLine();
        fetchList.append(line);
    }
    dataFile.close();
}

void MainWindow::saveUpdatedData(QString fileName, QStringList &fetchList)
{
    QString fileNamePath = QCoreApplication::applicationDirPath();
    QFile dataFile(fileNamePath.append(fileName));

    if(!dataFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0,"Warning","Error opening data file for writing.");
        return;
    }

    QTextStream tsDataFile(&dataFile);
    tsDataFile.setCodec("UTF-8");
    for (int i=0; i<fetchList.size(); i++)
    {
        tsDataFile << fetchList.at(i) << endl;
    }

    dataFile.close();
}

//filterNumber=1 for browser; 2 for files
void MainWindow::initiateFilterCombobox(QComboBox *filter, int filterNumber)
{
    QStringList filterList;
    switch(filterNumber)
    {
    case SetBrowserFilter:
        filterList << "All" << "Project" << "Customer" << "Project Folder";
        break;
    case SetFilesFilter:
        filterList << "All" << "File" << "Date" << "Comment";
        break;
    default:
        filterList.clear();
    };
    filter->addItems(filterList);
}

void MainWindow::addFiles()
{
    try
    {
        QList<FileInfoItem *> *tempFileInfoList = new  QList<FileInfoItem *>();
        QString projectFolder = m_FileSetModel->projectFolder();
        QStringList files = QFileDialog::getOpenFileNames(this, tr("Select files to add"),projectFolder);
        int filesCount;
        filesCount =  files.size();

        for (int i = 0; i < filesCount; i++)
        {
            FileInfoItem *fileInfoItem = new FileInfoItem;
            tempFileInfoList->append(fileInfoItem);
            tempFileInfoList->at(i)->setFileName(files.at(i));

            QFileInfo fileInfo(files.at(i));
            QDateTime fileModDate;
            fileModDate = fileInfo.created();
            tempFileInfoList->at(i)->setDate(fileModDate.date());
        }
        if(m_FileSetModel->insertRows(m_FileSetModel->rowCount(), filesCount, QModelIndex(), tempFileInfoList))
            m_isSaved = false;

        delete tempFileInfoList;
    }
    catch(...)
    {
        qDebug() << "inside  MainWindow::addFiles: exception";
    }
}

void MainWindow::removeFiles()
{
    // retrieve selectionModel, each QTableView object has one
    QItemSelectionModel *selectionModel = ui->tableView_FilesBrowser->selectionModel();

    // obtain selected items, if you have multiple selection enabled
    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;
    QList<int> rows;

    foreach(index, indexes)
    {
        rows.append(proxyToModel(index, m_FileSetProxyModel));
    }

    qSort(rows);

    for(int i = rows.size()-1; i>=0; i--)
    {
        m_FileSetModel->removeRows(rows.at(i), 1, QModelIndex());
    }
    m_isSaved = false;
}

void MainWindow::addNewProject()
{
    NewProjectDialog newProjectDialog;
    if(newProjectDialog.exec())
    {
        int newRow = m_BrowserModel->projectsListSize();
        ProjectItem *newItem = new ProjectItem();

        newItem->setProjectName(newProjectDialog.project());
        newItem->setCustomer(newProjectDialog.customer());
        newItem->setProjectFolder(newProjectDialog.projectFolder());
        if(m_BrowserModel->insertRows(newRow,1,QModelIndex(),newItem))
            m_isSaved = false;

        ui->tableView_ProjectsBrowser->selectRow(modelToProxy(newRow, m_BrowserModel, m_BrowserProxyModel));
        m_currentFileSetRow = newRow;
        setFileSetModel(m_currentFileSetRow);
    }
}

void MainWindow::removeProjects()
{
    // retrieve selectionModel, each QTableView object has one
    QItemSelectionModel *selectionModel = ui->tableView_ProjectsBrowser->selectionModel();

    // obtain selected items, if you have multiple selection enabled
    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;
    QList<int> rows;

    foreach(index, indexes)
    {
        rows.append(proxyToModel(index, m_BrowserProxyModel));
    }

    qSort(rows);

    for(int i = rows.size()-1; i>=0; i--)
    {
        m_BrowserModel->removeRows(rows.at(i), 1, QModelIndex());
    }

    int currentRow = m_BrowserModel->projectsListSize()-1;
    ui->tableView_ProjectsBrowser->selectRow(modelToProxy(currentRow, m_BrowserModel, m_BrowserProxyModel));
    m_currentFileSetRow = currentRow;
    setFileSetModel(m_currentFileSetRow);

    if(m_currentFileSetRow == -1)
    {
        m_FileSetModel->resetModel();
    }
    m_isSaved = false;
}

void MainWindow::setFileSetModel(int currentRow)
{
    if (currentRow > -1 && currentRow < m_BrowserModel->projectsListSize())
    {
        ProjectItem *projectItem = m_BrowserModel->projectItemAt(currentRow);
        m_FileSetModel->setFilesData(projectItem);
        //qDebug() << "MainWindow::setFileSetModel: currentRow=" << currentRow;
    }
    if (currentRow == -1)
    {
        m_FileSetModel->resetModel();
    }
}

void MainWindow::on_actionOpen_XML_triggered()
{
    QFileDialog loadDialog(this);
    loadDialog.setNameFilter("XML Files (*.xml)");
    QString fileName = loadDialog.getOpenFileName(this,tr("Open File"),tr("%1").arg(m_recentlyOpenedFileName),"XML Files (*.xml)");

    if(loadFile(fileName))
    {
        updateRecentFiles(fileName);
    }
}

void MainWindow::on_actionSave_XML_triggered()
{
    bool saveAs = false;
    saveFile(saveAs);
}

void MainWindow::on_actionSave_As_triggered()
{
    saveFile();
}

void MainWindow::showContextMenuProjectsBrowser(const QPoint &position)
{
    QPoint globalPosition = ui->tableView_ProjectsBrowser->mapToGlobal(position);
    QMenu rightClickMenu;
    QStringList menuList, fileName;
    menuList << "Add project" << "Remove projects"; // ...
    fileName << ":/images/Images/add.png" << ":/images/Images/delete.png"; //..

    QModelIndexList filesSelection = ui->tableView_ProjectsBrowser->selectionModel()->selectedRows();

    if(filesSelection.isEmpty())
        rightClickMenu.addAction(QIcon(fileName.at(0)), menuList.at(0));
    else
    {
        for (int i=0; i < menuList.size(); i++)
        {
            rightClickMenu.addAction(QIcon(fileName.at(i)), menuList.at(i));
        }
    }

    QAction* selectedItem = rightClickMenu.exec(globalPosition);
    if (selectedItem)
    {
        if(selectedItem->text() == menuList.at(0))
        {
            addNewProject();
        }
        else if (selectedItem->text() == menuList.at(1))
        {
            removeProjects();
        }
    }
}

void MainWindow::showContextMenuFilesBrowser(const QPoint &position)
{
    QPoint globalPosition = ui->tableView_FilesBrowser->mapToGlobal(position);
    QMenu rightClickMenu;
    QStringList menuList, fileName;
    menuList << "Show in Explorer" << "Open File" << "Add Files" <<"Delete"; // ...
    fileName << ":/images/Images/open_folder.png" << ":/images/Images/open_document.png" <<
                ":/images/Images/add.png" << ":/images/Images/delete.png"; //..

    QModelIndexList filesSelection = ui->tableView_FilesBrowser->selectionModel()->selectedRows();

    if(filesSelection.isEmpty())
    {
        if(m_currentFileSetRow > -1)
            rightClickMenu.addAction(QIcon(fileName.at(2)), menuList.at(2));
    }
    else
    {
        for (int i=0; i < menuList.size(); i++)
        {
            rightClickMenu.addAction(QIcon(fileName.at(i)), menuList.at(i));
        }
    }

    QAction* selectedItem = rightClickMenu.exec(globalPosition);
    if (selectedItem)
    {
        if(selectedItem->text() == menuList.at(0) || selectedItem->text() == menuList.at(1))
        {
            for (int i=0; i < filesSelection.size(); i++)
            {
                int row = filesSelection.at(i).row();
                QString fileNameString = ui->tableView_FilesBrowser->model()->index(row, FilesColFiles).data().toString();

                QFileInfo fileName(fileNameString);
                if (fileName.isFile() && selectedItem->text() == menuList.at(0))
                    fileNameString =  fileName.path();
                qDebug() << "fileNameString:" << fileNameString;
                bool test = QDesktopServices::openUrl(QUrl(tr("file:///%1").arg(fileNameString), QUrl::TolerantMode));
                qDebug() << "test url:" << test;
            }
        }
        else if (selectedItem->text() == menuList.at(2) && m_currentFileSetRow > -1)
        {
            addFiles();
        }
        else if (selectedItem->text() == menuList.at(3))
        {
            removeFiles();
        }
    }
}

bool MainWindow::saveXml(QString fileName)
{
    QFile outFile( fileName );
    if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        qDebug( "Failed to open file for writing." );
        return false;
    }
    QDomDocument xmlDocument = modelToXml();
    QTextStream stream( &outFile );
    stream.setCodec("UTF-8");
    stream << xmlDocument.toString();

    outFile.close();
    return true;
}

bool MainWindow::saveFile(bool saveAs)
{
    if(saveAs || m_isNewXML || m_recentlyOpenedFileName == "")
    {
        QFileDialog saveDialog(this);
        QFileInfo recentlyOpenedDirectory;
        recentlyOpenedDirectory.setFile(m_recentlyOpenedFileName);
        QString path = recentlyOpenedDirectory.path();

        m_recentlyOpenedFileName = saveDialog.getSaveFileName(this,tr("Save File"),
                                                              tr("%1/projectsDB.xml").arg(path)
                                                              ,"XML Files (*.xml)");
    }

    if(!m_recentlyOpenedFileName.isEmpty())
    {
        if ( saveXml(m_recentlyOpenedFileName))
        {
            QString msgText;
            msgText = tr("XML file was successfully saved");

            QMessageBox::information(0,"Saving status",tr("%1: <i> %2 <i>")
                                     .arg(msgText, m_recentlyOpenedFileName));
            m_isSaved = true;
            m_isNewXML = false;
        }
    }

    this->setWindowTitle(tr(PROGRAM_NAME" - %1").arg(m_recentlyOpenedFileName));
    return m_isSaved;
}

QDomDocument MainWindow::readFile(QString fileName)
{
    QFile dataFile(fileName);
    QDomDocument xmlDocument;

    if(!dataFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0,"Warning","Error opening XML file.");
        return QDomDocument();
    }

    xmlDocument.setContent(&dataFile);
    dataFile.close();
    QMessageBox::information(0,"Loading status",tr("XML file successfully loaded"));

    return xmlDocument;
}

QDomDocument MainWindow::modelToXml()
{
    QDomDocument xmlDocument;
    QDomElement xmlRoot = xmlDocument.createElement("Projects");
    xmlDocument.appendChild(xmlRoot);
    QString tempText;

    for(int row = 0; row < m_BrowserModel->projectsListSize(); ++row)
    {
        QDomElement xmlProject = xmlDocument.createElement("ProjectItem");
        xmlRoot.appendChild(xmlProject);

        tempText = m_BrowserModel->projectItemAt(row)->projectName();
        addElement("ProjectName", tempText, xmlProject, xmlDocument);

        tempText = m_BrowserModel->projectItemAt(row)->customer();
        addElement("Customer", tempText, xmlProject, xmlDocument);

        tempText = m_BrowserModel->projectItemAt(row)->projectFolder();
        addElement("ProjectFolder", tempText, xmlProject, xmlDocument);

        QDomElement xmlFileSetList = xmlDocument.createElement("FileSetList");
        xmlProject.appendChild(xmlFileSetList);

        for(int fileRow=0; fileRow < m_BrowserModel->projectItemAt(row)->fileInfoListSize(); ++fileRow)
        {
            QDomElement xmlFileSet = xmlDocument.createElement("FileSet");
            xmlFileSetList.appendChild(xmlFileSet);
            tempText = m_BrowserModel->projectItemAt(row)->fileInfoListItem(FileInfo::File, fileRow);
            addElement("File", tempText, xmlFileSet, xmlDocument);

            tempText = m_BrowserModel->projectItemAt(row)->fileInfoListItem(FileInfo::Date, fileRow);
            addElement("Date", tempText, xmlFileSet, xmlDocument);

            tempText = m_BrowserModel->projectItemAt(row)->fileInfoListItem(FileInfo::Comment, fileRow);
            addElement("Comment", tempText, xmlFileSet, xmlDocument);
        }
    }
    return xmlDocument;
}

void MainWindow::xmlToModel(QDomDocument xmlDocument)
{
    m_BrowserModel->updateBrowserModel(xmlDocument);
    m_currentFileSetRow = m_BrowserModel->projectsListSize()-1;
    ui->tableView_ProjectsBrowser->selectRow(modelToProxy(m_currentFileSetRow, m_BrowserModel, m_BrowserProxyModel));
    setFileSetModel(m_currentFileSetRow);
}

void MainWindow::addElement(QString tagString, QString textString, QDomElement project, QDomDocument &xmlDocument)
{
    QDomElement tag = xmlDocument.createElement(tagString);
    QDomText text = xmlDocument.createTextNode(textString);

    project.appendChild(tag);
    tag.appendChild(text);
}

void MainWindow::saveSettings()
{
    QSettings settings("microPFM","savingSettings");

    settings.setValue("resentlyUsedDir",m_recentlyOpenedFileName);
    settings.setValue("recentFiles", recentFiles);

    QByteArray projectsBrowserState = ui->tableView_ProjectsBrowser->horizontalHeader()->saveState();
    settings.setValue("projectsBrowserState",projectsBrowserState);

    QByteArray filesBrowserState = ui->tableView_FilesBrowser->horizontalHeader()->saveState();
    settings.setValue("filesBrowserState",filesBrowserState);

    settings.setValue("windowState", this->saveState());
    settings.setValue("windowGeometry", this->saveGeometry());
    settings.setValue( "windowMaximized", isMaximized() );
    if ( !isMaximized() ) {
        settings.setValue( "windowPos", pos() );
        settings.setValue( "windowSize", size() );
    }

}

void MainWindow::loadSettings()
{
    QSettings settings("microPFM","savingSettings");

    if(settings.contains("resentlyUsedDir"))
    {
        m_recentlyOpenedFileName = settings.value("resentlyUsedDir").toString();
    }
    if(settings.contains("recentFiles"))
    {
        recentFiles = settings.value("recentFiles").toStringList();
        recentFiles.removeDuplicates();
        updateRecentFileActions();
    }

    if(settings.contains("windowState") && settings.contains("windowGeometry") //&& settings.contains("windowPos")
            && settings.contains("windowSize") && settings.contains("windowMaximized"))
    {
        restoreState(settings.value("windowState").toByteArray());
        restoreState(settings.value("windowGeometry").toByteArray());
        //move(settings.value( "windowPos", pos() ).toPoint());
        resize(settings.value( "windowSize", size() ).toSize());
        if ( settings.value( "windowMaximized", isMaximized() ).toBool() )
            showMaximized();
    }

    if(settings.contains("projectsBrowserState"))
        ui->tableView_ProjectsBrowser->horizontalHeader()->restoreState(settings.value("projectsBrowserState").toByteArray());

    if(settings.contains("filesBrowserState"))
        ui->tableView_FilesBrowser->horizontalHeader()->restoreState(settings.value("filesBrowserState").toByteArray());
}

bool MainWindow::maybeSave()
{
    if (!m_isSaved) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr(PROGRAM_NAME),
                                   tr("The data has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            bool saveAs = false;
            return saveFile(saveAs);
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::on_pushButton_AddNewProject_clicked()
{
    addNewProject();
}

void MainWindow::on_lineEdit_ProjectsBrowserFilter_textChanged(const QString &arg1)
{
    m_BrowserProxyModel->setFilterRegExp(QRegExp(arg1,Qt::CaseInsensitive));
    ui->tableView_ProjectsBrowser->horizontalHeader()->setSortIndicator(BrowserColProject,Qt::DescendingOrder);
}

void MainWindow::on_lineEdit_FilesBrowserFilter_textChanged(const QString &arg1)
{
    m_FileSetProxyModel->setFilterRegExp(QRegExp(arg1,Qt::CaseInsensitive));
    ui->tableView_FilesBrowser->horizontalHeader()->setSortIndicator(FilesColDate,Qt::DescendingOrder);
}

void MainWindow::on_comboBox_ProjectsBrowser_currentIndexChanged(int index)
{
    ui->lineEdit_ProjectsBrowserFilter->clear();
    m_BrowserProxyModel->setFilteringColumns(index);
}

void MainWindow::on_comboBox_Files_currentIndexChanged(int index)
{
    ui->lineEdit_FilesBrowserFilter->clear();
    m_FileSetProxyModel->setFilteringColumns(index);
}

void MainWindow::on_pushButton_AddFiles_clicked()
{
    if (m_currentFileSetRow > -1)
        addFiles();
    else
        QMessageBox::information(this,"Information","Please, first select the the project in order to add files.");
}

void MainWindow::on_tableView_ProjectsBrowser_clicked(const QModelIndex &index)
{
    //qDebug() << "MainWindow::on_tableView_ProjectsBrowser_clicked: insex.is valid" << index.isValid();
    int currentRow = proxyToModel(index, m_BrowserProxyModel);
    //qDebug() << "MainWindow::on_tableView_ProjectsBrowser_clicked: currentRow" << currentRow;

    if (m_currentFileSetRow != currentRow)
    {
        setFileSetModel(currentRow);
        m_currentFileSetRow = currentRow;
    }
}

void MainWindow::setFalseSave()
{
    m_isSaved = false;
}

void MainWindow::on_tableView_FilesBrowser_doubleClicked(const QModelIndex &index)
{
    if(index.column() == FilesColFiles)
    {
        int row = index.row();
        QString fileNameString = ui->tableView_FilesBrowser->model()->index(row, FilesColFiles).data().toString();
        QDesktopServices::openUrl(QUrl(tr("file:///%1").arg(fileNameString), QUrl::TolerantMode));
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

bool MainWindow::loadFile(const QString &fileName)
{
    if(maybeSave())
    {
        m_recentlyOpenedFileName =fileName;

        if(!m_recentlyOpenedFileName.isEmpty())
        {
            saveSettings();
            QDomDocument xmlDocument = readFile(m_recentlyOpenedFileName);
            xmlToModel(xmlDocument);
        }
        this->setWindowTitle(tr(PROGRAM_NAME" - %1").arg(m_recentlyOpenedFileName));
        m_isNewXML = false;
        return true;
    }
    return false;
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    int numRecentFiles = qMin(recentFiles.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1. %2").arg(i + 1).arg(strippedName(recentFiles[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(recentFiles[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFiles(QString &fileName)
{
    if(!recentFiles.contains(fileName))
    {
        qDebug() << "on_actionOpen_XML_triggered recentFiles.size()=" << recentFiles.size();
        if(recentFiles.size() >= MaxRecentFiles)
        {
            recentFiles.removeFirst();
            qDebug() << "on_actionOpen_XML_triggered - removed file";
        }

        recentFiles.append(fileName);
        updateRecentFileActions();
    }
}

void MainWindow::on_tableView_ProjectsBrowser_doubleClicked(const QModelIndex &index)
{
    if(index.column() == BrowserColProjectFolder)
    {
        int row = index.row();
        QString fileNameString = ui->tableView_ProjectsBrowser->model()->index(row, BrowserColProjectFolder).data().toString();
        QDesktopServices::openUrl(QUrl(tr("file:///%1").arg(fileNameString), QUrl::TolerantMode));
    }
}
