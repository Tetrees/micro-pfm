#ifndef CUSTOMERSELECTDIALOG_H
#define CUSTOMERSELECTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QListWidget>

namespace Ui {
class CustomerSelectDialog;
}

class CustomerSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CustomerSelectDialog(QWidget *parent = 0);
    ~CustomerSelectDialog();

    void initiateCustomerSelect(QStringList *customersList);
    
private slots:
    void on_pushButton_Select_clicked();

    void on_listWidget_Customers_doubleClicked(const QModelIndex &index);

private:
    Ui::CustomerSelectDialog *ui;
    QListWidgetItem *m_CurrentSelection;
};

#endif // CUSTOMERSELECTDIALOG_H
