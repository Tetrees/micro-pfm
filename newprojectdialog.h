#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include "customerselectdialog.h"

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();
    //Getters
    QString project();
    QString customer();
    QString projectFolder();

    //Setters
    void setProject(QString project);
    void setCustomer(QString customer);
    void setProjectFolder(QString projectFolder);
    
private slots:
    void on_pushButton_Create_clicked();

    void on_pushButtonSelect_ProjectFolder_clicked();

private:
    Ui::NewProjectDialog *ui;
    QString m_Project;
    QString m_Customer;
    QString m_ProjectFolder;
};

#endif // NEWPROJECTDIALOG_H
