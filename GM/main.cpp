#include "gm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GM w;
	w.show();
	
	return a.exec();
}
