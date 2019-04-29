#ifndef PLUGINSCOMBOBOX_H
#define PLUGINSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <QPluginLoader>
#include <QMessageBox>
#include <../pluginsxmlbuilder/pluginsxmlbuilder.h>
#include <../plugins/ExtraPluginInterface/extraplugininterface.h>
#include <../filesearchloader/filesearchloader.h>
#include <QFileDialog>

class PluginsComboBox : public QComboBox
{
    Q_OBJECT

signals:
    void pluginChanged(bool);

public:
    explicit PluginsComboBox(QObject *parent = nullptr);
    ~PluginsComboBox();

    QString getCurrentExtraFile();
    void reloadComboBoxItem();
public slots:
    void addItemAction();
//    void editAction(QString variant);
    void deleteAction();
    void pluginCheckAction(int index);
private:
    PluginsXmlBuilder *builder;

    //file list buffer
    QStringList buffer;

    //auto search settings
    QList<QStringList> searchsettings;

    const int NAME_XML = 1;
    const int PATH_XML = 3;
};

#endif // PLUGINSCOMBOBOX_H
