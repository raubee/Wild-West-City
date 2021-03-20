/*
*
* Auteur : Aubree Lois et Loic Teyssier
* 
* Classe CMWindow dérivée de CWidgetOGL
* Usage :	Initialise OpenGL
*			Gere l'affichage.
*
*/

#ifndef WINDOW_H
#define WINDOW_H

#include "WidgetOGL.h"

class CWindow : public CWidgetOGL
{
	Q_OBJECT

public:
	CWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CWindow();

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

};

#endif // WINDOW_H
