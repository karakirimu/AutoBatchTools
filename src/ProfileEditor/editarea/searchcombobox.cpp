#include "searchcombobox.h"

SearchComboBox::SearchComboBox(QObject *)
{
    //set new xml builder
    builder = new SearchXmlBuilder();
}

SearchComboBox::~SearchComboBox()
{
    delete builder;
}

void SearchComboBox::reloadComboBoxItem()
{
    int tmp = this->currentIndex();
    this->clear();
    QList<QStringList> item;
    int counter = builder->count();
    for(int i = 0; i < counter; i++){
        if(builder->readItem(i, &item)){
            this->addItem(item.at(0).at(1));
            item.clear();
        }
    }
    if(-1 < tmp && tmp < counter)
        this->setCurrentIndex(tmp);
}

void SearchComboBox::addAction()
{
    FileSearchDialog *fs = new FileSearchDialog();
    setTheme(fs);
    fs->setWindowTitle(tr("編集-新規*"));
    if(fs->exec() == QDialog::Accepted){
        reloadComboBoxItem();
        this->setCurrentIndex(this->count() - 1);
    }
}

void SearchComboBox::editAction()
{
    //if count is zero.
    if(this->count() == 0) return;

    FileSearchDialog *fs = new FileSearchDialog();
    setTheme(fs);
    QList<QStringList> list;

    int index = this->currentIndex();
    if(builder->readItem(index, &list)){
        //set title
        fs->loadSettingList(index, &list);
    }
    if(fs->exec() == QDialog::Accepted){
        reloadComboBoxItem();
    }
}

void SearchComboBox::deleteAction()
{
    //if rowcount is zero.
    if(builder->count() == 0) return;

    // show delete warning
    QMessageBox::StandardButton res = QMessageBox::warning(
      this, tr("Alert")
                , tr("Do you want to delete selected item ?\r\n(File is not delete)")
                , QMessageBox::Yes | QMessageBox::No );

    if(res == QMessageBox::Yes){
        //delete file item
        int current = this->currentIndex();
        builder->deleteItem(current);

        //reload
        reloadComboBoxItem();

        //re-set combo select
        this->setCurrentIndex(current - 1);
    }
}

QString SearchComboBox::getCurrentVariant()
{
    int selected = this->currentIndex();
    if(selected > -1){
        QList<QStringList> item;
        builder->readItem(selected, &item);
        return item.at(1).at(1);
    }

    return "";
}

//QSS_THEME
void SearchComboBox::setTheme(FileSearchDialog *fs)
{
    QSettings settings( "./psettings.ini", QSettings::IniFormat );

    //theme settings
    settings.beginGroup("BASICSETTING");
    QString stylecolor = settings.value("THEMECOLOR", "Default").toString();
    settings.endGroup();

    if(stylecolor != "Default"){
#ifdef QT_DEBUG
        QFile file(QString("C:/Users/mr/Dropbox/Qt Creator/master-autobatchrunner/res/themes/%1.qss").arg(stylecolor));
#else
        QFile file(QString(":/themes/%1.qss").arg(stylecolor));
#endif
        if(file.open( QFile::ReadOnly | QFile::Text )){
            QString data(QLatin1String(file.readAll()));
            fs->setStyleSheet(data);
        }
    }
}
