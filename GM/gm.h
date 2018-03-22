#ifndef GM_H
#define GM_H

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include "HalfEdge.h"
#include "ui_gm.h"
#include "render.h"

class GM : public QMainWindow
{
	Q_OBJECT

public:
	GM(QWidget *parent = 0);
	~GM();

private:
	Ui::GMClass ui;
	Render *render;

	QMenu * fileMenu;
	QAction * openObjAct;
	QAction * saveObjAct;
	QLabel *currentTimeLabel;

	void createActions();
	void createMenus();

	private slots:
	void openObj();
	void saveObj();

protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif // GM_H
