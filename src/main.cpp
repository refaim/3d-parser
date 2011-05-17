#include <QtGui/QApplication>
#include <QtOpenGL>
#include "mainwindow.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(4); // x4 antialiasing is default now
    QGLFormat::setDefaultFormat(glf);

    MainWindow w;
    w.show();

    return a.exec();
}

#ifdef _MSC_VER
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd) {
    return main(0, 0);
}
#endif
