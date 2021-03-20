#include "WidgetOGL.h"

int CWidgetOGL::ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Affiche le panneau du jeu.
***************************/
int CWidgetOGL::ContactProcessPanneauVache (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 0.1;	
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);
	static int i=0;

	if (speed > MIN_SPEED && i==0)
	{
		CWidgetOGL::m_bsBoolAskJeuVache = true;
		i++;
	}
	if (speed<MIN_SPEED)
		i=0;
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Affiche le panneau du jeu.
***************************/
int CWidgetOGL::ContactProcessPanneauBouteille (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 0.1;	
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);
	static int i=0;

	if (speed > MIN_SPEED && i==0)
	{
		CWidgetOGL::m_bsBoolAskJeuBouteille = true;
		i++;
	}
	if (speed<MIN_SPEED)
		i=0;
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Affiche le panneau du jeu.
***************************/
int CWidgetOGL::ContactProcessPanneauCaisse (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 0.1;	
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);
	static int i=0;

	if (speed > MIN_SPEED && i==0)
	{
		CWidgetOGL::m_bsBoolAskJeuCaisse = true;
		i++;
	}
	if (speed<MIN_SPEED)
		i=0;
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Affiche un message si les 3 jeux ne sont pas finis.
		Affiche les credits lorsque les 3 jeux sot finis.
***************************/
int CWidgetOGL::ContactProcessPanneauEntree (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 0.1;	
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);
	static int i=0;

	if (speed > MIN_SPEED && i==0)
	{
		CWidgetOGL::m_bsBoolAskEntree = true;
		i++;

	}
	if (speed<MIN_SPEED)
	{
		CWidgetOGL::m_bsBoolAskEntree = false;
		i=0;
	}
	return 1;
}
void CWidgetOGL::ContactEnd (const NewtonMaterial* material) 
{
}