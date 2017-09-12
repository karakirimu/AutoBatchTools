#include "editortab.h"
#include "profilecombobox.h"

EditorTab::EditorTab(QWidget *parent)
    : QTabWidget(parent)
{
//    QLineEdit *edit = this->findChild<QLineEdit *>("verLineEdit");
//    if(edit != nullptr) edit->setText("access from tabclass");
//    QList<QLineEdit*> x = findChildren<QLineEdit *>();
}

EditorTab::~EditorTab()
{

}


// Constructor can not initialize connection.
void EditorTab::setConnection()
{
    //connect action in search widget
    QWidget *widgetsearch = this->widget(SEARCH);
    QPushButton *addbutton = widgetsearch->findChild<QPushButton *>("searchAddButton");
    QPushButton *editbutton = widgetsearch->findChild<QPushButton *>("searchEditButton");
    QPushButton *deletebutton = widgetsearch->findChild<QPushButton *>("searchDeleteButton");
    SearchComboBox *scb = widgetsearch->findChild<SearchComboBox *>("searchComboBox");
    QToolButton *openButton = widgetsearch->findChild<QToolButton *>("openToolButton");
    connect(addbutton, &QAbstractButton::clicked, scb, &SearchComboBox::addAction);
    connect(editbutton, &QAbstractButton::clicked, scb, &SearchComboBox::editAction);
    connect(deletebutton, &QAbstractButton::clicked, scb, &SearchComboBox::deleteAction);
    connect(openButton, &QToolButton::clicked, this, &EditorTab::openSavefile); 

    //connect action in extrafunc widget
    QWidget *widgetextra = this->widget(EXTRAFUNC);
    ExtraFunctionsComboBox *efcb = widgetextra->findChild<ExtraFunctionsComboBox *>("extrafuncComboBox");
    QToolButton *addbutton_e = widgetextra->findChild<QToolButton *>("extrafuncAddButton");
    QToolButton *deletebutton_e = widgetextra->findChild<QToolButton *>("extrafuncDeleteButton");
    QPushButton *pluginsetting = widgetextra->findChild<QPushButton *>("pluginSettingButton");
    connect(addbutton_e, &QAbstractButton::clicked, efcb, &ExtraFunctionsComboBox::addItemAction);
    connect(deletebutton_e, &QAbstractButton::clicked, efcb, &ExtraFunctionsComboBox::deleteAction);
    connect(efcb, &ExtraFunctionsComboBox::pluginChanged, pluginsetting, &QPushButton::setEnabled);

}

void EditorTab::moveTabFromXml(int num)
{
    if(num < 4 && num > 0) setCurrentIndex(num);
}

void EditorTab::setNormalDataList(QList<QStringList> *list, int firstpos)
{
    QWidget *widget = this->widget(NORMAL);
    QCheckBox *chb = widget->findChild<QCheckBox *>("noWaitCheckBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox");
    CommandTable *table = widget->findChild<CommandTable *>("cmdTableWidget");
    int counter = VariantConverter::stringToInt(list->at(firstpos + 1).at(1));
    chb->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(1)));
    table->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        table->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 2 + i).at(1)));
    }
    autoonly->setChecked(VariantConverter::stringToBool(list->at(0).at(3)));
}

void EditorTab::setSearchDataList(QList<QStringList> *list, int firstpos)
{
    QWidget *widget = this->widget(SEARCH);
    SearchComboBox *scb = widget->findChild<SearchComboBox *>("searchComboBox");
    QLineEdit *sle = widget->findChild<QLineEdit *>("separatorLineEdit");
    VariantComboBox *vcb = widget->findChild<VariantComboBox *>("localVariantComboBox");
    QLineEdit *ole = widget->findChild<QLineEdit *>("outputLineEdit");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_2");
    QRadioButton *vari = widget->findChild<QRadioButton *>("variRadioButton");
    QRadioButton *file = widget->findChild<QRadioButton *>("fileRadioButton");

    scb->setCurrentText(list->at(firstpos).at(1));
    sle->setText(list->at(firstpos + 1).at(1));
    vcb->insertItem(0,list->at(firstpos + 2).at(1));
    ole->setText(list->at(firstpos + 3).at(1));

    if(list->at(firstpos + 3).at(3) == "0"){
        vari->setChecked(true);
    }else{
        file->setChecked(true);
    }

    autoonly->setChecked(VariantConverter::stringToBool(list->at(0).at(3)));
}

void EditorTab::setScriptDataList(QList<QStringList> *list, int firstpos)
{
    QWidget *widget = this->widget(EXTRAFUNC);
    ExtraFunctionsComboBox *scb = widget->findChild<ExtraFunctionsComboBox *>("extrafuncComboBox");
//    VariantComboBox *vcb = widget->findChild<VariantComboBox *>("varComboBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_3");
    CommandTable *table = widget->findChild<CommandTable *>("extrafuncTableWidget");

    scb->setCurrentText(list->at(firstpos).at(1));
//    vcb->insertItem(0,list->at(firstpos + 1).at(1));
    int counter = VariantConverter::stringToInt(list->at(firstpos + 2).at(1));
    table->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        table->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 3 + i).at(1)));
    }

    autoonly->setChecked(VariantConverter::stringToBool(list->at(0).at(3)));
}

