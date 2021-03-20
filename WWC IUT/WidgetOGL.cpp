#include "WidgetOGL.h"

bool CWidgetOGL::m_bsBoolJeuVache = false;
bool CWidgetOGL::m_bsBoolJeuBouteille = false;
bool CWidgetOGL::m_bsBoolJeuCaisse = false;

bool CWidgetOGL::m_bsVacheOk = false;
bool CWidgetOGL::m_bsBouteilleOk = false;
bool CWidgetOGL::m_bsCaisseOk = false;

bool CWidgetOGL::m_bsBoolAskJeuVache = false;
bool CWidgetOGL::m_bsBoolAskJeuBouteille = false;
bool CWidgetOGL::m_bsBoolAskJeuCaisse = false;
bool CWidgetOGL::m_bsBoolAskEntree = false;

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Initialise la scene.
			Lance le timer.
			Initialise les variables.
			Initialise la camera.
**************************/
CWidgetOGL::CWidgetOGL(int fps, QWidget *parent, char *titre)
: QGLWidget(parent),m_nMouseX(CWidgetOGL::width()/2),m_nMouseY(CWidgetOGL::height()/2)
{
	setWindowTitle(QString::fromUtf8(titre));
	if(fps == 0)
        t_Timer = NULL;
	else
    {
        int seconde = 1000; // 1 seconde = 1000 ms
        int timerInterval = seconde / fps; // l'intervalle entre deux images
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start( timerInterval );
    }

	connect(this,SIGNAL(LancerMiniJeu(int)),this,SLOT(CommencerMiniJeu(int)));
	
	// Initialisation de la scene
	NewtonInit();
	m_NumBalle=0;

	m_JeuEnCours = NULL;
	m_bMoveCam = false;

	m_Camera= new CCamera(m_3Dperso,0,1,0,10.0);
	CVector3 taille(1,1,1);
	m_Camera->Initialiser(m_World,taille);
	m_Camera->setTheta(m_Camera->getTheta()+1.5);

	CMatrix matrice;
	NewtonBodyGetMatrix(m_3Dperso->GetBody(), &matrice.m_Mat[0][0]);
	matrice.m_Mat[0][0] = cos(m_Camera->getTheta());
	matrice.m_Mat[0][1] = 0;
	matrice.m_Mat[0][2] = sin(m_Camera->getTheta());

	matrice.m_Mat[1][0] = 0;
	matrice.m_Mat[1][1] = 1;
	matrice.m_Mat[1][2] = 0;

	matrice.m_Mat[2][0] = -sin(m_Camera->getTheta());
	matrice.m_Mat[2][1] = 0;
	matrice.m_Mat[2][2] = cos(m_Camera->getTheta());

	NewtonBodySetMatrix(m_3Dperso->GetBody(), &matrice.m_Mat[0][0]);

	setMouseTracking(true);	
}

CWidgetOGL::~CWidgetOGL()
{}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Lance les signaux pour commencer les jeux.
			Gere l'affichage des panneaux.
