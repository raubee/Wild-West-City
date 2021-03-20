#include "MiniJeuCaisse.h"

/***************************
Auteur : Loïc Teyssier

Usage : Strucure servant a reperer les caisses touchées, et celles supprimées.
***************************/
struct COLCAISSE
   {
      bool c1,c2,c3,c4;
	  bool suppr1, suppr2, suppr3, suppr4;
   } g_colC;

/***************************
Auteur : Loïc Teyssier

Usage : Charge les modeles des caisses, leur attribue un matériau et 
			une fonction de rappel de collision.
***************************/
CMiniJeuCaisse::CMiniJeuCaisse(NewtonWorld *world, int matPerso)
	: CMiniJeu(4)
{
	m_nWorld = world;
	m_sNameElem = "caisse(s)";

	// TIMER :
	int seconde = 1000; // 1 seconde = 1000 ms
    int timerInterval = seconde / 60; // l'intervalle entre deux images
    t_TimerCaisse = new QTimer(this);
    connect(t_TimerCaisse, SIGNAL(timeout()), this, SLOT(timeOutSlotCaisse()));
    t_TimerCaisse->start( timerInterval );

	//		Caisses :
	m_Caisse1 = new C3DModel();
	m_Caisse2 = new C3DModel();
	m_Caisse3 = new C3DModel();
	m_Caisse4 = new C3DModel();

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Caisse1,"Models/caisse1.3ds");
	delete m_JeuLoader;
	
	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Caisse2,"Models/caisse2.3ds");
	delete m_JeuLoader;

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Caisse3,"Models/caisse3.3ds");
	delete m_JeuLoader;

	m_JeuLoader = new CLoad3DS();
	m_JeuLoader->Import3DS(m_Caisse4,"Models/caisse4.3ds");
	delete m_JeuLoader;


	m_Caisse1->LoadTextures();
	m_Caisse2->LoadTextures();
	m_Caisse3->LoadTextures();
	m_Caisse4->LoadTextures();


	m_Caisse1->Initialiser(world,true,200.0);
	m_Caisse2->Initialiser(world,true,200.0);
	m_Caisse3->Initialiser(world,true,200.0);
	m_Caisse4->Initialiser(world,true,200.0);

	
	int matCaisse1 = NewtonMaterialCreateGroupID(world);
	int matCaisse2 = NewtonMaterialCreateGroupID(world);
	int matCaisse3 = NewtonMaterialCreateGroupID(world);
	int matCaisse4 = NewtonMaterialCreateGroupID(world);

	NewtonBodySetMaterialGroupID(m_Caisse1->GetBody(),matCaisse1);
	NewtonBodySetMaterialGroupID(m_Caisse2->GetBody(),matCaisse2);
	NewtonBodySetMaterialGroupID(m_Caisse3->GetBody(),matCaisse3);
	NewtonBodySetMaterialGroupID(m_Caisse4->GetBody(),matCaisse4);
	
	NewtonMaterialSetCollisionCallback (world, matCaisse1, matPerso , NULL, ContactBegin, ContactProcessCaisse1, ContactEnd);
	NewtonMaterialSetCollisionCallback (world, matCaisse2, matPerso , NULL, ContactBegin, ContactProcessCaisse2, ContactEnd);
	NewtonMaterialSetCollisionCallback (world, matCaisse3, matPerso , NULL, ContactBegin, ContactProcessCaisse3, ContactEnd);
	NewtonMaterialSetCollisionCallback (world, matCaisse4, matPerso , NULL, ContactBegin, ContactProcessCaisse4, ContactEnd);

	g_colC.c1 = true;
	g_colC.c2 = true;
	g_colC.c3 = true;
	g_colC.c4 = true;
}

/***************************
Auteur : Loïc Teyssier

Usage : Remet les booleens de la structure a false. Les objets ont déja été detruits avant.
***************************/
CMiniJeuCaisse::~CMiniJeuCaisse()
{
	g_colC.c1 = false;
	g_colC.c2 = false;
	g_colC.c3 = false;
	g_colC.c4 = false;

	g_colC.suppr1 = false;
	g_colC.suppr2 = false;
	g_colC.suppr3 = false;
	g_colC.suppr4 = false;
}

/***************************
Auteur : Loïc Teyssier

Usage : Dessine tous les modeles, supprime ceux qu'il ne faut plus dessiner.
***************************/
void CMiniJeuCaisse::Dessiner()
{
	if (g_colC.suppr1)
	{
		delete m_Caisse1;
		m_Caisse1 = NULL;
		g_colC.suppr1 = false;

	}
	if (g_colC.suppr2)
	{
		delete m_Caisse2;
		m_Caisse2 = NULL;
		g_colC.suppr2 = false;
	}
	if (g_colC.suppr3)
	{
		delete m_Caisse3;
		m_Caisse3 = NULL;
		g_colC.suppr3 = false;
	}
	if (g_colC.suppr4)
	{
		delete m_Caisse4;
		m_Caisse4 = NULL;
		g_colC.suppr4 = false;
	}

	if (m_Caisse1 != NULL)
		m_Caisse1->Dessiner();

	if (m_Caisse2 != NULL)
		m_Caisse2->Dessiner();

	if (m_Caisse3 != NULL)
		m_Caisse3->Dessiner();

	if (m_Caisse4 != NULL)
		m_Caisse4->Dessiner();
}

/***************************
Auteur : Loïc Teyssier

Usage : Retourne toujours 1.
		Devait retourner 1 seulement lorsque le corps touché n'avait pas déja été traité, afin de
			n'avoir qu'un seul ContactProcess, mais lors de l'importation des modeles 3D, cette
			fonction était sans cesse appelée...
			
***************************/
int CMiniJeuCaisse::ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité 
			et si la vitesse de collision est assez grande.
			
***************************/
int CMiniJeuCaisse::ContactProcessCaisse1 (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 100;
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);

	if (g_colC.c1 && speed>MIN_SPEED)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("bris-bois.wav");
		g_colC.c1 = false;
		g_colC.suppr1 = true;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité 
			et si la vitesse de collision est assez grande.
			
***************************/
int CMiniJeuCaisse::ContactProcessCaisse2 (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 100;
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);

	if (g_colC.c2 && speed>MIN_SPEED)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("bris-bois.wav");
		g_colC.c2 = false;
		g_colC.suppr2 = true;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité 
			et si la vitesse de collision est assez grande.
			
***************************/
int CMiniJeuCaisse::ContactProcessCaisse3 (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 100;
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);

	if (g_colC.c3 && speed>MIN_SPEED)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("bris-bois.wav");
		g_colC.c3 = false;
		g_colC.suppr3 = true;
	}
	
	return 1;
}

/***************************
Auteur : Loïc Teyssier

Usage : Incremente reussi seulement si ce corps n'a pas déja été traité 
			et si la vitesse de collision est assez grande.
			
***************************/
int CMiniJeuCaisse::ContactProcessCaisse4 (const NewtonMaterial* material, const NewtonContact* contact)
{
	const float MIN_SPEED = 100;
	float speed = NewtonMaterialGetContactNormalSpeed (material, contact);

	if (g_colC.c4 && speed>MIN_SPEED)
	{
		CMiniJeu::m_nReussi ++;
		QSound::play("bris-bois.wav");
		g_colC.c4 = false;
		g_colC.suppr4 = true;
	}
	
	return 1;
}
void CMiniJeuCaisse::ContactEnd (const NewtonMaterial* material)
{}