/*
*
* Auteur : Aubree Lois 
*
* Usage : La classe Menu permet l'affichage des différentes textures 2D en plein ecran
*		  Elle permet l'affichage des pages du menu puis des instructions des panneaux
*
*/
#pragma once
#include <QImage>
#include <QGLWidget>
#include <QString>

class Menu : public QGLWidget
{
public:
	Menu(void);
	~Menu(void);
	void afficher();
	void loadImage(QString name);
	bool m_bMenu;
	int m_nStat;
	int m_nNumText;
	int m_nTextCourante;
	QString m_sAffichage_temps;
private:
	GLuint m_nTextMenu[20] ;
	
	
	
};
