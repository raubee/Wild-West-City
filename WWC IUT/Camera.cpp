#include "Camera.h"
#include "Matrix.h"
#include <math.h>

CCamera::CCamera(void)
{
}

CCamera::~CCamera(void)
{
}
CCamera::CCamera(GLfloat center_x, GLfloat center_y,GLfloat center_z,GLfloat haut_x,GLfloat haut_y,GLfloat haut_z,double loin)
	: m_dRoll(0), m_dPitch(0), m_dHeading(0), m_dPhi(0),m_dTheta(0)
{
	m_dFar=loin;
	m_vPos_Cam.setCoordonnees(0,0,0);
	m_vCenter_Cam.setCoordonnees(center_x,center_y,center_z);
	m_vHaut_Cam.setCoordonnees(haut_x,haut_y,haut_z);
	calculposition(0,0,0,0);
}
CCamera::CCamera(CObjet * Center_obj ,GLfloat haut_x,GLfloat haut_y,GLfloat haut_z,double loin)
: m_dRoll(0), m_dPitch(0), m_dHeading(0), m_dPhi(0), m_dTheta(0)
{
	m_dFar=loin;
	m_nCenterObj= Center_obj;
	calculcentre();
	m_vPos_Cam.setCoordonnees(0,0,0);
	m_vHaut_Cam.setCoordonnees(haut_x,haut_y,haut_z);
	calculposition(0,0,0,0);
}
void CCamera::setCenter(CVector3 center)
{
	m_vCenter_Cam.setCoordonnees(center.x,center.y,center.z);
}
void CCamera::setHaut(CVector3 haut)
{
	m_vHaut_Cam.setCoordonnees(haut.x,haut.y,haut.z);
}
void CCamera::setPosition(CVector3 pos)
{
	m_vPos_Cam.setCoordonnees(pos.x,pos.y,pos.z);
}
void CCamera::calculposition(int x, int y,GLdouble w,GLdouble h)
{
	// recuperation des changement en x et y
	m_dTheta=(x-w)/100;
	m_dPhi=(y-h)/100;
	
	// Calcul de R
	GLdouble X2=(m_dFar-m_vPos_Cam.x)*(m_dFar-m_vPos_Cam.x);
	GLdouble Y2=(m_dFar-m_vPos_Cam.y)*(m_dFar-m_vPos_Cam.y);
	GLdouble Z2=(m_dFar-m_vPos_Cam.z)*(m_dFar-m_vPos_Cam.z);
	m_dR=sqrt(X2+Y2);
	
}
void CCamera::calculcentre()
{
	//Recuperation du centre de l'objet Newton pointé
	if(m_nCenterObj!=NULL){
		CMatrix LockTmp;
		NewtonBodyGetMatrix(m_nCenterObj->GetBody(),&LockTmp.m_Mat[0][0]);
		m_vCenter_Cam = LockTmp.getPosition();
	}
}
void CCamera::Dessiner()
{
	calculcentre();
		if (m_dPhi > 1.5)
			m_dPhi=1.5;
		if (m_dPhi < -1.0)
			m_dPhi=-1.0;

	// Calcul des coordonnées polaires
	m_dPitch =m_dR*cos(m_dPhi)*cos(m_dTheta);
	m_dRoll =m_dR*cos(m_dPhi)*sin(m_dTheta);
	m_dHeading =m_dR*sin(m_dPhi);
	
	//Empecher le  retour Camera
	if(m_dHeading <0)
		m_dHeading = 0;

	//Mise a jour de la matrice Newton de la camera
	CMatrix matrice_Collision_Cam;
	NewtonBodyGetMatrix(GetBody(), &matrice_Collision_Cam.m_Mat[0][0]);

	matrice_Collision_Cam.m_Mat[3][0] = m_vCenter_Cam.x+m_dPitch;
	matrice_Collision_Cam.m_Mat[3][1] = m_vCenter_Cam.y+m_dHeading;
	matrice_Collision_Cam.m_Mat[3][2] = m_vCenter_Cam.z+m_dRoll;

	NewtonBodySetMatrix(GetBody(), &matrice_Collision_Cam.m_Mat[0][0]);

	//Placer la camera
	gluLookAt(matrice_Collision_Cam.getPosition().x,matrice_Collision_Cam.getPosition().y,matrice_Collision_Cam.getPosition().z,m_vCenter_Cam.x,m_vCenter_Cam.y+1.5,m_vCenter_Cam.z,  m_vHaut_Cam.x,m_vHaut_Cam.y,m_vHaut_Cam.z);
}
void CCamera::zoomer(GLfloat zoom)
{
	m_dFar-=zoom;
	if(m_dFar<2)
		m_dFar=2;
	Dessiner();
}
void CCamera::setTheta(GLfloat angle)
{
	m_dTheta = angle;
}
void CCamera::Initialiser(NewtonWorld *nWorld,CVector3 taille)
{
	// On initialise le vecteur de dimensions
   m_vTailleCollisionCam.x = taille.x;
   m_vTailleCollisionCam.y = taille.y;
   m_vTailleCollisionCam.z = taille.z;


   CMatrix matrice; // On créé une matrice
   matrice.setIdentite();

   // On définit la matrice de manière à ce que l'objet soit placé aux positions
   // spécifiées en utilisant la dernière colonne de la matrice
   matrice.m_Mat[3][0] = 0;
   matrice.m_Mat [3][1] = 0;
   matrice.m_Mat [3][2] = 0;

   // On initialise la boîte de collision
   NewtonCollision * collision = NULL;
   // On créé la boite de collision aux dimensions de l'objet
   collision = NewtonCreateBox (nWorld, m_vTailleCollisionCam.x, m_vTailleCollisionCam.y, m_vTailleCollisionCam.z, NULL);
   // On initialise le corps avec la boite de collision
   m_pBody = NewtonCreateBody (nWorld, collision);

   if (m_pBody == NULL)
      std::cerr << "Impossible d'initialiser le corps.";
   // On détruit la boite de collision, on n'en a plus besoin
   NewtonReleaseCollision (nWorld, collision);

   // Enfin, on affecte notre matrice (qui représente donc sa position dans l'espace)
   // à notre corps grâce à la fonction NewtonBodySetMatrix
   NewtonBodySetMatrix (m_pBody, &matrice.m_Mat [0][0]);
}