#ifndef FILEINFOITEM_H
#define FILEINFOITEM_H

#include <QDate>
#include <QString>
#include <QDebug>

class FileInfoItem
{

public:
    explicit FileInfoItem();

    //setters
    void setFileName(QString fileName);
    void setComment(QString fComment);
    void setDate(QDate fDate);

    //getters
    QString fileName();
    QString comment();
    QDate date();

private:
    QString m_FileName;
    QString m_FComment;
    QDate m_FDate;
};

#endif // FILEINFOITEM_H