**************************/
void CWidgetOGL::timeOutSlot()
{
	updateGL();
	if(!m_Menu.m_bMenu)
	{
		NewtonUpdate (m_World, 1.0f/60.0f);

		if (m_bsBoolJeuVache==true)
		{
			emit LancerMiniJeu(1);
			m_bsBoolJeuVache=false;
		}

		if (m_bsBoolJeuBouteille==true)
		{
			emit LancerMiniJeu(2);
			m_bsBoolJeuBouteille=false;
		}

		if (m_bsBoolJeuCaisse==true)
		{
			emit LancerMiniJeu(3);
			m_bsBoolJeuCaisse=false;
		}
	}
	if(m_bsBoolAskJeuVache == true)
	{
		NewtonUpdate (m_World, 1.0f/60.0f);
		m_Menu.m_bMenu = true;
		m_Menu.m_nTextCourante = 13;
		m_Menu.m_nStat = 4;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
	if(m_bsBoolAskJeuBouteille == true)
	{
		NewtonUpdate (m_World, 1.0f/60.0f);
		m_Menu.m_bMenu = true;
		m_Menu.m_nTextCourante = 11;
		m_Menu.m_nStat = 4;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
	if(m_bsBoolAskJeuCaisse == true)
	{
		NewtonUpdate (m_World, 1.0f/60.0f);
		m_Menu.m_bMenu = true;
		m_Menu.m_nTextCourante = 12;
		m_Menu.m_nStat = 4;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
	if(m_bsBoolAskEntree == true)
	{
		NewtonUpdate (m_World, 1.0f/60.0f);
		m_Menu.m_bMenu = true;
		if(m_bsVacheOk && m_bsCaisseOk && m_bsBouteilleOk )
			m_Menu.m_nTextCourante = 10;
		else m_Menu.m_nTextCourante = 9;
		m_Menu.m_nStat = 3;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Gere l'interaction avec le clavier.
**************************/
void CWidgetOGL::keyPressEvent(QKeyEvent *keyEvent)
{
	if(!m_Menu.m_bMenu)
	{
		CMatrix position;
		NewtonBodyGetMatrix(m_3Dperso->GetBody(), &position.m_Mat[0][0]);

		switch(keyEvent->key())
		{
			case Qt::Key_Space:
				if (TestCollision(m_3Dsol->GetBody(), m_3Dperso->GetBody()) || TestCollision(m_3Dplateau->GetBody(), m_3Dperso->GetBody()))
				{
					CVector3 vitesse;
					NewtonBodyGetVelocity(m_3Dperso->GetBody(), &vitesse.x);
					vitesse= vitesse + CVector3(0,10,0);
					vitesse.Normalize();
					vitesse = vitesse * 10;
					NewtonBodySetVelocity(m_3Dperso->GetBody(), &vitesse.x);
					QSound::play("Jump.wav");
				}
			break;

			case Qt::Key_D:
				if (TestCollision(m_3Dsol->GetBody(), m_3Dperso->GetBody()) || TestCollision(m_3Dplateau->GetBody(), m_3Dperso->GetBody()))
				{
					m_Camera->setTheta(m_Camera->getTheta()+0.01);
				}
			break;

			case Qt::Key_Q : 
				if (TestCollision(m_3Dsol->GetBody(), m_3Dperso->GetBody()) || TestCollision(m_3Dplateau->GetBody(), m_3Dperso->GetBody()))
				{
					m_Camera->setTheta(m_Camera->getTheta()-0.01);
				}
			break;

			case Qt::Key_Z:
				if (TestCollision(m_3Dsol->GetBody(), m_3Dperso->GetBody()) || TestCollision(m_3Dplateau->GetBody(), m_3Dperso->GetBody()))
				{
					CVector3 vitesse(-20*cos(m_Camera->getTheta()),0,-20*sin(m_Camera->getTheta()));
					NewtonBodySetVelocity(m_3Dperso->GetBody(), &vitesse.x);
				}
			break;
		
			case Qt::Key_S:
				if (TestCollision(m_3Dsol->GetBody(), m_3Dperso->GetBody()) || TestCollision(m_3Dplateau->GetBody(), m_3Dperso->GetBody()))
				{
					CVector3 vitesse(10*cos(m_Camera->getTheta()),0,10*sin(m_Camera->getTheta()));
					NewtonBodySetVelocity(m_3Dperso->GetBody(), &vitesse.x);
				}
			break;

			case Qt::Key_Control:
				{
					CVector3 vitesse(-40*cos(m_Camera->getTheta()),0,-40*sin(m_Camera->getTheta()));
					NewtonBodySetVelocity(m_3Dperso->GetBody(), &vitesse.x);
				}
			break;
			case Qt::Key_F:
				{
					QSound::play("Klaxon.wav");
				}
			break;

			case Qt::Key_M:
					
				break;

			case Qt::Key_Escape:
					m_Menu.m_nTextCourante =0;
					QSound::play("Menu.wav");
					m_Menu.m_bMenu = true;
					this->setCursor(QCursor(Qt::ArrowCursor));
				break;
		}
		CMatrix matrice;
		NewtonBodyGetMatrix(m_3Dperso->GetBody(), &matrice.m_Mat[0][0]);
		matrice.m_Mat[0][0] = cos(m_Camera->getTheta());
		matrice.m_Mat[0][1] = 0;
		matrice.m_Mat[0][2] = sin(m_Camera->getTheta());

		matrice.m_Mat[1][0] = 0;
		matrice.m_Mat[1][1] = 1;
		matrice.m_Mat[1][2] = 0;

		matrice.m_Mat[2][0] = -sin(m_Camera->getTheta());
		matrice.m_Mat[2][1] = 0;
		matrice.m_Mat[2][2] = cos(m_Camera->getTheta());

		NewtonBodySetMatrix(m_3Dperso->GetBody(), &matrice.m_Mat[0][0]);
	}
}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Gere l'interaction avec la souris dans le jeu et les menus.
**************************/
void CWidgetOGL::mousePressEvent(QMouseEvent *mouseEvent)
{
	if(m_Menu.m_bMenu)
	{
		if(mouseEvent->button() == Qt::LeftButton)
		{
			// gestion des boutons m_Menu
			switch (m_Menu.m_nStat)
			{
				case 0:
				{
					//Interaction du menu avec utilisateur
					if(mouseEvent->x() > (int)(3*CWidgetOGL::width()/8)  &&  mouseEvent->x() < (5*CWidgetOGL::width()/8))
					{
						if(mouseEvent->y() > (int)(2*CWidgetOGL::height()/5)  &&  mouseEvent->y() < (5*CWidgetOGL::height()/10))
						{
							
							m_Menu.m_bMenu = false;
							resizeGL(CWidgetOGL::width(),CWidgetOGL::height());
							this->setCursor(QCursor(Qt::BlankCursor));
							QEvent event(QEvent::MouseButtonRelease);
						}

						if(mouseEvent->y() > (int)(5*CWidgetOGL::height()/10)  &&  mouseEvent->y() < (int)(3*CWidgetOGL::height()/5))
						{
							m_Menu.m_nStat = 1;
							m_Menu.m_nTextCourante = 5;
							m_Menu.m_bMenu = true;
						}

						if(mouseEvent->y() > (int)(3*CWidgetOGL::height()/5)  &&  mouseEvent->y() < (int)(15*CWidgetOGL::height()/20))
						{
							exit(0);
						}

						if(mouseEvent->y() > (int)(15*CWidgetOGL::height()/20)  &&  mouseEvent->y() < (int)(9*CWidgetOGL::height()/10))
						{
							m_Menu.m_nStat = 2;
							m_Menu.m_nTextCourante = 7;
							m_Menu.m_bMenu = true;
						}
					}
				}
				break;

				case 1:
				{
					if(mouseEvent->x() > (int)(13*CWidgetOGL::width()/16)  &&  mouseEvent->x() < (16*CWidgetOGL::width()/16))
					{
						if(mouseEvent->y() > (int)(9*CWidgetOGL::height()/10)  &&  mouseEvent->y() < (10*CWidgetOGL::height()/10))
						{
							m_Menu.m_nStat=0;
							m_Menu.m_nTextCourante = 0;
							m_Menu.m_bMenu = true;
						}
					}
				}
				break;

				case 2:
				{
					//Interaction des Pages Credits et Commandes => Permet le retour au menu principal
					if(mouseEvent->x() > (int)(13*CWidgetOGL::width()/16)  &&  mouseEvent->x() < (16*CWidgetOGL::width()/16))
					{
						if(mouseEvent->y() > (int)(9*CWidgetOGL::height()/10)  &&  mouseEvent->y() < (10*CWidgetOGL::height()/10))
						{
							m_Menu.m_nStat=0;
							m_Menu.m_nTextCourante = 0;
							m_Menu.m_bMenu = true;
						}
					}
				}
				break;
				case 3 :
					{
						//Interaction du panneau d'entrée
						if(mouseEvent->y() > (int)(10*CWidgetOGL::height()/13)  &&  mouseEvent->y() < (11*CWidgetOGL::height()/13))
						{
							if(mouseEvent->x() > (int)(16*CWidgetOGL::width()/20)  &&  mouseEvent->x() < (17*CWidgetOGL::width()/20))
							{
								if(m_bsVacheOk && m_bsCaisseOk && m_bsBouteilleOk && m_bsBoolAskEntree)
								{
									m_bsBoolAskEntree = false;
									m_Menu.m_nStat = 2;
									m_Menu.m_nTextCourante = 7;
								}
								else
								{
									m_bsBoolAskEntree = false;
									m_Menu.m_bMenu = false;
									resizeGL(CWidgetOGL::width(),CWidgetOGL::height());
									this->setCursor(QCursor(Qt::BlankCursor));
									m_Menu.m_nStat = 0;
								}
							}
						}
					}
					break;
				case 4 :
					{
						if(mouseEvent->y() > (int)(10*CWidgetOGL::height()/13)  &&  mouseEvent->y() < (11*CWidgetOGL::height()/13))
						{
							if(mouseEvent->x() > (int)(16*CWidgetOGL::width()/20)  &&  mouseEvent->x() < (17*CWidgetOGL::width()/20))
							{
								if(m_bsBoolAskJeuVache)
								{
									m_bsBoolAskJeuVache = false;
									m_bsBoolJeuVache = true;
								}
								if(m_bsBoolAskJeuBouteille)
								{
									m_bsBoolAskJeuBouteille = false;
									m_bsBoolJeuBouteille = true;
								}
								if(m_bsBoolAskJeuCaisse)
								{
									m_bsBoolAskJeuCaisse = false;
									m_bsBoolJeuCaisse = true;
								}
								m_Menu.m_bMenu = false;
								resizeGL(CWidgetOGL::width(),CWidgetOGL::height());
								this->setCursor(QCursor(Qt::BlankCursor));
								m_Menu.m_nStat = 0;
								m_Menu.m_sAffichage_temps = "";
							}
							if(mouseEvent->x() > (int)(3*CWidgetOGL::width()/20)  &&  mouseEvent->x() < (9*CWidgetOGL::width()/20))
							{
								m_bsBoolAskJeuCaisse = false;
								m_bsBoolAskJeuBouteille = false;
								m_bsBoolAskJeuVache = false;
								m_Menu.m_bMenu = false;
								resizeGL(CWidgetOGL::width(),CWidgetOGL::height());
								this->setCursor(QCursor(Qt::BlankCursor));
								m_Menu.m_nStat = 0;
							}
						}
					}
			}
		}
	}
	else {
		if(mouseEvent->button() == Qt::LeftButton)
		{
			QSound::play("Gun.wav");
			m_NumBalle++;
			if (m_NumBalle == m_nsNbBalles )
				m_NumBalle = 0;
			SetTabBalles();
			CMatrix matrice;
			NewtonBodyGetMatrix(m_3Dperso->GetBody(), &matrice.m_Mat[0][0]);
			matrice.m_Mat[0][0] = cos(m_Camera->getTheta());
			matrice.m_Mat[0][1] = 0;
			matrice.m_Mat[0][2] = sin(m_Camera->getTheta());

			matrice.m_Mat[1][0] = 0;
			matrice.m_Mat[1][1] = 1;
			matrice.m_Mat[1][2] = 0;

			matrice.m_Mat[2][0] = -sin(m_Camera->getTheta());
			matrice.m_Mat[2][1] = 0;
			matrice.m_Mat[2][2] = cos(m_Camera->getTheta());

			NewtonBodySetMatrix(m_3Dperso->GetBody(), &matrice.m_Mat[0][0]);

			CMatrix position;
			NewtonBodyGetMatrix(m_3Dperso->GetBody(), &position.m_Mat[0][0]);
			CVector3 vitesse(0,0,0);
			vitesse.setCoordonnees(-m_Camera->getPitch()*10, m_PosY, -m_Camera->getRoll()*10);

			NewtonBodySetVelocity(m_tTableBalle[m_NumBalle]->GetBody(), &vitesse.x);
		}


		if(mouseEvent->button() == Qt::MidButton)
		{
			m_bMoveCam = true;
			m_nMouseX-=mouseEvent->x();
			m_nMouseY-=mouseEvent->y();
		}
	}
}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Met a jour les coordonnées du pointeur lors du relachement.
**************************/
void CWidgetOGL::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	if(!m_Menu.m_bMenu)
	{
		if(mouseEvent->button() == Qt::MidButton)
		{
			m_bMoveCam = false;
			m_nMouseX+=mouseEvent->x();
			m_nMouseY+=mouseEvent->y();
		}
	}
}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Gere les surlignements dans le menu.
			Gere le déplacement de la caméra.
			Met a jour la position en Y du viseur.
**************************/
void CWidgetOGL::mouseMoveEvent(QMouseEvent* moveEvent)
{
	if(!m_Menu.m_bMenu)
	{
		if (m_bMoveCam == true)
		{
			m_Camera->calculposition(m_nMouseX+moveEvent->x(),m_nMouseY+moveEvent->y(),CWidgetOGL::width()/2,CWidgetOGL::height()/2);
		}
		m_PosY = -moveEvent->y()+CWidgetOGL::height()/2;
	}
	else
	{	
		// Gestion des boutons m_Menu
		switch (m_Menu.m_nStat)
		{
			case 0:
			{
				if(moveEvent->x() > (int)(3*CWidgetOGL::width()/8)  &&  moveEvent->x() < (5*CWidgetOGL::width()/8))
				{
					if(moveEvent->y() > (int)(2*CWidgetOGL::height()/5)  &&  moveEvent->y() < (5*CWidgetOGL::height()/10))
					{
						m_Menu.m_nTextCourante = 1;
					}

					if(moveEvent->y() > (5*CWidgetOGL::height()/10)  &&  moveEvent->y() < (3*CWidgetOGL::height()/5))
					{
						m_Menu.m_nTextCourante = 2;
					}

					if(moveEvent->y() > (3*CWidgetOGL::height()/5)  &&  moveEvent->y() < (15*CWidgetOGL::height()/20))
					{
						m_Menu.m_nTextCourante = 3;
					}


					if(moveEvent->y() > (15*CWidgetOGL::height()/20)  &&  moveEvent->y() < (9*CWidgetOGL::height()/10))
					{
						m_Menu.m_nTextCourante = 4;
					}
				}
				else m_Menu.m_nTextCourante = 0;
			}
			break;

			case 1:
			{
				if(moveEvent->x() > (int)(13*CWidgetOGL::width()/16)  &&  moveEvent->x() < (16*CWidgetOGL::width()/16))
				{
					if(moveEvent->y() > (int)(9*CWidgetOGL::height()/10)  &&  moveEvent->y() < (10*CWidgetOGL::height()/10))
					{
						m_Menu.m_nTextCourante = 6;
					}
				}
				else m_Menu.m_nTextCourante =5;
			}
			break;

			case 2:
			{
				if(moveEvent->x() > (int)(13*CWidgetOGL::width()/16)  &&  moveEvent->x() < (16*CWidgetOGL::width()/16))
				{
					if(moveEvent->y() > (int)(9*CWidgetOGL::height()/10)  &&  moveEvent->y() < (10*CWidgetOGL::height()/10))
					{
						m_Menu.m_nTextCourante = 8;
					}
				}
				else m_Menu.m_nTextCourante = 7;
			}
			break;
		}
	}
	
}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Gere le zoom de la camera.
**************************/
void CWidgetOGL::wheelEvent(QWheelEvent * wheelEvent)
{
	if(!m_Menu.m_bMenu)
	{
		m_Camera->zoomer(wheelEvent->delta()/100);
		m_Camera->calculposition(m_nMouseX,m_nMouseY,CWidgetOGL::width()/2,CWidgetOGL::height()/2);
	}
}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Créé le monde Newton.
			Charge les modeles.
			Créé des objets newton avec des boites de collisions.
			Initialise les matériaux.
			Initialise les fonctions de rappel des collisions.
**************************/
void CWidgetOGL::NewtonInit()
{
	m_World = NewtonCreate(NULL, NULL);
	CVector3 sMin(-250,-250,-250);
	CVector3 sMax(250,250,250);
	NewtonSetWorldSize(m_World, &sMin.x, &sMax.x);

	/// Model 3DS ////
	m_Loader = new CLoad3DS();

	m_3Dsol = new C3DModel();
	m_3Dplateau = new C3DModel();
	m_3Dperso = new C3DModel();
	m_3Dskybox = new C3DModel();
	m_3DpanBout = new C3DModel();
	m_3DpanVache = new C3DModel();
	m_3DpanCaisse = new C3DModel();
	m_3DpanEntree= new C3DModel();
	m_3Dlimite = new C3DModel();
	m_3Dcactus = new C3DModel();

    //				Sol :
	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3Dsol,"Models/sol.3DS");
	delete m_Loader;

	//             Plateau :
	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3Dplateau,"Models/Town.3ds");
	delete m_Loader;

	//              Cactus :
	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3Dcactus,"Models/cactus.3ds");
	delete m_Loader;

	//				Skybox:
	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3Dskybox,"Models/skybox.3ds");
	delete m_Loader;

	//				Perso:
	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3Dperso,"Models/BillyFinal2.3ds");
	delete m_Loader;

	//			   Panneaux:

	

	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3DpanCaisse,"Models/pancartecaisse.3ds");
	delete m_Loader;

	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3DpanBout,"Models/pancartebouteille.3ds");
	delete m_Loader;

	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3DpanVache,"Models/pancartevache.3ds");
	delete m_Loader;

	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3DpanEntree,"Models/panneauentree.3ds");
	delete m_Loader;

	m_Loader = new CLoad3DS();
	m_Loader->Import3DS(m_3Dlimite,"Models/limite.3ds");
	delete m_Loader;

	m_tTableBalle = new C3DModel*[m_nsNbBalles];
	for (int i=0; i<m_nsNbBalles; i++)
	{
		m_Loader = new CLoad3DS();
		m_tTableBalle[i] = new C3DModel;
		m_Loader->Import3DS(m_tTableBalle[i],"Models/balle.3ds");
		delete m_Loader;
		m_tTableBalle[i]->Initialiser(m_World, true, 1, false);
		NewtonBodySetAutoFreeze (m_tTableBalle[i]->GetBody(), 0);
	}

	m_3Dsol->Initialiser(m_World,false,0.0,true);
	m_3Dcactus->Initialiser(m_World,false,0.0,true);
	m_3Dplateau->Initialiser(m_World,false,0.0,false);
	m_3DpanCaisse->Initialiser(m_World,false,0.0,false);
	m_3DpanBout->Initialiser(m_World,false,0.0,false);
	m_3DpanVache->Initialiser(m_World,false,0.0,false);
	m_3DpanEntree->Initialiser(m_World,false,0.0,false);
	m_3Dlimite->Initialiser(m_World,false,0.0,false);


	CVector3 tperso(2.0,3.0,2.0);					//taille
	CVector3 pperso(0,(tperso.y+10)/2, 220);	// position
	m_3Dperso->InitialiserPerso(m_World,tperso,pperso,true,100.0);
	CVector3 centre(0, -tperso.y/2, 0);
	NewtonBodySetCentreOfMass(m_3Dperso->GetBody(), &centre.x); // Centre de gravité aux pieds pour qu'il soit toujours debout.
	NewtonBodySetAutoFreeze (m_3Dperso->GetBody(), 0); //Le corps ne se mttra jamais en position d'équilibre.


	// DEFINITION DES MATERIAUX :

	int matSol = NewtonMaterialCreateGroupID(m_World); // sol & maisons
	int matPerso = NewtonMaterialCreateGroupID(m_World); // perso
	int matWorld = NewtonMaterialCreateGroupID(m_World); // skybox

	int matPanneauVache = NewtonMaterialCreateGroupID(m_World);
	int matPanneauBouteille = NewtonMaterialCreateGroupID(m_World);
	int matPanneauCaisse = NewtonMaterialCreateGroupID(m_World);
	int matPanneauEntree= NewtonMaterialCreateGroupID(m_World);

	int matBalle = NewtonMaterialCreateGroupID(m_World);


	for (int i=0; i<m_nsNbBalles; i++)
		NewtonBodySetMaterialGroupID(m_tTableBalle[i]->GetBody(),matBalle);

	NewtonBodySetMaterialGroupID(m_3Dperso->GetBody(),matPerso);
	NewtonBodySetMaterialGroupID(m_3Dsol->GetBody(),matSol);
	NewtonBodySetMaterialGroupID(m_3Dlimite->GetBody(),matWorld);


	NewtonBodySetMaterialGroupID(m_3DpanVache->GetBody(),matPanneauVache);
	NewtonBodySetMaterialGroupID(m_3DpanBout->GetBody(),matPanneauBouteille);
	NewtonBodySetMaterialGroupID(m_3DpanCaisse->GetBody(),matPanneauCaisse);
	NewtonBodySetMaterialGroupID(m_3DpanEntree->GetBody(),matPanneauEntree);

	NewtonMaterialSetDefaultElasticity(m_World,matSol, matPerso, 0.1); // Le perso ne rebondit ps sur le sol.
	NewtonMaterialSetDefaultFriction(m_World,matSol, matPerso, 1,10); //Le perso ne glisse pas trop sur le sol.
	NewtonMaterialSetDefaultCollidable(m_World, matPerso, matBalle,0);
	NewtonMaterialSetDefaultCollidable(m_World, matWorld, matBalle,0);

	NewtonMaterialSetCollisionCallback (m_World, matPerso, matPanneauVache, NULL, ContactBegin, ContactProcessPanneauVache, ContactEnd);
	NewtonMaterialSetCollisionCallback (m_World, matPerso, matPanneauBouteille, NULL, ContactBegin, ContactProcessPanneauBouteille, ContactEnd);
	NewtonMaterialSetCollisionCallback (m_World, matPerso, matPanneauCaisse, NULL, ContactBegin, ContactProcessPanneauCaisse, ContactEnd);
	NewtonMaterialSetCollisionCallback (m_World, matPerso, matPanneauEntree, NULL, ContactBegin, ContactProcessPanneauEntree, ContactEnd);


}

