#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    ProjectsList projectList;
    BrowserModel browserModel(&projectList);

    ProjectItem projectItem;
    FileSetModel fileSetModel(&projectItem);

    w.initiateDialogData(&browserModel, &fileSetModel);
    w.show();

    return a.exec();
}
