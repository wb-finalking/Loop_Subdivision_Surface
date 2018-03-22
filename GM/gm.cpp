#include "gm.h"

GM::GM(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle(tr("subdivision surface"));

	createActions();
	createMenus();
	
	currentTimeLabel = new QLabel(this); 
	//currentTimeLabel->setText("wb");
	statusBar()->addWidget(currentTimeLabel);

	/*render = NULL;*/
	render = new Render(this);
	setCentralWidget(render);
	render->resize(800, 800);


}

GM::~GM()
{
	delete render;
	delete currentTimeLabel;
}

void GM::createActions()
{
	openObjAct = new QAction(tr("&Open Obj File"), this);
	openObjAct->setShortcut(QKeySequence::Open);
	openObjAct->setStatusTip(tr("Select Obj File"));
	connect(openObjAct, SIGNAL(triggered()), this, SLOT(openObj()));

	saveObjAct = new QAction(tr("&Save Obj File"), this);
	saveObjAct->setShortcut(QKeySequence::Save);
	saveObjAct->setStatusTip(tr("Select Obj File"));
	connect(saveObjAct, SIGNAL(triggered()), this, SLOT(saveObj()));
}

void GM::createMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	//fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openObjAct);
	fileMenu->addAction(saveObjAct);
	menuBar()->addMenu(fileMenu);
}

void GM::openObj()
{
	QString source = QFileDialog::getOpenFileName(this,
		tr("Open Obj File"), QDir::currentPath(), tr("Image Files (*.obj)"));
	std::string filename = source.toStdString();

	currentTimeLabel->setText("Reading obj File");
	if (ReadObj::readfile(filename)) {
		Render::vertices.clear();
		Render::vertices = std::move(ReadObj::getVertices());

		Render::halfEdges.clear();
		Render::halfEdges = std::move(ReadObj::getHalfEdges());

		Render::faces.clear();
		Render::faces = std::move(ReadObj::getFaces());
		
	}
	currentTimeLabel->setText("Reading obj File Over");
	render->repaint();
}

void GM::saveObj()
{
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save Image"), "../", tr("*.obj"));

	std::ofstream file(filename.toStdString());
	for (int i = 0; i < render->vertices.size(); i++)
	{
		ostringstream stringout;
		stringout << "v " << Render::vertices[i].vector->x << " " << -Render::vertices[i].vector->z
			<< " " << Render::vertices[i].vector->y << endl;
		string s = stringout.str();
		file << s;
	}
	for (int i = 0; i < Render::faces.size(); i++)
	{
		ostringstream stringout;
		stringout << "f " << Render::faces[i].verts[0] + 1 << "/" << Render::faces[i].verts[0] + 1 << " "
			<< Render::faces[i].verts[1] + 1 << "/" << Render::faces[i].verts[1] + 1 << " "
			<< Render::faces[i].verts[2] + 1 << "/" << Render::faces[i].verts[2] + 1 << endl;
		string s = stringout.str();
		file << s;
	}
	file.close();
}

void GM::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F)
	{
		Render::wireframeEnabled = !Render::wireframeEnabled;
	}
	render->update();
}