/*************************
Auteur: Loïc Teyssier

Usage :		Detruit le balle la plus ancienne et en créé une nouvelle.
**************************/
void CWidgetOGL::SetTabBalles()
{
	int matBalle = NewtonBodyGetMaterialGroupID(m_tTableBalle[m_NumBalle]->GetBody());
	NewtonDestroyBody(m_World, m_tTableBalle[m_NumBalle]->GetBody());
	
	m_tTableBalle[m_NumBalle]->Initialiser(m_World, true, 1, false);

	CMatrix position;
	NewtonBodyGetMatrix(m_3Dperso->GetBody(), &position.m_Mat[0][0]);
	NewtonBodySetMatrix(m_tTableBalle[m_NumBalle]->GetBody(), &position.m_Mat[0][0]);

	NewtonBodySetMaterialGroupID(m_tTableBalle[m_NumBalle]->GetBody(),matBalle);
	NewtonBodySetContinuousCollisionMode(m_tTableBalle[m_NumBalle]->GetBody(), 1); // Collisions jamais ignorées 
																		//même à grande vitesse																	
}

/*************************
Auteur: Loïc Teyssier

Usage :		Teste une collision entre deux corps newton.
**************************/
bool CWidgetOGL::TestCollision(NewtonBody* body1, NewtonBody* body2)
{
	const int maxsize = 1;
	float contacts[3*maxsize];
	float normals[3*maxsize];
	float penetra[3*maxsize];
	CMatrix t1, t2;

	NewtonBodyGetMatrix( body1, &t1.m_Mat[0][0] );
	NewtonBodyGetMatrix( body2, &t2.m_Mat[0][0] );

	if ( NewtonCollisionCollide( m_World, maxsize, NewtonBodyGetCollision(body1), &t1.m_Mat[0][0], NewtonBodyGetCollision(body2), &t2.m_Mat[0][0], contacts, normals, penetra ) != 0 ) 
		return true;
	 
	else 
	  return false;
}


