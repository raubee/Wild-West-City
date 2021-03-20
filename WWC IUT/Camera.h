/*
* Auteur :Aubree Lois
* Usage : Gestion complète de la camera.
*         Le constructeur nessecite un objet newton pour que la camera pointe sur celui-ci
*		  La methode Dessiner affiche la camera dans la scene
*         
*/
#pragma once
#include "Vector.h"
#include "Newton/Newton.h"
#include "Objet.h"

class CCamera : public CObjet
{
public:
	CCamera(void);
	CCamera(GLfloat center_x, GLfloat center_y,GLfloat center_z,GLfloat haut_x,GLfloat haut_y,GLfloat haut_z,double loin);
	CCamera(CObjet * Center_body,GLfloat haut_x,GLfloat haut_y,GLfloat haut_z,double loin);

	void setPosition(CVector3 pos);
	void setCenter(CVector3 center);
	void setHaut(CVector3 haut);
	void setTheta(GLfloat angle);

	CVector3 getPosition(){return m_vPos_Cam;};
	CVector3 getCenter(){return m_vCenter_Cam;};
	CVector3 getHaut(){return m_vHaut_Cam;};
	GLdouble getRoll(){return m_dRoll;};
	GLdouble getPitch(){return m_dPitch;};
	GLdouble getHeading(){return m_dHeading;};
	GLdouble getTheta(){return m_dTheta;};
	GLdouble getPhi(){return m_dPhi;};
	GLdouble getDistance(){return m_dR;};

	void calculposition(int x , int y,GLdouble w,GLdouble h); //Calcul position de la camera après mouvement en x et y
	void calculcentre(); // calcul du pointage de la camera
	void Dessiner(); // Heritée de Objet, Dessiner place la camera dans la scene
	void zoomer(GLfloat zoom); // Gestion du zoom ( rapprochement de la camera par rapport à l'objet centre

	// Instancier la camera comme objet newton
	void Initialiser(NewtonWorld *nWorld,CVector3 taille);

	~CCamera(void);

private:
	CVector3 m_vPos_Cam; // vecteur position
	CVector3 m_vCenter_Cam; // vecteur centrage
	CVector3 m_vHaut_Cam; // vecteur haut
	CObjet * m_nCenterObj; // objet centré par la camera
	GLdouble m_dRoll, m_dPitch, m_dHeading ; // recuperation des angles selon theta, pitch, heading
	GLdouble m_dTheta,m_dPhi,m_dR; // angle de placement de la camera et R longueur entre objet/camera
	CVector3 m_vTailleCollisionCam; // Taille de la boite de collision camera
	double m_dFar;
	
};
