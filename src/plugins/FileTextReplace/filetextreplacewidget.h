#ifndef FILETEXTREPLACEWIDGET_H
#define FILETEXTREPLACEWIDGET_H

#include <QWidget>

namespace Ui {
class FileTextReplaceWidget;
}

class FileTextReplaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileTextReplaceWidget(QWidget *parent = nullptr);
    ~FileTextReplaceWidget();

private:
    Ui::FileTextReplaceWidget *ui;
};

#endif // FILETEXTREPLACEWIDGET_H