/*************************
Auteur: Loïc Teyssier

Usage :		Instancie m_JeuEnCours selon le jeu choisi.
**************************/
void CWidgetOGL::CommencerMiniJeu(int numJeu)
{
	switch (numJeu)
	{
	case 1:
		m_JeuEnCours = new CMiniJeuVache(m_World,NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()));
		break;
	case 2:
		m_JeuEnCours = new CMiniJeuBouteille(m_World, NewtonBodyGetMaterialGroupID(m_tTableBalle[0]->GetBody()));
		break;
	case 3:
		m_JeuEnCours = new CMiniJeuCaisse(m_World, NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()));
		break;
	default:
		return;
	}


	connect(m_JeuEnCours,SIGNAL(miniJeuFini(int)),this,SLOT(FinirMiniJeu(int)));

	NewtonMaterialSetCollisionCallback (m_World, NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()), NewtonBodyGetMaterialGroupID(m_3DpanBout->GetBody()), NULL, NULL, NULL, NULL);
	NewtonMaterialSetCollisionCallback (m_World, NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()), NewtonBodyGetMaterialGroupID(m_3DpanCaisse->GetBody()), NULL, NULL, NULL, NULL);
	NewtonMaterialSetCollisionCallback (m_World, NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()), NewtonBodyGetMaterialGroupID(m_3DpanVache->GetBody()), NULL, NULL, NULL, NULL);
}

