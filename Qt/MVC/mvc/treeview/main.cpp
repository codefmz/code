#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss} [%{type}] %{file} [%{function}(%{line})]  %{message}");
    MainWindow w;
    w.show();
    return a.exec();
}
