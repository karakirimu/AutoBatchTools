#ifndef ABOUTPE_H
#define ABOUTPE_H

#include <QWidget>

namespace Ui {
class AboutPE;
}

class AboutPE : public QWidget
{
    Q_OBJECT

public:
    explicit AboutPE(QWidget *parent = 0);
    ~AboutPE();

private:
    Ui::AboutPE *ui;
};

#endif // ABOUTPE_H
