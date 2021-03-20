#include "MiniJeuVache.h"

/***************************
Auteur : Loïc Teyssier

Usage : Strucure servant a reperer les vaches mises dans l'enclos.
***************************/
struct COLVACHE
   {
	   bool v1, v2, v3;
   } g_colV;

/***************************
Auteur : Loïc Teyssier

Usage : Charge les modeles des vaches, leur attribue un matériau et 
			une fonction de rappel de collision.
		Charge une boite de collision dans l'enclos.
***************************/
CMiniJeuVache::CMiniJeuVache(NewtonWorld *world, int matPerso)
	: CMiniJeu(3)
{
	m_nWorld = world;
	m_sNameElem = "vache(s)";
	//				Vaches:
	m_Vache1 = new C3DModel();
	m_Vache2 = new C3DModel();
	m_Vache3 = new C3DModel();
	m_Enclos = new C3DModel();

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Vache1,"Models/vache1.3ds");
	delete m_JeuLoader;
	
	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Vache2,"Models/vache2.3ds");
	delete m_JeuLoader;

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Vache3,"Models/vache3.3ds");
	delete m_JeuLoader;

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Enclos,"Models/boxEnclos.3ds");
	delete m_JeuLoader;


	m_Vache1->LoadTextures();
	m_Vache2->LoadTextures();
	m_Vache3->LoadTextures();

	m_Vache1->Initialiser(world, true,200);
	m_Vache2->Initialiser(world,true, 200);
	m_Vache3->Initialiser(world,true, 200);
	m_Enclos->Initialiser(world,false, 0);

	int matVache1 = NewtonMaterialCreateGroupID(world);
	int matVache2 = NewtonMaterialCreateGroupID(world);
	int matVache3 = NewtonMaterialCreateGroupID(world);

	int matEnclos = NewtonMaterialCreateGroupID(world);

	NewtonBodySetMaterialGroupID(m_Vache1->GetBody(),matVache1);
	NewtonBodySetMaterialGroupID(m_Vache2->GetBody(),matVache2);
	NewtonBodySetMaterialGroupID(m_Vache3->GetBody(),matVache3);

	CVector3 centre;
	NewtonBodyGetCentreOfMass(m_Vache1->GetBody(), &centre.x);
	centre = centre + CVector3 (0,-3,0);
	NewtonBodySetCentreOfMass(m_Vache1->GetBody(), &centre.x);

	NewtonBodyGetCentreOfMass(m_Vache2->GetBody(), &centre.x);
	centre = centre + CVector3 (0,-3,0);
	NewtonBodySetCentreOfMass(m_Vache2->GetBody(), &centre.x);

	NewtonBodyGetCentreOfMass(m_Vache3->GetBody(), &centre.x);
	centre = centre + CVector3 (0,-3,0);
	NewtonBodySetCentreOfMass(m_Vache3->GetBody(), &centre.x);

	NewtonBodySetMaterialGroupID(m_Enclos->GetBody(),matEnclos);

	NewtonMaterialSetDefaultCollidable(world, matPerso, matEnclos,0);

	NewtonMaterialSetCollisionCallback(world, matEnclos, matVache1, NULL, ContactBegin, ContactProcessVache1, ContactEnd);
	NewtonMaterialSetCollisionCallback(world, matEnclos, matVache2, NULL, ContactBegin, ContactProcessVache2, ContactEnd);
	NewtonMaterialSetCollisionCallback(world, matEnclos, matVache3, NULL, ContactBegin, ContactProcessVache3, ContactEnd);

	g_colV.v1 = true;
	g_colV.v2 = true;
	g_colV.v3 = true;
}

/***************************
Auteur : Loïc Teyssier

Usage : Detruit les objets et remet les booleens de la structure a false.
***************************/
CMiniJeuVache::~CMiniJeuVache()
{
	g_colV.v1 = false;
	g_colV.v2 = false;
	g_colV.v3 = false;

	delete m_Vache1;
	delete m_Vache2;
	delete m_Vache3;
}

/***************************
Auteur : Loïc Teyssier

Usage : Dessine tous les modeles.
***************************/
void CMiniJeuVache::Dessiner()
{
	m_Vache1->Dessiner();
	m_Vache2->Dessiner();
	m_Vache3->Dessiner();
}

/***************************
Auteur : Loïc Teyssier

Usage : Retourne toujours 1.
		Devait retourner 1 seulement lorsque le corps touché n'avait pas déja été traité, afin de
			n'avoir qu'un seul ContactProcess, mais lors de l'importation des modeles 3D, cette
			fonction était sans cesse appelée...
			
***************************/
int CMiniJeuVache::ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{
	return 1;
}


/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuVache::ContactProcessVache1 (const NewtonMaterial* material, const NewtonContact* contact)
{	
	NewtonMaterialDisableContact(material);

	if (g_colV.v1)
	{
		QSound::play("cow.wav");
		CMiniJeu::m_nReussi ++;
		g_colV.v1 = false;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuVache::ContactProcessVache2 (const NewtonMaterial* material, const NewtonContact* contact)
{	
	NewtonMaterialDisableContact(material);

	if (g_colV.v2)
	{
		QSound::play("cow.wav");
		CMiniJeu::m_nReussi ++;
		g_colV.v2 = false;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité.
			
***************************/
int CMiniJeuVache::ContactProcessVache3 (const NewtonMaterial* material, const NewtonContact* contact)
{	
	NewtonMaterialDisableContact(material);

	if (g_colV.v3)
	{
		QSound::play("cow.wav");
		CMiniJeu::m_nReussi ++;
		g_colV.v3 = false;
	}
	
	return 1;
}
void CMiniJeuVache::ContactEnd (const NewtonMaterial* material) 
{
}