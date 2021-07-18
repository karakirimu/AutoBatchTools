#include "xmlbuilder.test.h"
#include "table.test.h"
#include "xmllistgenerator.test.h"

#include <gtest/gtest.h>

#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
    QApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    QTimer::singleShot(0, [&](){
        auto res = RUN_ALL_TESTS();
        app.exit(res);
    });

    return app.exec();
}