void EditorTab::setOtherDataList(QList<QStringList> *list, int firstpos)
{
    QWidget *widget = this->widget(OTHER);
    ProfileComboBox *pcb = widget->findChild<ProfileComboBox *>("profileComboBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_4");

    pcb->setCurrentText(list->at(firstpos + 1).at(1));
    autoonly->setChecked(VariantConverter::stringToBool(list->at(0).at(3)));
}

void EditorTab::setTempDataList(QList<QStringList> *list, PESharedFunction *sfunction)
{
    if(list->empty()) return;

    //no file showing module
    int cmdskip = VariantConverter::stringToInt(list->at(sfunction->firstPosNormal() + 1).at(1));

    //set widget selection
    setCurrentIndex(VariantConverter::stringToInt(list->at(1).at(1)));

    //load type=normal
    setNormalDataList(list, sfunction->firstPosNormal());

    //load type=search
    setSearchDataList(list, sfunction->firstPosSearch());

    //load type=extrafunc
    setScriptDataList(list, sfunction->firstPosScript() + cmdskip);

    //load type=other
    setOtherDataList(list, sfunction->firstPosOther());
}

void EditorTab::getNormalDataList(QStringList *list)
{
    QWidget *widget = this->widget(NORMAL);
    QCheckBox *noWaitCheckBox = widget->findChild<QCheckBox *>("noWaitCheckBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox");
    CommandTable *table = widget->findChild<CommandTable *>("cmdTableWidget");

    int rcount = table->rowCount();
    list->append(VariantConverter::boolToString(autoonly->isChecked()));
    list->append(VariantConverter::boolToString(noWaitCheckBox->isChecked()));
    list->append(QString::number(rcount));
    for(int i = 0; i < rcount; i++){
        list->append(table->getText(i));
    }

}

void EditorTab::getSearchDataList(QStringList *list)
{
    QWidget *widget = this->widget(SEARCH);
    SearchComboBox *searchComboBox = widget->findChild<SearchComboBox *>("searchComboBox");
    QLineEdit *separatorLineEdit = widget->findChild<QLineEdit *>("separatorLineEdit");
    VariantComboBox *localVariantComboBox = widget->findChild<VariantComboBox *>("localVariantComboBox");
    QLineEdit *outputLineEdit = widget->findChild<QLineEdit *>("outputLineEdit");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_2");
    QRadioButton *vari = widget->findChild<QRadioButton *>("variRadioButton");
//    QRadioButton *file = widget->findChild<QRadioButton *>("fileRadioButton");

    list->append(VariantConverter::boolToString(autoonly->isChecked()));
    list->append(searchComboBox->currentText());
    list->append(QString::number(searchComboBox->currentIndex()));
    list->append(separatorLineEdit->text());
    list->append(localVariantComboBox->currentText());
    list->append(outputLineEdit->text());
    if(vari->isChecked()){
        list->append("0");
    }else{
        list->append("1");
    }
}

void EditorTab::getScriptDataList(QStringList *list)
{
    QWidget *widget = this->widget(EXTRAFUNC);
    ExtraFunctionsComboBox *scb = widget->findChild<ExtraFunctionsComboBox *>("extrafuncComboBox");
//    VariantComboBox *vcb = widget->findChild<VariantComboBox *>("varComboBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_3");
    CommandTable *extrafuncTableWidget = widget->findChild<CommandTable *>("extrafuncTableWidget");

    int rcount = extrafuncTableWidget->rowCount();
    list->append(VariantConverter::boolToString(autoonly->isChecked()));
    list->append(scb->currentText());
    list->append(scb->getCurrentExtraFile());
    list->append(QString::number(rcount));
    for(int i = 0; i < rcount; i++){
        list->append(extrafuncTableWidget->getText(i));
    }
}

void EditorTab::getOtherDataList(QStringList *list)
{
    QWidget *widget = this->widget(OTHER);
    ProfileComboBox *pcb = widget->findChild<ProfileComboBox *>("profileComboBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_4");

    list->append(VariantConverter::boolToString(autoonly->isChecked()));
    list->append(pcb->currentText());
    list->append(pcb->getCurrentFileName());
}

void EditorTab::openSavefile()
{
    QLineEdit *outputLineEdit = this->widget(SEARCH)->findChild<QLineEdit *>("outputLineEdit");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Search file"),\
                                         QDir::currentPath(), tr("File (*.*)"));

    outputLineEdit->setText(fileName);
}
