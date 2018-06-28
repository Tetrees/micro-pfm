#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QComboBox>
#include <QMessageBox>
#include <QList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QCompleter>
#include <QtAlgorithms>
#include <QSettings>
#include <QDesktopServices>
#include <QtXml>
#include <QUrl>
#include <QCloseEvent>

#include "browsermodel.h"
#include "browserproxymodel.h"
#include "filesetmodel.h"
#include "filesetproxymodel.h"
#include "newprojectdialog.h"
#include "calendardelegate.h"

#include "constDefinitions.h"

#define PROGRAM_NAME "microPFM"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initiateDialogData(BrowserModel *browserModel,FileSetModel *fileSetModel);

protected:

    void closeEvent(QCloseEvent * event);
    
private slots:
    void on_actionOpen_XML_triggered();

    void on_actionSave_XML_triggered();

    void on_actionSave_As_triggered();

    void showContextMenuProjectsBrowser(const QPoint& position);

    void showContextMenuFilesBrowser(const QPoint& position);

    void on_pushButton_AddNewProject_clicked();

    void on_lineEdit_ProjectsBrowserFilter_textChanged(const QString &arg1);

    void on_lineEdit_FilesBrowserFilter_textChanged(const QString &arg1);

    void on_comboBox_ProjectsBrowser_currentIndexChanged(int index);

    void on_comboBox_Files_currentIndexChanged(int index);

    void on_pushButton_AddFiles_clicked();

    void on_tableView_ProjectsBrowser_clicked(const QModelIndex &index);

    void setFalseSave();

    void on_tableView_FilesBrowser_doubleClicked(const QModelIndex &index);

    void openRecentFile();

    void on_tableView_ProjectsBrowser_doubleClicked(const QModelIndex &index);

private:
    enum {SetBrowserFilter = 1, SetFilesFilter = 2};

    Ui::MainWindow *ui;

    BrowserModel *m_BrowserModel;

    BrowserProxyModel *m_BrowserProxyModel;

    FileSetModel *m_FileSetModel;

    FileSetProxyModel *m_FileSetProxyModel;

    CalendarDelegate *m_CalendarDelegate;

    int m_currentFileSetRow;

    QString m_recentlyOpenedFileName;

    bool m_isSaved;

    bool m_isNewXML;

    QStringList recentFiles;

    enum { MaxRecentFiles = 5 };

    QAction *recentFileActs[MaxRecentFiles];

    void fetchDataFromFile(QString fileName, QStringList &fetchList);

    void saveUpdatedData(QString fileName, QStringList &fetchList);

    void initiateFilterCombobox(QComboBox *filter, int filterNumber);

    void addFiles();

    void removeFiles();

    void addNewProject();

    void removeProjects();

    void setFileSetModel(int currentRow);

    bool saveXml(QString fileName);

    bool saveFile(bool saveAs = true);

    QDomDocument readFile(QString fileName);

    QDomDocument modelToXml();

    void xmlToModel(QDomDocument xmlDocument);

    void addElement(QString tagString, QString textString, QDomElement project, QDomDocument &xmlDocument);

    void saveSettings();

    void loadSettings();

    bool maybeSave();

    int proxyToModel(const QModelIndex &index, QSortFilterProxyModel *proxyModel);//return row from model

    int modelToProxy(int modelRow, QAbstractTableModel *model, QSortFilterProxyModel *proxyModel);//return row from proxy

    void createActions();

    void createMenus();

    bool loadFile(const QString &fileName);

    void updateRecentFileActions();

    QString strippedName(const QString &fullFileName);

    void updateRecentFiles(QString &fileName);

};

#endif // MAINWINDOW_H
