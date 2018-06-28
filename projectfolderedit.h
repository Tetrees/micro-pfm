#ifndef PROJECTFOLDEREDIT_H
#define PROJECTFOLDEREDIT_H

#include <QtGui>
#include <QFileInfo>

class ProjectFolderEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ProjectFolderEdit(QWidget *parent = 0);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    
signals:
    
public slots:
    
};

#endif // PROJECTFOLDEREDIT_H
