#include "Objet.h"

CObjet::CObjet ()
: m_pBody (NULL)
{
   // Ne fait rien
}

CObjet::~CObjet ()
{
	if(m_pBody!=NULL)
		NewtonDestroyBody (NewtonBodyGetWorld (m_pBody), m_pBody);
}

void ApplyForceAndTorqueCallback (const NewtonBody * nBody)
{
   // Cette fonction est une fonction Callback. Elle sera appel�e � chaque fois
   // qu'une modification aura lieu sur le corps.

   // On r�cup�re en premier lieu la masse ainsi que l'inertie
   GLfloat masse; // Contiendra la masse de l'objet pris en param�tre par la fonction
   CVector3 inertie; // Contiendra l'inertie du corps
   CVector3 force; // Sp�cifiera la force appliqu�e sur le corps

   NewtonBodyGetMassMatrix (nBody, &masse, &inertie.x, &inertie.y, &inertie.z);

   force.x = 0.0f;
   force.y = -masse * 9.81; // 9.81 est l'attraction gravitationnelle de la Terre
   force.z = 0.0f;

   NewtonBodyAddForce (nBody, &force.x); // On ajoute la force au corps
}

NewtonBody* CObjet::GetBody()
{
	return m_pBody;
}