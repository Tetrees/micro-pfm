#include "projectfolderedit.h"

ProjectFolderEdit::ProjectFolderEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void ProjectFolderEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void ProjectFolderEdit::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> droppedUrl = event->mimeData()->urls();
        if (droppedUrl.size() > 0)
        {
            QString localPath = droppedUrl[0].toLocalFile();
            QFileInfo fileInfo(localPath);
            setText(fileInfo.absoluteFilePath());
        }
    }
    event->acceptProposedAction();
}