/*************************
Auteur: Aubree Loïs et Loïc Teyssier

Usage :		Termine le jeu en cours.
			Affiche le message de victoire correspondant.
			Met a vrai le booleen du jeu terminé.
**************************/
void CWidgetOGL::FinirMiniJeu(int temps)
{
	QSound::play("Yehaw.wav");
	
	m_Menu.m_sAffichage_temps = QString().number(m_JeuEnCours->t_Time.elapsed()/1000);
	m_Menu.m_bMenu = true;
	m_Menu.m_nStat = 3;
	this->setCursor(QCursor(Qt::ArrowCursor));

	if (m_JeuEnCours->getNbElem() == 3)
	{
		m_Menu.m_nTextCourante =16;
		CWidgetOGL::m_bsVacheOk = true;
	}
	if (m_JeuEnCours->getNbElem() == 5)
	{
		m_Menu.m_nTextCourante =15;
		CWidgetOGL::m_bsBouteilleOk = true;
	}
	if (m_JeuEnCours->getNbElem() == 4)
	{
		m_Menu.m_nTextCourante =14;
		CWidgetOGL::m_bsCaisseOk = true;
	}

	delete m_JeuEnCours;
	m_JeuEnCours = NULL;

	NewtonMaterialSetCollisionCallback (m_World, NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()), NewtonBodyGetMaterialGroupID(m_3DpanVache->GetBody()), NULL, ContactBegin, ContactProcessPanneauVache, ContactEnd);
	NewtonMaterialSetCollisionCallback (m_World, NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()), NewtonBodyGetMaterialGroupID(m_3DpanBout->GetBody()), NULL, ContactBegin, ContactProcessPanneauBouteille, ContactEnd);
	NewtonMaterialSetCollisionCallback (m_World, NewtonBodyGetMaterialGroupID(m_3Dperso->GetBody()), NewtonBodyGetMaterialGroupID(m_3DpanCaisse->GetBody()), NULL, ContactBegin, ContactProcessPanneauCaisse, ContactEnd);
		
}