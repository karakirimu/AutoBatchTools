#include "filesearchdialog.h"
#include "ui_filesearchdialog.h"

FileSearchDialog::FileSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSearchDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowCloseButtonHint);

    //install closeevent filter
    installEventFilter(this);

    ui->setupUi(this);

    //set input number only.
    ui->secondsLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    ui->fsizeLineEdit1->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));
    ui->fsizeLineEdit2->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));

    //connect action
    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
    connect( ui->buttonBox, SIGNAL(rejected()), this, SLOT(onReject()));
    connect( ui->openToolButton, SIGNAL(clicked()), this, SLOT(openAction()));
    connect( ui->testSearchButton, SIGNAL(clicked()), this, SLOT(searchAction()));
    connect( ui->directoryLineEdit, SIGNAL(textChanged(QString)), this, SLOT(dirEditFinished(QString)));

    //set current time
    QDateTime time = QDateTime::currentDateTime();
    ui->modifiedDateTimeEdit->setDateTime(time);
    ui->createDateTimeEdit->setDateTime(time);

    //set new xml builder
    builder = new SearchXmlBuilder();

    editflag = false;
}

FileSearchDialog::~FileSearchDialog()
{
    delete builder;
    delete ui;
}

/**
 * @brief FileSearchDialog::setSettingList
 * @param data
 * @details this function sets text and checkbox. it uses in edit only.
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:      2:      3:      4:     5:
 * 0    :name     :text   :
 * 1    :variant  :text   :
 * 2    :keyword  :text   :
 * 3    :dir      :text   :
 * 4    :recursive:data   :
 * 5    :seconds  :text   :enabled:data   :
 * 6    :creation :text   :enabled:data   :combo   :data
 * 7    :modified :text   :enabled:data   :combo   :data
 * 8    :fsize_1  :text   :enabled:data   :combo   :data
 * 9    :fsize_2  :text   :enabled:data   :combo   :data
 */
void FileSearchDialog::loadSettingList(int index, const QList<QStringList> *data)
{
    //qDebug() << data->toVector();
    if(data->count() != 9) return;

    //window title
    setWindowTitle(tr("Editing-") + data->at(NAME).at(1));

    //set edit flags
    editflag = true;
    //set edit index
    editindex = index;

    //setting data
    ui->nameLineEdit->setText(data->at(NAME).at(1));

//    ui->variantLineEdit->setText(data->at(1).at(1));

    ui->keywordLineEdit->setText(data->at(KEYWORD).at(1));

    ui->directoryLineEdit->setText(data->at(DIR).at(1));

    ui->recursiveCheckBox->setChecked(VariantConverter::stringToBool(data->at(RECURSIVE).at(1)));

    ui->secondsLineEdit->setText(secondsToTime(data->at(SECONDS).at(1)));
    ui->secondsCheckBox->setChecked(VariantConverter::stringToBool(data->at(SECONDS).at(3)));

    QDateTime time = QDateTime::fromString(data->at(CREATION).at(1), "yyyy/MM/dd HH:mm:ss");
    ui->createDateTimeEdit->setDateTime(time);
    ui->createCheckBox->setChecked(VariantConverter::stringToBool(data->at(CREATION).at(3)));
    ui->createComboBox->setCurrentIndex(VariantConverter::stringToInt(data->at(CREATION).at(5)));

    QDateTime time2 = QDateTime::fromString(data->at(MODIFIED).at(1), "yyyy/MM/dd HH:mm:ss");
    ui->modifiedDateTimeEdit->setDateTime(time2);
    ui->modifiedCheckBox->setChecked(VariantConverter::stringToBool(data->at(MODIFIED).at(3)));
    ui->modifiedComboBox->setCurrentIndex(VariantConverter::stringToInt(data->at(MODIFIED).at(5)));

    ui->fsizeLineEdit1->setText(decodeFromBytes(data->at(FSIZE_1).at(1), 1));
    ui->fsizeCheckBox->setChecked(VariantConverter::stringToBool(data->at(FSIZE_1).at(3)));
    ui->fsizeComboBox1->setCurrentIndex(VariantConverter::stringToInt(data->at(FSIZE_1).at(5)));

    ui->fsizeLineEdit2->setText(decodeFromBytes(data->at(FSIZE_2).at(1), 2));
    ui->fsizeComboBox2->setCurrentIndex(VariantConverter::stringToInt(data->at(FSIZE_2).at(5)));
}

void FileSearchDialog::onAccept(){
    QList<QStringList> list;
    createList(&list);
    if(editflag){
        // modified
        builder->editItem(editindex, &list);
    }else{
        builder->addItem(&list);
    }
    accept();
}

void FileSearchDialog::onReject(){
    reject();
}

void FileSearchDialog::openAction()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString folder = dialog.getExistingDirectory(this, tr("Open Folder"), "./");
    ui->directoryLineEdit->setText(folder);
}

void FileSearchDialog::searchAction()
{
    ui->resultTableWidget->setRowCount(0);

    QList<QStringList> list;
    createList(&list);

    FileSearchLoader *loader = new FileSearchLoader();
    QStringList nlist = loader->searchFromStrList(&list);
    delete loader;

    //for windows
    int count = nlist.count();
    ui->resultTableWidget->setRowCount(count);

    for(int i = 0; i < count; i++){
        ui->resultTableWidget->setItem(i, 0, new QTableWidgetItem(nlist.at(i)));
    }
}

void FileSearchDialog::dirEditFinished(QString text)
{
    ui->directoryTestLineEdit->setText(text);
}

