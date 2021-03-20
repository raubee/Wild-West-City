/*
*
* Auteur : Loic Teyssier
*
* Classe CMiniJeuCaisse dérivée de CMiniJeu
* Usage :	Importe les caisses nécéssaires au jeu.
*			Gère les collisions entre le perso et les caises.
*			Lorsque le perso rentr assez vite dan une caisse, suppression de la caisse
*				et incrémentation de la variable reussi.
*
*/
#pragma once
#include "MiniJeu.h"

class CMiniJeuCaisse : public CMiniJeu
{

public:
	CMiniJeuCaisse(NewtonWorld *world, int matBalle);
	~CMiniJeuCaisse();

	void Dessiner();

private:

	QTimer *t_TimerCaisse;

	static int ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
	
	static int ContactProcessCaisse1 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessCaisse2 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessCaisse3 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessCaisse4 (const NewtonMaterial* material, const NewtonContact* contact);

	static void ContactEnd (const NewtonMaterial* material);
	
	C3DModel * m_Caisse1 , * m_Caisse2 , * m_Caisse3 , *m_Caisse4 ;

};