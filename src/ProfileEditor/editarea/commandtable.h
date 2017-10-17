#ifndef COMMANDTABLE_H
#define COMMANDTABLE_H

#include <QApplication>
#include <QClipboard>
#include <../basictable/basictable.h>
#include <xmllistgenerator.h>

class CommandTable : public BasicTable
{
    Q_OBJECT
public:
    explicit CommandTable(QWidget *parent = Q_NULLPTR);
    ~CommandTable();

    QString getText(int row);

signals:
    void updateTable(int index, QString newstr, int function);
    void swapTable(int before, int after);

private slots:
    void addAction();
    void editAction();
    void deleteAction();
    void cutAction();
    void copyAction();
    void plainpasteAction();
    void pasteAction();
    void upAction();
    void downAction();
    void openFileAction();
    void openDirectoryAction();

    void editedAction(int row, int column);

private:
    void setPopupActionTop();
    void setPopupActionDefault();
    bool eventFilter(QObject *obj, QEvent *event);

    QAction *m_file;
    QAction *m_dir;
    QAction *m_add;
    QAction *m_delete;
    QAction *m_edit;
    QAction *m_paste;
};

#endif // COMMANDTABLE_H
