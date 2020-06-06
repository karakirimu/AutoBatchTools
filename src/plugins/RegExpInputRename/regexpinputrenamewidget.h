#ifndef REGEXPINPUTRENAMEWIDGET_H
#define REGEXPINPUTRENAMEWIDGET_H

#include <QWidget>

namespace Ui {
class RegExpInputRenameWidget;
}

class RegExpInputRenameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegExpInputRenameWidget(QWidget *parent = nullptr);
    ~RegExpInputRenameWidget();

private:
    Ui::RegExpInputRenameWidget *ui;
};

#endif // REGEXPINPUTRENAMEWIDGET_H
