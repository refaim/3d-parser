#include <QtGui/QApplication>
#include "mainwindow.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#ifdef _MSC_VER
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd) {
    return main(0, 0);
}
#endif
