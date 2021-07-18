#ifndef TABLE_TEST
#define TABLE_TEST

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <basetable.h>
#include <basictable.h>
#include <stringtable.h>

using namespace testing;

class TableEvaluate {
public:
    TableEvaluate(QTableWidget *widget, int rowcount, int colcount){
        table = widget;
        rows = rowcount;
        cols = colcount;
    }

    ~TableEvaluate(){
        table = nullptr;
        rows = 0;
        cols = 0;
    }

    inline QString getIndexItem(int row, int col){
        return table->model()->index(row, col).data().toString();
    }

    void resetTestItems(){
        table->clear();
        table->setColumnCount(cols);
        table->setRowCount(rows);

        //set item 1 to 100
        //  1  2  3  4  5  6  7  8  9  10
        // 11 12 13 14 ..
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                table->setItem(i
                               , j
                               , new QTableWidgetItem(
                                   QString::number((i * cols) + (j + 1))));
            }
        }
    }

    void checkValueDelete(int *selected, int size){
        QVector<int> rownum;
        int sk = 0;
        for(int i = 0; i < rows; i++){
            if(sk < size
                && i == selected[sk]){
                sk++;
            }else{
                rownum.append(i);
            }
        }

        for(int i = 0; i < table->rowCount(); i++){
            for(int j = 0; j < cols; j++){
                EXPECT_EQ(getIndexItem(i, j).toStdString()
                    , QString::number(rownum.at(i) * cols + j + 1)
                                      .toStdString());
            }
        }
    }

    void checkTableUp(int selected){

        QVector<int> rownum;
        for(int i = 0; i < rows; i++){
            rownum.append(i);
        }

        int swapto = selected - 1;

        // If it gets a incorrect position, such as 0,
        // this function will do nothing.
        if(selected >= 0 && selected < rownum.count()
            && swapto >= 0 && swapto < rownum.count()){
            rownum.swapItemsAt(selected, swapto);
        }

        for(int i = 0; i < table->rowCount(); i++){
            for(int j = 0; j < cols; j++){
                EXPECT_EQ(getIndexItem(i, j).toStdString()
                              , QString::number(rownum.at(i) * cols + j + 1)
                                                .toStdString());
            }
        }
    }

    void checkTableDown(int selected){

        QVector<int> rownum;
        for(int i = 0; i < rows; i++){
            rownum.append(i);
        }

        int swapto = selected + 1;
        if(selected >= 0 && selected < rownum.count()
            && swapto >= 0 && swapto < rownum.count()){
            rownum.swapItemsAt(selected, swapto);
        }

        for(int i = 0; i < table->rowCount(); i++){
            for(int j = 0; j < cols; j++){
                EXPECT_EQ(getIndexItem(i, j).toStdString()
                              , QString::number(rownum.at(i) * cols + j + 1)
                                                .toStdString());
            }
        }
    }

    void checkTableInsert(int *copied, int size, int insertnum){
        QVector<int> rownum;
        for(int i = 0; i < rows; i++){
            rownum.append(i);
        }

        for(int s = (size - 1); s >= 0; s--){
            rownum.insert(insertnum, copied[s]);
        }

        EXPECT_EQ(table->rowCount(), rows + size);

        for(int i = 0; i < table->rowCount(); i++){
            for(int j = 0; j < cols; j++){
                EXPECT_EQ(getIndexItem(i, j).toStdString()
                              , QString::number(rownum.at(i) * cols + j + 1)
                                                .toStdString());
            }
        }
    }

private:
    QTableWidget *table;
    int rows;
    int cols;
};

//class StringTableTest: public ::testing::Test {
//protected:
//    virtual void SetUp() override {
//        table = new StringTable();
//        eval = new TableEvaluate(table, rows, cols);
//    }

//    virtual void TearDown() override {
//        delete table;
//        delete eval;
//    }

//    StringTable *table;
//    TableEvaluate *eval;
//    static constexpr int rows = 10;
//    static constexpr int cols = 3;

