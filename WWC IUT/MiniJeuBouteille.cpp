#include "MiniJeuBouteille.h"

/***************************
Auteur : Loïc Teyssier

Usage : Strucure servant a reperer les bouteilles touchées.
***************************/
struct COLBOUTEILLE
   {
       bool b1, b2, b3, b4, b5;
   } g_colB;


/***************************
Auteur : Loïc Teyssier

Usage : Charge les modeles des bouteilles, leur attribue un matériau et 
			une fonction de rappel de collision.
***************************/
CMiniJeuBouteille::CMiniJeuBouteille(NewtonWorld *world, int matBalle)
	: CMiniJeu(5)
{
	m_sNameElem = "bouteille(s)";
	m_nWorld = world;

	//		Bouteilles :
	m_Bouteille1 = new C3DModel();
	m_Bouteille2 = new C3DModel();
	m_Bouteille3 = new C3DModel();
	m_Bouteille4 = new C3DModel();
	m_Bouteille5 = new C3DModel();


	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Bouteille1,"Models/bout1.3ds");
	delete m_JeuLoader;
	
	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Bouteille2,"Models/bout2.3ds");
	delete m_JeuLoader;

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Bouteille3,"Models/bout3.3ds");
	delete m_JeuLoader;

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Bouteille4,"Models/bout4.3ds");
	delete m_JeuLoader;

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Bouteille5,"Models/bout5.3ds");
	delete m_JeuLoader;

	m_Bouteille1->Initialiser(world,true,1.0);
	m_Bouteille2->Initialiser(world,true,1.0);
	m_Bouteille3->Initialiser(world,true,1.0);
	m_Bouteille4->Initialiser(world,true,1.0);
	m_Bouteille5->Initialiser(world,true,1.0);

	
	int matBouteille1 = NewtonMaterialCreateGroupID(world);
	int matBouteille2 = NewtonMaterialCreateGroupID(world);
	int matBouteille3 = NewtonMaterialCreateGroupID(world);
	int matBouteille4 = NewtonMaterialCreateGroupID(world);
	int matBouteille5 = NewtonMaterialCreateGroupID(world);

	NewtonBodySetMaterialGroupID(m_Bouteille1->GetBody(),matBouteille1);
	NewtonBodySetMaterialGroupID(m_Bouteille2->GetBody(),matBouteille2);
	NewtonBodySetMaterialGroupID(m_Bouteille3->GetBody(),matBouteille3);
	NewtonBodySetMaterialGroupID(m_Bouteille4->GetBody(),matBouteille4);
	NewtonBodySetMaterialGroupID(m_Bouteille5->GetBody(),matBouteille5);
	
	NewtonMaterialSetCollisionCallback (world, matBouteille1, matBalle , NULL, ContactBegin, ContactProcessBouteille1, ContactEnd);
	NewtonMaterialSetCollisionCallback (world, matBouteille2, matBalle , NULL, ContactBegin, ContactProcessBouteille2, ContactEnd);
	NewtonMaterialSetCollisionCallback (world, matBouteille3, matBalle , NULL, ContactBegin, ContactProcessBouteille3, ContactEnd);
	NewtonMaterialSetCollisionCallback (world, matBouteille4, matBalle , NULL, ContactBegin, ContactProcessBouteille4, ContactEnd);
	NewtonMaterialSetCollisionCallback (world, matBouteille5, matBalle , NULL, ContactBegin, ContactProcessBouteille5, ContactEnd);

	g_colB.b1 = true;
	g_colB.b2 = true;
	g_colB.b3 = true;
	g_colB.b4 = true;
	g_colB.b5 = true;
}

/***************************
Auteur : Loïc Teyssier

Usage : Detruit les objets et remet les booleens de la structure a false.
***************************/
CMiniJeuBouteille::~CMiniJeuBouteille()
{
	delete m_Bouteille1;
	delete m_Bouteille2;
	delete m_Bouteille3;
	delete m_Bouteille4;
	delete m_Bouteille5;

	g_colB.b1 = false;
	g_colB.b2 = false;
	g_colB.b3 = false;
	g_colB.b4 = false;
	g_colB.b5 = false;

}

/***************************
Auteur : Loïc Teyssier

Usage : Dessine tous les modeles.
***************************/
void CMiniJeuBouteille::Dessiner()
{
	m_Bouteille1->Dessiner();
	m_Bouteille2->Dessiner();
	m_Bouteille3->Dessiner();
	m_Bouteille4->Dessiner();
	m_Bouteille5->Dessiner();

}

/***************************
Auteur : Loïc Teyssier

Usage : Retourne toujours 1.
		Devait retourner 1 seulement lorsque le corps touché n'avait pas déja été traité, afin de
			n'avoir qu'un seul ContactProcess, mais lors de l'importation des modeles 3D, cette
			fonction était sans cesse appelée...
			
***************************/
int CMiniJeuBouteille::ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuBouteille::ContactProcessBouteille1 (const NewtonMaterial* material, const NewtonContact* contact)
{
	if (g_colB.b1)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("Bouteille.wav");
		g_colB.b1 = false;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuBouteille::ContactProcessBouteille2 (const NewtonMaterial* material, const NewtonContact* contact)
{
	if (g_colB.b2)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("Bouteille.wav");
		g_colB.b2 = false;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuBouteille::ContactProcessBouteille3 (const NewtonMaterial* material, const NewtonContact* contact)
{
	if (g_colB.b3)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("Bouteille.wav");
		g_colB.b3 = false;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuBouteille::ContactProcessBouteille4 (const NewtonMaterial* material, const NewtonContact* contact)
{
	if (g_colB.b4)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("Bouteille.wav");
		g_colB.b4 = false;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuBouteille::ContactProcessBouteille5 (const NewtonMaterial* material, const NewtonContact* contact)
{
	if (g_colB.b5)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("Bouteille.wav");
		g_colB.b5 = false;
	}
	
	return 1;
}

void CMiniJeuBouteille::ContactEnd (const NewtonMaterial* material)
{}