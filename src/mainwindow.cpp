#include <string>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"

QString SUPPORTED_FORMATS[] = {
    "3D GameStudio Model (*.mdl) (*.mdl)",
    "3D GameStudio Terrain (*.hmp) (*.hmp)",
    "3D Studio Max ASE (*.ase, *.ask) (*.ase *.ask)",
    "3D Studio Max Project (*.3ds, *.prj) (*.3ds *.prj)",
    "AC3D (*.ac, *.ac3d, *.acc) (*.ac *.ac3d *.acc)",
    "AutoCAD DXF (*.dxf) (*.dxf)",
    "Autodesk DXF (*.dxf) (*.dxf)",
    "B3D (*.b3d) (*.b3d)",
    "Biovision BVH (*.bvh) (*.bvh)",
    "Blender (*.blend) (*.blend)",
    "CharacterStudio Motion (*.csm) (*.csm)",
    "Collada (*.dae, *.xml) (*.dae *.xml)",
    "DirectX X (*.x) (*.x)",
    "Doom 3 (*.md5mesh, *.md5anim, *.md5camera) (*.md5mesh *.md5anim *.md5camera)",
    "Irrlicht Mesh (*.irrmesh, *.xml) (*.irrmesh *.xml)",
    "Irrlicht Scene (*.irr, *.xml) (*.irr *.xml)",
    "LightWave Model (*.lwo) (*.lwo)",
    "LightWave Scene (*.lws, *.mot) (*.lws *.mot)",
    "Milkshape 3D (*.ms3d) (*.ms3d)",
    "Modo Model (*.lxo) (*.lxo)",
    "Nendo (*.ndo) (*.ndo)",
    "Neutral File Format (*.enff, *.nff) (*.enff *.nff)",
    "Object File Format (*.off) (*.off)",
    "Ogre (*.mesh.xml, *.skeleton.xml, *.material) (*.mesh.xml *.skeleton.xml *.material)",
    "Quake I (*.mdl) (*.mdl)",
    "Quake II (*.md2) (*.md2)",
    "Quake III (*.md3) (*.md3)",
    "Quake III BSP (*.pk3) (*.pk3)",
    "Quick3D (*.q3o, *q3s) (*.q3o *q3s)",
    "Raw Triangles (*.raw) (*.raw)",
    "Return to Castle Wolfenstein (*.mdc) (*.mdc)",
    "Sense8 WorldToolkit (*.nff) (*.nff)",
    "Stanford Ply (*.ply) (*.ply)",
    "Stanford Polygon Library (*.ply) (*.ply)",
    "Stereolithography (*.stl) (*.stl)",
    "Terragen Terrain (*.ter) (*.ter)",
    "TrueSpace (*.cob, *.scn) (*.cob *.scn)",
    "Unreal (*.3d, *.uc) (*.3d *.uc)",
    "Valve Model (*.smd, *.vta) (*.smd *.vta)",
    "Wavefront Object (*.obj) (*.obj)",
    "All Files (*.*) (*.*)",
};
#define FORMATS_COUNT sizeof(SUPPORTED_FORMATS) / sizeof(SUPPORTED_FORMATS[0])

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    viewer(this)
{
    ui.setupUi(this);
    setCentralWidget(&viewer);
    viewer.setFocus();
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));
}

void MainWindow::openFileDialog()
{
    std::string extensions;
    QString filter, filterSeparator = ";;";

    viewer.sceneModel.GetExtensionList(extensions);
    filter = tr("All suported formats (%1)%2").arg(
        QString::fromStdString(extensions).replace(';', ' '),
        filterSeparator);

    for (unsigned int i = 0; i < FORMATS_COUNT; ++i)
    {
        filter += SUPPORTED_FORMATS[i] + filterSeparator;
    }

    const QString filename = QFileDialog::getOpenFileName(this,
        tr("Open 3D model"),
        "",
        filter,
        0,
        QFileDialog::HideNameFilterDetails);

    if (!filename.length()) // Dialog canceled
        return;

    try
    {
        viewer.loadScene(filename.toStdString());
    }
    catch (ImportError error)
    {
        QMessageBox::critical(this, "Importer error", error.what());
    }
}