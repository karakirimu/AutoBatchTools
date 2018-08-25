#ifndef COMMANDTABLE_H
#define COMMANDTABLE_H

#include <QApplication>
#include <QClipboard>
#include <../basictable/basictable.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class CommandTable : public BasicTable
{
    Q_OBJECT
public:
    explicit CommandTable(QWidget *parent = Q_NULLPTR);
    ~CommandTable();

    QString getText(int row);
    void insertItems(QStringList *item);

signals:
    void updateTable(int index, QString newstr, int function);
    void swapTable(int before, int after);

private slots:
    void addAction();
    void editAction();
    void deleteAction();
    void cutAction();
    void copyAction();
    void pasteAction();
    void pasteSpaceAction();
    void pasteEnterAction();
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
    QAction *m_cut;
    QAction *m_paste;
    QAction *m_pastespace;
    QAction *m_pasteenter;
};

#endif // COMMANDTABLE_H