//};

//TEST_F(StringTableTest, addAction){
//    for(int i = 0; i < rows; i++){
//        table->addAction();
//        EXPECT_EQ(table->rowCount(), i+1);
//    }
//}

//TEST_F(StringTableTest, editAction){

//}

//TEST_F(StringTableTest, deleteAction){

//}

//TEST_F(StringTableTest, reloadAction){

//}

//TEST_F(StringTableTest, cutAction){

//}

//TEST_F(StringTableTest, copyAction){

//}

//TEST_F(StringTableTest, pasteAction){

//}

//TEST_F(StringTableTest, upAction){

//}

//TEST_F(StringTableTest, downAction){

//}

//TEST_F(StringTableTest, openFileAction){

//}

//TEST_F(StringTableTest, openDirectoryAction){

//}

//TEST_F(StringTableTest, saveAction){

//}

//TEST_F(StringTableTest, resave){

//}

class MockBasicTable : public BasicTable {
public:
    explicit MockBasicTable(QWidget *parent = nullptr){}
    virtual ~MockBasicTable() override{}

    void upAction(){ BasicTable::upSelected([](int row){ Q_UNUSED(row) }); }
    void downAction(){ BasicTable::downSelected([](int row){ Q_UNUSED(row) }); }
    void horizontalHeaderClicked(int cols){
        BasicTable::horizontalHeaderClicked(cols);
    }

    // Add QAction
    void setPopupActionDefault(QIcon copy, QIcon up, QIcon down){
        BasicTable::setPopupActionDefault(copy, up, down);
    }

    // It needs GUI selection.
    bool deleteCheckMessage(){
        return BasicTable::deleteCheckMessage();
    }

    enum class ACTION{
        NEWFILE, ADD, REMOVE, EDIT, CLEAR,
        ENABLE, DISABLE, FILE, FILES, FOLDER, FILEINFO,
        CUT, COPY, PASTE, PASTESPACE, PASTENEWLINE, UP, DOWN,
        REFRESH, PROPERTY};

    // return const variables.
    QIcon getIcon(BasicTable::ACTION action){
        return BasicTable::getIcon(action);
    }

    // return const variables.
    QString getActionText(BasicTable::ACTION action){
        return BasicTable::getActionText(action);
    }

    // It needs GUI selection.
    QString selectFile(QString basedir){
        return BasicTable::selectFile(basedir);
    }

    // It needs GUI selection.
    QString selectFolder(QString basedir){
        return BasicTable::selectFolder(basedir);
    }

    QAction *addTableAction(BasicTable::ACTION action,
                            int keycode1 = 0,
                            int keycode2 = 0,
                            int keycode3 = 0,
                            int keycode4 = 0){
        return BasicTable::addTableAction(action
                                          , keycode1
                                          , keycode2
                                          , keycode3
                                          , keycode4);
    }

    // This class cannot use Q_OBJECT macro
    void installFilter(){
        BasicTable::installClipboardFilter(this);
    }

//    QClipboard not work
//    void copyToClipboard(){
//        BasicTable::copyToClipboard(QApplication::clipboard());
//    }

//    QClipboard not work
//    void pasteFromClipboard(){
//        BasicTable::pasteFromClipboard(QApplication::clipboard());
//    }

    const QList<QStringList> tableSelectionToList(){
        return BasicTable::tableSelectionToList();
    }

    bool insertItemFromList(const QList<QStringList> &list){
        return BasicTable::insertItemFromList(list);
    }

    QString getMimeTypeName() { return mimetypeclass; }
};

class BasicTableTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        table = new MockBasicTable();
        eval = new TableEvaluate(table, rows, cols);
        eval->resetTestItems();
    }

    virtual void TearDown() override {
        delete table;
        delete eval;
    }

    MockBasicTable *table;
    TableEvaluate *eval;
    static constexpr int rows = 10;
    static constexpr int cols = 10;
};

