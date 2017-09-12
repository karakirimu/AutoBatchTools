#include "profileeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProfileEditor w;
    w.show();

    return a.exec();
}
