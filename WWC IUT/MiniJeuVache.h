/*
*
* Auteur : Loic Teyssier
*
* Classe CMiniJeuVache dérivée de CMiniJeu
* Usage :	Importe les vaches nécéssaires au jeu 
*				et une boite de collision a l'entrée de l'enclos.
*			Gere les collisions entre les vaches et la boite.
*			Lorsqu'une vache est dans l'enclos, incrementation de la variable reussi.
*/
#pragma once
#include "MiniJeu.h"


class CMiniJeuVache : public CMiniJeu
{

public:
	CMiniJeuVache(NewtonWorld *world, int matPerso);
	~CMiniJeuVache();

	void Dessiner();

private:

	static int ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
	
	static int ContactProcessVache1 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessVache2 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessVache3 (const NewtonMaterial* material, const NewtonContact* contact);

	static void ContactEnd (const NewtonMaterial* material);

	C3DModel * m_Vache1 , * m_Vache2 , * m_Vache3, *m_Enclos;

};