TEST_F(BasicTableTest, up){
    // up single index
    for(int i = 0; i < rows; i++){
        table->selectRow(i);
        table->upAction();
        eval->checkTableUp(i);
        eval->resetTestItems();
    }
}

TEST_F(BasicTableTest, up_no_selection){
    // up single index
    for(int i = 0; i < rows; i++){
        table->clearSelection();
        table->upAction();
    }
}

TEST_F(BasicTableTest, down){
    // down single index
    for(int i = 0; i < rows; i++){
        table->selectRow(i);
        table->downAction();
        eval->checkTableDown(i);
        eval->resetTestItems();
    }
}

TEST_F(BasicTableTest, down_no_selection){
    // down single index
    for(int i = 0; i < rows; i++){
        table->clearSelection();
        table->downAction();
    }
}

//TEST_F(BasicTableTest, deletecheckmessage){
//    table->deleteCheckMessage();
//}

TEST_F(BasicTableTest, horizontalheaderclick){
    table->horizontalHeaderClicked(0);
    QModelIndexList list = table->selectionModel()->selectedIndexes();
    int i = 0;
    for(const QModelIndex &item : list){
        i++;
        EXPECT_EQ(i, item.data().toInt());
    }
}

TEST_F(BasicTableTest, installClipboardFilter){
    table->installFilter();

    // If the test class does not have a Q_OBJECT macro,
    // metaObject will return the name of the parent class
    // that has the macro.
    EXPECT_EQ(table->getMimeTypeName().toStdString(), "BasicTable");
}

TEST_F(BasicTableTest, copyPaste_Multiple){
    table->installFilter();
    QTableWidgetSelectionRange range(0, 0, 2, cols - 1);
    table->setRangeSelected(range, true);

    const QList<QStringList> list = table->tableSelectionToList();

    table->clearSelection();
    table->selectRow(4);

    table->insertItemFromList(list);

    int test[] = { 0, 1, 2 };
    eval->checkTableInsert(test, 3, 4);
}

class MockBaseTable : public BaseTable {
public:
    explicit MockBaseTable(QWidget *parent = nullptr) {
        setSelectionBehavior(QTableWidget::SelectRows);
        setSelectionMode(QTableWidget::ExtendedSelection);
    }
    virtual ~MockBaseTable() override {}

    void deleteTableRecursive(){ BaseTable::deleteTableRecursive(); }

//    not used
//    void dropEvent(QDropEvent *event) {
//        BaseTable::dropEvent(event);
//    }

//    not used
//    void dragEnterEvent(QDragEnterEvent *event) {
//        BaseTable::dragEnterEvent(event);
//    }

//    not used
//    bool eventFilter(QObject *obj, QEvent *event) {
//        return BaseTable::eventFilter(obj, event);
//    }

    void swapTableRow(int from, int dest){
        BaseTable::swapTableRow(from, dest);
    }

    QStringList droppedFromOutside(QDropEvent *event){
        return BaseTable::droppedFromOutside(event);
    }

//    bool insideDropRowsMove(QDropEvent *event
//                            , QList<int> *selected = nullptr){
//        return BaseTable::insideDropRowsMove(event, selected);
//    }

    bool droppedFromInside(int dropped
                           , QList<int> *selected = nullptr){
        return BaseTable::droppedFromInside(dropped, selected);;
    }
};


class BaseTableTest : public ::testing::Test {
protected:
    virtual void SetUp() override {
        table = new MockBaseTable();
        resetTestItems();
    }

    virtual void TearDown() override {
        delete table;
    }

    inline QString getIndexItem(int row, int col){
        return table->model()->index(row, col).data().toString();
    }

