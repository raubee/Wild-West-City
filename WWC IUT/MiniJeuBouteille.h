/*
*
* Auteur : Loic Teyssier
*
* Classe CMiniJeuBouteille dérivée de CMiniJeu
* Usage :	Importe les bouteilles nécésaire au jeu.
*			Gere les collisions entre les balles et les bouteilles.
*			Lorsqu'une balle touche une bouteille, incrémentation de la variable reussi.
*
*
*/
#pragma once
#include "MiniJeu.h"

class CMiniJeuBouteille : public CMiniJeu
{

public:
	CMiniJeuBouteille(NewtonWorld *world, int matBalle);
	~CMiniJeuBouteille();

	
	void Dessiner();

private:

	static int ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
	
	static int ContactProcessBouteille1 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessBouteille2 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessBouteille3 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessBouteille4 (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessBouteille5 (const NewtonMaterial* material, const NewtonContact* contact);

	static void ContactEnd (const NewtonMaterial* material);
	
	C3DModel * m_Bouteille1 , * m_Bouteille2 , * m_Bouteille3 , *m_Bouteille4 , *m_Bouteille5;

};