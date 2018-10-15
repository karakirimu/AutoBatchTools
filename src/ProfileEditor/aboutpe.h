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
    explicit AboutPE(QWidget *parent = nullptr);
    ~AboutPE();

private:
    Ui::AboutPE *ui;
};

#endif // ABOUTPE_H