    void resetTestItems(){
        table->clear();
        table->setColumnCount(cols);
        table->setRowCount(rows);

        //set item 1 to 100
        //  1  2  3  4  5  6  7  8  9  10
        // 11 12 13 14 ..
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                table->setItem(i
                               , j
                               , new QTableWidgetItem(
                                   QString::number((i * cols) + (j + 1))));
            }
        }
    }

    void checkValueDelete(int *selected, int size){
        QVector<int> rownum;
        int sk = 0;
        for(int i = 0; i < rows; i++){
            if(sk < size
                && i == selected[sk]){
                sk++;
            }else{
                rownum.append(i);
            }
        }

        EXPECT_EQ(table->rowCount(), rows - size);

        for(int i = 0; i < table->rowCount(); i++){
            for(int j = 0; j < cols; j++){
                EXPECT_EQ(getIndexItem(i, j).toStdString()
                              , QString::number(rownum.at(i) * cols + j + 1).toStdString());
            }
        }
    }

    MockBaseTable *table;
    static constexpr int rows = 10;
    static constexpr int cols = 10;
};

TEST_F(BaseTableTest, deleteRowRecursive_Single){
    // delete single index
    for(int i = 0; i < rows; i++){
        table->selectRow(i);
        table->deleteTableRecursive();
        int select[] = { i };
        checkValueDelete(select, 1);
        resetTestItems();
    }
}

TEST_F(BaseTableTest, deleteRowRecursive_multiContinuous){
    // multi continuous selection and delete

    // first
    QTableWidgetSelectionRange range(0, 0, 2, cols - 1);
    table->setRangeSelected(range, true);
    table->deleteTableRecursive();

    int test1[] = { 0, 1, 2 };
    checkValueDelete(test1, 3);

    resetTestItems();

    // last
    QTableWidgetSelectionRange range2(rows - 1, 0, rows - 3, cols - 1);
    table->setRangeSelected(range2, true);
    table->deleteTableRecursive();

    int test2[] = { rows - 3, rows - 2, rows - 1 };
    checkValueDelete(test2, 3);

    resetTestItems();

    // middle
    constexpr int center = rows / 2;
    QTableWidgetSelectionRange range3(center - 1, 0, center + 1, cols - 1);
    table->setRangeSelected(range3, true);
    table->deleteTableRecursive();

    int test3[] = { center - 1, center, center + 1 };
    checkValueDelete(test3, 3);

}

TEST_F(BaseTableTest, deleteRowRecursive_multiIndependent){
    // multi independent selection and delete

    auto rowSelect = [=](int row, QItemSelectionModel *model){
        for(int i = 0; i < cols; i++){
            model->select(table->model()->index(row,i)
                              , QItemSelectionModel::Select);
        }
    };

    QItemSelectionModel *model = table->selectionModel();

    int test1[] = {0, 2, 5};

    for(int &i : test1){
        rowSelect(i, model);
    }
    table->deleteTableRecursive();

    EXPECT_EQ(table->rowCount(), rows - 3);
    checkValueDelete(test1, 3);

    resetTestItems();

    model = table->selectionModel();

    // It has to put ascend order.
    int delandskip[] = {1, 4, rows - 1};

    for(int &i : delandskip){
        rowSelect(i, model);
    }
    table->deleteTableRecursive();

    EXPECT_EQ(table->rowCount(), rows - (sizeof(delandskip) / sizeof (int)));
    checkValueDelete(delandskip, 3);

    resetTestItems();
}

TEST_F(BaseTableTest, deleteRowRecursive_multiMixed){
    // multi selection mixed (independent and continuous selection)

    auto rowSelect = [=](int row, QItemSelectionModel *model){
        for(int i = 0; i < cols; i++){
            model->select(table->model()->index(row,i)
                              , QItemSelectionModel::Select);
        }
    };

    QItemSelectionModel *model = table->selectionModel();

    int test1[] = {0, 2, 3, 9};

    for(int &i : test1){
        rowSelect(i, model);
    }
    table->deleteTableRecursive();

    EXPECT_EQ(table->rowCount(), rows - 4);
    checkValueDelete(test1, 4);

    resetTestItems();

}

