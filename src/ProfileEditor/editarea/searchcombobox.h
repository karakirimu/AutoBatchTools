#ifndef SEARCHCOMBOBOX_H
#define SEARCHCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <QSettings>
#include <../searchxmlbuilder/searchxmlbuilder.h>
#include <../filesearchdialog/filesearchdialog.h>

class SearchComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SearchComboBox(QObject *parent= 0);
    ~SearchComboBox();
    void reloadComboBoxItem();

//    QString getCurrentVariant();
public slots:
    void addAction();
    void editAction();
    void deleteAction();

private:
    SearchXmlBuilder *builder;

};

#endif // SEARCHCOMBOBOX_H
