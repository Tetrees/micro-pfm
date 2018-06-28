#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    m_Project.clear();
    m_Customer.clear();
    m_ProjectFolder.clear();
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

QString NewProjectDialog::project()
{
    return m_Project;
}

QString NewProjectDialog::customer()
{
    return m_Customer;
}

QString NewProjectDialog::projectFolder()
{
    return m_ProjectFolder;
}

void NewProjectDialog::setProject(QString project)
{
    m_Project=project;
}

void NewProjectDialog::setCustomer(QString customer)
{
    m_Customer=customer;
}

void NewProjectDialog::setProjectFolder(QString projectFolder)
{
    m_ProjectFolder=projectFolder;
}

void NewProjectDialog::on_pushButton_Create_clicked()
{
    setProject(ui->lineEdit_Project->text());
    setCustomer(ui->lineEdit_Customer->text());
    setProjectFolder(ui->lineEdit_ProjectFolder->text());
    qDebug() << "inside: NewProjectDialog::on_pushButton_Create_clicked";
    accept();
}

void NewProjectDialog::on_pushButtonSelect_ProjectFolder_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        ui->lineEdit_ProjectFolder->setText(dir);
}
