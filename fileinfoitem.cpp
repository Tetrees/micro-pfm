#include "fileinfoitem.h"

FileInfoItem::FileInfoItem()
{
    m_FileName.clear();
    m_FComment.clear();
    m_FDate.setDate(QDate::currentDate().year(),
                    QDate::currentDate().month(),
                    QDate::currentDate().day());
}

void FileInfoItem::setFileName(QString fileName)
{
    m_FileName = fileName;
}

void FileInfoItem::setComment(QString fComment)
{
    m_FComment = fComment;
}

void FileInfoItem::setDate(QDate fDate)
{
    m_FDate = fDate;
}

QString FileInfoItem::fileName()
{
    return m_FileName;
}

QString FileInfoItem::comment()
{
    return m_FComment;
}

QDate FileInfoItem::date()
{
    return m_FDate;
}
