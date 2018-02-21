#include "profileeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set default text codec
#ifdef Q_OS_WIN
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift_JIS"));
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
#endif

    ProfileEditor w(a.arguments());
    w.show();

    return a.exec();
}
