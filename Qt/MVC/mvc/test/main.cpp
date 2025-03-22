//#include "MainWindow.h"
#include <QApplication>
#include "log.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();

    SetLogFunc(Log2File);
    SetLogDir(QCoreApplication::applicationDirPath().toStdString().c_str());
    qDebug() << __BASE_FILE__;
    LOG_INFO("exe filePath = %s", argv[0]);
    LOG_ERR("hello %d", 2024);
    LOG_ERR("hello %d", 2024);
    LOG_ERR("hello %d", 2024);
    LOG_ERR("hello %d", 2024);
    LOG_INFO("info log %d, %s", 2024, "haha");
    return 0;
}
