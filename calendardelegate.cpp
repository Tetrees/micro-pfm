#include "calendardelegate.h"

CalendarDelegate::CalendarDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *CalendarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid() && index.column() == FilesColDate)
    {
        QDateEdit *editor = new QDateEdit(parent);
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QDate date = QDate::fromString(value, "dd/MM/yyyy");
        if (date.isNull())
            editor->setDate(QDate::currentDate());
        editor->setDate(date);
        editor->setCalendarPopup(true);

        return editor;
    }
    else
    {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void CalendarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.isValid() && index.column() == FilesColDate)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        qDebug() <<  "date value:" <<value;
        QDate date;
        date.fromString(value, "dd/MM/yyyy");
        qDebug() <<  "date from string:" << date.fromString(value, "dd/MM/yyyy");;
        QDateEdit *calendar = static_cast<QDateEdit*>(editor);
        calendar->setDate(date);
    }
    else
        QItemDelegate::setEditorData(editor, index);
}

void CalendarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid() && index.column() == FilesColDate)
    {
        QDateEdit *calendar = static_cast<QDateEdit*>(editor);
        QString value = calendar->date().toString();
        model->setData(index, value, Qt::EditRole);
    }
    else
        QItemDelegate::setModelData(editor, model, index);
}

void CalendarDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        editor->setGeometry(option.rect);
}
