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

//DEPENDS_XML
void EditorTab::setNormalDataList(QList<QStringList> *list, int firstpos)
{
    QWidget *widget = this->widget(NORMAL);
    QCheckBox *timechb = widget->findChild<QCheckBox *>("timeoutCheckBox");
    QSpinBox *tospin = widget->findChild<QSpinBox *>("timeoutSpinBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox");
    CommandTable *table = widget->findChild<CommandTable *>("cmdTableWidget");
    int counter = VariantConverter::stringToInt(list->at(firstpos + 2).at(1));
    timechb->setChecked(VariantConverter::stringToBool(list->at(firstpos + 1).at(1)));
    tospin->setValue(((QString)list->at(firstpos + 1).at(3)).toInt());
    table->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        table->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 3 + i).at(1)));
    }
    autoonly->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));
}

//DEPENDS_XML
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

    scb->setCurrentText(list->at(firstpos + 1).at(1));
    sle->setText(list->at(firstpos + 2).at(1));
    vcb->insertItem(0,list->at(firstpos + 3).at(1));
    ole->setText(list->at(firstpos + 4).at(1));

    if(list->at(firstpos + 4).at(3) == "0"){
        vari->setChecked(true);
    }else{
        file->setChecked(true);
    }

    autoonly->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));
}

//DEPENDS_XML
void EditorTab::setScriptDataList(QList<QStringList> *list, int firstpos)
{
    QWidget *widget = this->widget(EXTRAFUNC);
    ExtraFunctionsComboBox *scb = widget->findChild<ExtraFunctionsComboBox *>("extrafuncComboBox");
//    VariantComboBox *vcb = widget->findChild<VariantComboBox *>("varComboBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_3");
    CommandTable *table = widget->findChild<CommandTable *>("extrafuncTableWidget");

    scb->setCurrentText(list->at(firstpos + 1).at(1));
//    vcb->insertItem(0,list->at(firstpos + 1).at(1));
    int counter = VariantConverter::stringToInt(list->at(firstpos + 3).at(1));
    table->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        table->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 4 + i).at(1)));
    }

    autoonly->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));
}

//DEPENDS_XML
void EditorTab::setOtherDataList(QList<QStringList> *list, int firstpos)
{
    QWidget *widget = this->widget(OTHER);
    ProfileComboBox *pcb = widget->findChild<ProfileComboBox *>("profileComboBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_4");

    pcb->setCurrentText(list->at(firstpos + 1).at(1));
    autoonly->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));
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

//DEPENDS_XML
void EditorTab::getNormalDataList(QStringList *list)
{
    QWidget *widget = this->widget(NORMAL);
    QCheckBox *timeoutCheckBox = widget->findChild<QCheckBox *>("timeoutCheckBox");
    QSpinBox *tospin = widget->findChild<QSpinBox *>("timeoutSpinBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox");
    CommandTable *table = widget->findChild<CommandTable *>("cmdTableWidget");

    int rcount = table->rowCount();
    list->append(VariantConverter::boolToString(autoonly->isChecked()));
    list->append(VariantConverter::boolToString(timeoutCheckBox->isChecked()));
    list->append(QString::number(tospin->value()));
    list->append(QString::number(rcount));
    for(int i = 0; i < rcount; i++){
        list->append(table->getText(i));
    }

}

//DEPENDS_XML
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

//DEPENDS_XML
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

//DEPENDS_XML
void EditorTab::getOtherDataList(QStringList *list)
{
    QWidget *widget = this->widget(OTHER);
    ProfileComboBox *pcb = widget->findChild<ProfileComboBox *>("profileComboBox");
    QCheckBox *autoonly = widget->findChild<QCheckBox *>("autoOnlyCheckBox_4");

    list->append(VariantConverter::boolToString(autoonly->isChecked()));
    list->append(pcb->currentText());
    list->append(pcb->getCurrentFileName());
}

bool EditorTab::getCurrentIndexOnlyChecked()
{
    QCheckBox *autoonly;
    switch (this->currentIndex()) {
    case NORMAL:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox");
        break;
    case SEARCH:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox_2");
        break;
    case EXTRAFUNC:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox_3");
        break;
    case OTHER:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox_4");
        break;
    default:
        return false;
    }

    return autoonly->isChecked();
}

void EditorTab::openSavefile()
{
    QLineEdit *outputLineEdit = this->widget(SEARCH)->findChild<QLineEdit *>("outputLineEdit");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Search file"),\
                                         QDir::currentPath(), tr("File (*.*)"));

    outputLineEdit->setText(fileName);
}
