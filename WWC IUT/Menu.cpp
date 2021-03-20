#include "Menu.h"

Menu::Menu(void)
{
	for (int i =0 ; i<20 ; i++)
		m_nTextMenu[i] =0;
	m_bMenu = true;
	m_nStat = 0;
	m_nNumText =0;
}

Menu::~Menu(void)
{
}
void Menu::loadImage(QString name)
{
	//On charge image dont le nom est m_Sname
	QImage t;
	QImage b;
	if(!b.load(name))
	{
		return;
	}
	// On converti l'image chargée en format OPengl
	t = QGLWidget::convertToGLFormat( b );
	glGenTextures( 1, &m_nTextMenu[m_nNumText] );
	glBindTexture( GL_TEXTURE_2D, m_nTextMenu[m_nNumText] );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	m_nNumText++;
}
void Menu::afficher()
{
	
	//On charge la matrice de projection puis on passe en affichage 2D.
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPopMatrix();

	
	//On charge la texture pour l'affichage
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_nTextMenu[m_nTextCourante]);
	glPushMatrix();

	//On desactive les lumieres
	glDisable(GL_LIGHTING);
	//Afficher images
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex2f(-1,-1);
		glTexCoord2f(0,1);glVertex2f(-1,1);
		glTexCoord2f(1,1);glVertex2f(1,1);
		glTexCoord2f(1,0);glVertex2f(1,-1);
	glEnd();
	glPopMatrix();
}