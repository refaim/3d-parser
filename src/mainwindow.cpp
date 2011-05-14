#include <string>

#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    viewer(this)
{
    ui.setupUi(this);
    setCentralWidget(&viewer);
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));
}

void MainWindow::openFileDialog()
{
    std::string extensions;
    viewer.sceneModel.GetExtensionList(extensions);
    const QString filename = QFileDialog::getOpenFileName(this,
        tr("Open 3D model"),
        "",
        QString::fromStdString(extensions));

    if (!filename.length()) // Dialog canceled
        return;

    try
    {
        viewer.sceneModel.loadScene(filename.toStdString());
    }
    catch (ImportError error)
    {
        QMessageBox::critical(this, "Importer error", error.what());
    }
}
