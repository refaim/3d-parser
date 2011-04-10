#include <string>

#include <QFileDialog>
#include <QMessageBox>

#include "assimp.hpp"
#include "aiScene.h"
#include "aiPostProcess.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    viewer(new Viewer(this))
{
    ui->setupUi(this);
    setCentralWidget(viewer);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete viewer;
}

void MainWindow::openFileDialog()
{
    std::string extensions;
    Assimp::Importer importer;
    importer.GetExtensionList(extensions);
    const QString filename = QFileDialog::getOpenFileName(this, tr("Open 3D model"), "", QString::fromStdString(extensions));

    const aiScene *scene = importer.ReadFile(filename.toStdString(), aiProcess_GenNormals);
    if (!scene)
    {
        QMessageBox::critical(this, "Importer error", importer.GetErrorString());
    }
}
