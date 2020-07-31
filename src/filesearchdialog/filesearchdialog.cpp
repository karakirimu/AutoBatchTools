/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
    connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &FileSearchDialog::onAccept);
    connect( ui->buttonBox, &QDialogButtonBox::rejected, this, &FileSearchDialog::onReject);
    connect( ui->openToolButton, &QAbstractButton::clicked, this, &FileSearchDialog::openAction);
    connect( ui->testSearchButton, &QAbstractButton::clicked, this, &FileSearchDialog::searchAction);
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
 * @details this function sets text and checkbox. it uses in edit only.
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:      2:      3:      4:     5:
 * 0    :name     :text   :
 * 1    :keyword  :text   :
 * 2    :regex    :text   :enabled:data   :
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
//    if(data->count() != 9) return;

    //window title
    setWindowTitle(tr("Editing - ") + builder->fetch(SEARCH_NAME, SEARCH_NONE, data));

    //set edit flags
    editflag = true;
    //set edit index
    editindex = index;

    //setting data
    ui->nameLineEdit->setText(builder->fetch(SEARCH_NAME, SEARCH_NONE, data));

//    ui->variantLineEdit->setText(data->at(1).at(1));

    ui->keywordLineEdit->setText(builder->fetch(SEARCH_KEYWORD, SEARCH_NONE, data));

    ui->regexLineEdit->setText(builder->fetch(SEARCH_REGEX, SEARCH_NONE, data));
    ui->regexCheckBox->setChecked(VariantConverter::stringToBool(builder->fetch(SEARCH_RECURSIVE, SEARCH_NONE, data)));

    ui->directoryLineEdit->setText(builder->fetch(SEARCH_DIR, SEARCH_NONE, data));

    ui->recursiveCheckBox->setChecked(VariantConverter::stringToBool(builder->fetch(SEARCH_RECURSIVE, SEARCH_NONE, data)));

    ui->secondsLineEdit->setText(secondsToTime(builder->fetch(SEARCH_SECONDS, SEARCH_NONE, data)));
    ui->secondsCheckBox->setChecked(VariantConverter::stringToBool(builder->fetch(SEARCH_SECONDS, ENABLED, data)));

    QDateTime time = QDateTime::fromString(builder->fetch(SEARCH_CREATION, SEARCH_NONE, data), "yyyy/MM/dd HH:mm:ss");
    ui->createDateTimeEdit->setDateTime(time);
    ui->createCheckBox->setChecked(VariantConverter::stringToBool(builder->fetch(SEARCH_CREATION, ENABLED, data)));
    ui->createComboBox->setCurrentIndex(builder->fetch(SEARCH_CREATION, COMBO, data).toInt());

    QDateTime time2 = QDateTime::fromString(builder->fetch(SEARCH_MODIFIED, SEARCH_NONE, data), "yyyy/MM/dd HH:mm:ss");
    ui->modifiedDateTimeEdit->setDateTime(time2);
    ui->modifiedCheckBox->setChecked(VariantConverter::stringToBool(builder->fetch(SEARCH_MODIFIED, ENABLED, data)));
    ui->modifiedComboBox->setCurrentIndex(builder->fetch(SEARCH_MODIFIED, COMBO, data).toInt());

    ui->fsizeLineEdit1->setText(decodeFromBytes(builder->fetch(SEARCH_FSIZE_1, SEARCH_NONE, data), 1));
    ui->fsizeCheckBox->setChecked(VariantConverter::stringToBool(builder->fetch(SEARCH_FSIZE_1, ENABLED, data)));
    ui->fsizeComboBox1->setCurrentIndex(builder->fetch(SEARCH_FSIZE_1, COMBO, data).toInt());

    ui->fsizeLineEdit2->setText(decodeFromBytes(builder->fetch(SEARCH_FSIZE_2, SEARCH_NONE, data), 2));
    ui->fsizeComboBox2->setCurrentIndex(builder->fetch(SEARCH_FSIZE_2, COMBO, data).toInt());

    // To reflect the theme setting
    ui->resultTableWidget->setStyleSheet(this->styleSheet());
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
    QMessageBox::StandardButton res = QMessageBox::question(\
      this, tr("Alert"), tr("Save this setting?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    switch( res )
    {
    case QMessageBox::Yes:
      // save
      onAccept();
      //if (saved())  event->accept();
      break;

    case QMessageBox::No:
      // not save
      reject();
      break;

    case QMessageBox::Cancel:
      // cancel
      event->ignore();
       break;

    default:
      break;
    }
}

void FileSearchDialog::createList(QList<QStringList> *newlist)
{
    QStringList tmp;

    //add title
    newlist->append(QStringList() << SEARCH_NAME << ui->nameLineEdit->text());

    //add keyword
    newlist->append(QStringList() << SEARCH_KEYWORD << ui->keywordLineEdit->text());

    //add regex
    tmp << SEARCH_REGEX << ui->regexLineEdit->text()
        << ENABLED << VariantConverter::boolToString(ui->regexCheckBox->isChecked());
    newlist->append(tmp);
    tmp.clear();

    //add dir
    newlist->append(QStringList() << SEARCH_DIR << ui->directoryLineEdit->text());

    //add recursive
    newlist->append(QStringList() << SEARCH_RECURSIVE \
                    << VariantConverter::boolToString(ui->recursiveCheckBox->isChecked()));

    //add seconds
    tmp << SEARCH_SECONDS << timeToSeconds(ui->secondsLineEdit->text())
        << ENABLED << VariantConverter::boolToString(ui->secondsCheckBox->isChecked());
    newlist->append(tmp);
    tmp.clear();

    //add creation
    tmp << SEARCH_CREATION << ui->createDateTimeEdit->text()
        << ENABLED << VariantConverter::boolToString(ui->createCheckBox->isChecked())
        << COMBO << QString::number(ui->createComboBox->currentIndex());
    newlist->append(tmp);
    tmp.clear();

    //add modified
    tmp << SEARCH_MODIFIED << ui->modifiedDateTimeEdit->text()
        << ENABLED << VariantConverter::boolToString(ui->modifiedCheckBox->isChecked())
        << COMBO << QString::number(ui->modifiedComboBox->currentIndex());
    newlist->append(tmp);
    tmp.clear();

    //add filesize_1(fsize_1)
    tmp << SEARCH_FSIZE_1 << encodeToBytes(ui->fsizeLineEdit1->text(), 1)
        << ENABLED << VariantConverter::boolToString(ui->fsizeCheckBox->isChecked())
        << COMBO << QString::number(ui->fsizeComboBox1->currentIndex());
    newlist->append(tmp);
    tmp.clear();

    //add filesize_2(fsize_2)
    tmp << SEARCH_FSIZE_2 << encodeToBytes(ui->fsizeLineEdit2->text(), 2)
        << ENABLED << VariantConverter::boolToString(ui->fsizeCheckBox->isChecked())
        << COMBO << QString::number(ui->fsizeComboBox2->currentIndex());
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
        filesize *= KB;
        break;
    case 2:
        filesize *= MB;
        break;
    case 3:
        filesize *= GB;
        break;
    case 4:
        filesize *= TB;
        break;
    default:
        break;
    }

    return QString::number(filesize);
}

QString FileSearchDialog::decodeFromBytes(QString bytes, int comboidx)
{
    long long bytesize = bytes.toLongLong();

    if(bytesize % TB == 0){
        setFsizeComboBox(4, comboidx);
        return QString::number(bytesize / TB);

    }else if(bytesize % GB == 0 ){
        setFsizeComboBox(3, comboidx);
        return QString::number(bytesize / GB);

    }else if(bytesize % MB == 0){
        setFsizeComboBox(2, comboidx);
        return QString::number(bytesize / MB);

    }else if(bytesize % KB == 0){
        setFsizeComboBox(1, comboidx);
        return QString::number(bytesize / KB);

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
