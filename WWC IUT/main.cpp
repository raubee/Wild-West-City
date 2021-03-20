#include "Window.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CWindow w;
	QMessageBox message;
	QPushButton oui("Oui");
	QPushButton non("Non");

	message.setText("Veux-tu jouer en plein écran ?");
	message.addButton(&oui, QMessageBox::YesRole);
	message.addButton(&non, QMessageBox::NoRole);
	message.exec();

	if (message.clickedButton() == &oui)
	{
		w.showFullScreen();
	}
	else w.show();
	return a.exec();
}