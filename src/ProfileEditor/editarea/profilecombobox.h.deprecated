#ifndef PROFILECOMBOBOX_H
#define PROFILECOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <profilexmlbuilder.h>

class ProfileComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ProfileComboBox(QObject *parent = 0);
    ~ProfileComboBox();

    QString getCurrentFileName();
    QString getCurrentFileName(int index);
public slots:
    void reloadComboBoxItem();

private:
    ProfileXmlBuilder *builder;
};

#endif // PROFILECOMBOBOX_H
