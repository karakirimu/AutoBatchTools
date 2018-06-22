#include "profileeditor.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set default text codec
#ifdef Q_OS_WIN
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift_JIS"));
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
#endif

//    QTranslator myappTranslator;
//    myappTranslator.load("peditor_ja_JP");
//    a.installTranslator(&myappTranslator);

    ProfileEditor w(a.arguments());
    w.show();

    return a.exec();
}