void FileSearchDialog::closeEvent(QCloseEvent *event)
{
    // ドキュメントが変更されている場合の警告
    QMessageBox::StandardButton res = QMessageBox::question(\
      this, tr("Alert"), tr("Overwrite this file ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    switch( res )
    {
    case QMessageBox::Yes:
      // 保存できたなら、閉じてOK
      onAccept();
      //if (saved())  event->accept();
      break;

    case QMessageBox::No:
      // 保存しなくて、そのまま閉じてOK
      reject();
      break;

    case QMessageBox::Cancel:
      // キャンセルして作業に戻る
      event->ignore();

    default:
      break;
    }
}

void FileSearchDialog::createList(QList<QStringList> *newlist)
{
    QStringList tmp;

    //add title
    tmp << "name" << ui->nameLineEdit->text();
    newlist->append(tmp);
    tmp.clear();

    //add variant
//    tmp << "variant" << ui->variantLineEdit->text();
//    newlist->append(tmp);
//    tmp.clear();

    //add keyword
    tmp << "keyword" << ui->keywordLineEdit->text();
    newlist->append(tmp);
    tmp.clear();

    //add dir
    tmp << "dir" << ui->directoryLineEdit->text();
    newlist->append(tmp);
    tmp.clear();

    //add recursive
    tmp << "recursive" << VariantConverter::boolToString(ui->recursiveCheckBox->isChecked());
    newlist->append(tmp);
    tmp.clear();

    //add seconds
    tmp << "seconds" << timeToSeconds(ui->secondsLineEdit->text())
        << "enabled" << VariantConverter::boolToString(ui->secondsCheckBox->isChecked());
    newlist->append(tmp);
    tmp.clear();

    //add creation
    tmp << "creation" << ui->createDateTimeEdit->text()
        << "enabled" << VariantConverter::boolToString(ui->createCheckBox->isChecked())
        << "combo" << QString::number(ui->createComboBox->currentIndex());
    newlist->append(tmp);
    tmp.clear();

    //add modified
    tmp << "modified" << ui->modifiedDateTimeEdit->text()
        << "enabled" << VariantConverter::boolToString(ui->modifiedCheckBox->isChecked())
        << "combo" << QString::number(ui->modifiedComboBox->currentIndex());
    newlist->append(tmp);
    tmp.clear();

    //add filesize_1(fsize_1)
    tmp << "fsize_1" << encodeToBytes(ui->fsizeLineEdit1->text(), 1)
        << "enabled" << VariantConverter::boolToString(ui->fsizeCheckBox->isChecked())
        << "combo" << QString::number(ui->fsizeComboBox1->currentIndex());
    newlist->append(tmp);
    tmp.clear();

    //add filesize_2(fsize_2)
    tmp << "fsize_2" << encodeToBytes(ui->fsizeLineEdit2->text(), 2)
        << "enabled" << VariantConverter::boolToString(ui->fsizeCheckBox->isChecked())
        << "combo" << QString::number(ui->fsizeComboBox2->currentIndex());
    newlist->append(tmp);
    tmp.clear();
}

QString FileSearchDialog::timeToSeconds(QString data)
{
    int comboindex = ui->secondsComboBox->currentIndex();
    int num = data.toInt();

    switch(comboindex){
    case 0:
        break;
    case 1:
        num*=60;
        break;
    case 2:
        num*=3600;
        break;
    default:
        break;
    }

    return QString::number(num);
}

QString FileSearchDialog::secondsToTime(QString data)
{
    int num = data.toInt();

    if(num % 3600 == 0){
        ui->secondsComboBox->setCurrentIndex(2);
        num/=3600;
    }else if(num % 60 == 0){
        ui->secondsComboBox->setCurrentIndex(1);
        num/=60;
    }else{
        ui->secondsComboBox->setCurrentIndex(0);
    }

    return QString::number(num);
}

QString FileSearchDialog::encodeToBytes(QString bytes, int comboidx)
{
    int comboindex;
    if(comboidx == 1){
        comboindex = ui->bytesComboBox1->currentIndex();
    }else{
        comboindex = ui->bytesComboBox2->currentIndex();
    }

    long long filesize = bytes.toLongLong();

    switch (comboindex) {
    case 0:
        break;
    case 1:
        filesize *= 1024;
        break;
    case 2:
        filesize *= 1048576;
        break;
    case 3:
        filesize *= 1073741824;
        break;
    case 4:
        filesize *= 1099511627776;
        break;
    default:
        break;
    }

    return QString::number(filesize);
}

QString FileSearchDialog::decodeFromBytes(QString bytes, int comboidx)
{
    long long bytesize = bytes.toLongLong();

    if(bytesize % 1099511627776 == 0){
        setFsizeComboBox(4, comboidx);
        return QString::number(bytesize / 1099511627776);

    }else if(bytesize % 1073741824 == 0 ){
        setFsizeComboBox(3, comboidx);
        return QString::number(bytesize / 1073741824);

    }else if(bytesize % 1048576 == 0){
        setFsizeComboBox(2, comboidx);
        return QString::number(bytesize / 1048576);

    }else if(bytesize % 1024 == 0){
        setFsizeComboBox(1, comboidx);
        return QString::number(bytesize / 1024);

    }else{
        setFsizeComboBox(0, comboidx);
        return QString::number(bytesize);

    }
}

void FileSearchDialog::setFsizeComboBox(int num, int comboidx)
{
    switch (comboidx) {
    case 1:
        ui->bytesComboBox1->setCurrentIndex(num);
        break;
    case 2:
        ui->bytesComboBox2->setCurrentIndex(num);
        break;
    default:
        break;
    }
}