TEST_F(BaseTableTest, deleteRowRecursive_all){
    // delete all items
    QTableWidgetSelectionRange range(0, 0, rows - 1, cols - 1);
    table->setRangeSelected(range, true);
    table->deleteTableRecursive();

    EXPECT_EQ(table->rowCount(), 0);
}

TEST_F(BaseTableTest, swapTableRow){

    auto swapCheck = [=](int from, int to){
        for(int j = 0; j < cols; j++){
            EXPECT_EQ(getIndexItem(to, j).toStdString()
                , QString::number(from * cols + j + 1).toStdString());
            EXPECT_EQ(getIndexItem(from, j).toStdString()
                , QString::number(to * cols + j + 1).toStdString());
        }
    };

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < rows; j++){
            table->swapTableRow(i, j);
            swapCheck(i, j);
            resetTestItems();
        }
    }
}

TEST_F(BaseTableTest, insideDropMove){
    // Supports only single selection
    // and multiple consecutive selections.
    // TODO: These tests are insufficient.

    auto rowSelect = [=](int row, QItemSelectionModel *model){
        for(int i = 0; i < cols; i++){
            model->select(table->model()->index(row,i)
                              , QItemSelectionModel::Select);
        }
    };

    auto checkdown = [=](int dest, QList<int> move){
        QList<int> indice;
        for(int i = 0; i < rows; i++){
            indice.append(i);
        }

        for(auto &del : move){
            indice.removeOne(del);
        }

        std::for_each(move.rbegin(), move.rend(), [&](int v) {
            indice.insert(dest - move.count(), v);
        });

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                EXPECT_EQ(getIndexItem(i, j).toStdString()
                        , QString::number(indice.at(i) * cols + j + 1).toStdString());
            }
        }
    };

    auto checkup = [=](int dest, QList<int> move){
        QList<int> indice;
        for(int i = 0; i < rows; i++){
            indice.append(i);
        }

        for(auto &del : move){
            indice.removeOne(del);
        }

        std::for_each(move.rbegin(), move.rend(), [&](int v) {
            indice.insert(dest, v);
        });

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                EXPECT_EQ(getIndexItem(i, j).toStdString()
                              , QString::number(indice.at(i) * cols + j + 1).toStdString());
            }
        }
    };

    QItemSelectionModel *model = table->selectionModel();

    // up to down
    QList<int> test1 = {0, 1, 2, 3};

    for(int in = 4; in < rows; in++){
        for(const int &i : test1){ rowSelect(i, model); }
        QList<int> result;
        table->droppedFromInside(in, &result);
        checkdown(in, result);
        resetTestItems();
    }

    // down to up
    QList<int> test3 = {rows - 4, rows - 3, rows - 2, rows - 1};

    for(int in = 0; in < (rows - 4); in++){
        for(const int &i : test3){ rowSelect(i, model); }
        QList<int> result;
        table->droppedFromInside(in, &result);
        checkup(in, result);
        resetTestItems();
    }

    constexpr int middle = rows / 2;
    QList<int> test5 = {middle - 1, middle, middle + 1};

    // middle to up
    for(int in = 0; in < (middle - 1); in++){
        for(const int &i : test5){ rowSelect(i, model); }
        QList<int> result;
        table->droppedFromInside(in, &result);
        checkup(in, result);
        resetTestItems();
    }

    // middle to down
    for(int in = (middle + 2); in < rows; in++){
        for(const int &i : test5){ rowSelect(i, model); }
        QList<int> result;
        table->droppedFromInside(in, &result);
        checkdown(in, result);
        resetTestItems();
    }
}

//TEST(guitest, unittest2)
//{
//    QTableWidget *widget = new QTableWidget();
//    widget->setColumnCount(1);
//    widget->setRowCount(10);
//    widget->setItem(0,0,new QTableWidgetItem("test"));

//    EXPECT_EQ(widget->model()->index(0,0).data().toString(), "test");
//}

#endif // TABLE_TEST
