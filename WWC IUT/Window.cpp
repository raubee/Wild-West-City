#include "Window.h"

CWindow::CWindow(QWidget *parent, Qt::WFlags flags)
: CWidgetOGL(60, parent, "Wild West City")
{
}

CWindow::~CWindow()
{}

void CWindow::initializeGL()
{
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);	

	glEnable(GL_DEPTH_TEST);

	/***************** Chargement des Textures Modeles 3d *****************/
	m_3Dplateau->LoadTextures();
	m_3Dsol->LoadTextures();
	m_3Dskybox->LoadTextures();
	m_3Dperso->LoadTextures();
	m_3DpanCaisse->LoadTextures();
	m_3DpanBout->LoadTextures();
	m_3DpanVache->LoadTextures();
	m_3DpanEntree->LoadTextures();
	m_3Dcactus->LoadTextures();

	for (int i=0; i<m_nsNbBalles; i++)
		m_tTableBalle[i]->LoadTextures();

	/// Chargement des Menus Page Menu ///
	m_Menu.loadImage("Menu/Menu1.png");
	m_Menu.loadImage("Menu/Menu2.png");	
	m_Menu.loadImage("Menu/Menu3.png");
	m_Menu.loadImage("Menu/Menu4.png");
	m_Menu.loadImage("Menu/Menu5.png");
	m_Menu.loadImage("Menu/Commandes.png");
	m_Menu.loadImage("Menu/Commandes2.png");
	m_Menu.loadImage("Menu/Credits.png");
	m_Menu.loadImage("Menu/Credits2.png");
	m_Menu.loadImage("Menu/entree.png");
	m_Menu.loadImage("Menu/sortie.png");
	m_Menu.loadImage("Menu/pbout.png");
	m_Menu.loadImage("Menu/pcaisse.png");
	m_Menu.loadImage("Menu/pvache.png");
	m_Menu.loadImage("Menu/pbout2.png");
	m_Menu.loadImage("Menu/pcaisse2.png");
	m_Menu.loadImage("Menu/pvache2.png");


	// Mettre la texture courante sur le menu principal
	m_Menu.m_nTextCourante =0;


}

void CWindow::resizeGL(int width, int height)
{
		if (height == 0) height = 1;
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 20000.0f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glMatrixMode(GL_MODELVIEW);
}

void CWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(m_Menu.m_bMenu ==true)
	{
		m_Menu.afficher();
	}
	else
	{
		m_Camera->Dessiner();

		glEnable(GL_LIGHTING);

		glEnable(GL_SMOOTH);
	
		glEnable(GL_LIGHT1);
		glDepthFunc(GL_LEQUAL);


		GLfloat blanc[4] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_position[] = { 0.0, 200.0, 300, 0.0 };

		//Soleil
		glLightfv(GL_LIGHT1, GL_POSITION, light_position);
		glLightfv(GL_LIGHT1, GL_AMBIENT, blanc);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, blanc);
		glLightfv(GL_LIGHT1, GL_SPECULAR, blanc);

		CVector3 omega(0,0,0);
		NewtonBodySetOmega(m_3Dperso->GetBody(), &omega.x);

		// Tester le collision de la Camera avec un autre objet 3D 
		m_3Dplateau->m_bTouchCam = TestCollision(m_Camera->GetBody(), m_3Dplateau->GetBody());

		//Dessiner les éléments 3DS !! dans le cas des d'une collision camera avec un objet 3D.
		m_3Dperso->Dessiner();
		glDisable(GL_LIGHTING);
		m_3Dsol->Dessiner();
		glEnable(GL_LIGHTING);
		m_3Dcactus->Dessiner();
		m_3DpanBout->Dessiner();
		m_3DpanVache->Dessiner();
		m_3DpanCaisse->Dessiner();
		m_3DpanEntree->Dessiner();
		m_3Dplateau->Dessiner();
	
		// Dessiner Balles
		for (int i=0; i<m_nsNbBalles; i++)
		{
			if (m_tTableBalle[i]->GetBody() != NULL)
			{
				m_tTableBalle[i]->Dessiner();
			}
		}

		glEnable(GL_BLEND);

		//Dessiner du temps au milieu du plateau
		if (m_JeuEnCours != NULL)
		{
			glDisable(GL_BLEND);
			m_JeuEnCours->Dessiner();
			glEnable(GL_BLEND);
			glPushMatrix();
			QFont maFonte("Arial", 40, QFont::DemiBold);
			glColor4f(1,0,0,0.5);
			renderText(0,10,0,QString().number(m_JeuEnCours->t_Time.elapsed()/1000),maFonte);
			glPopMatrix();

			glPushMatrix();
			glLoadIdentity();
			QFont font("Arial", 40, QFont::DemiBold);
			glColor4f(1,0,0,0.5);
			QString temps ("Il reste ");
			int nombre = m_JeuEnCours->getNbElem()-CMiniJeu::m_nReussi;
			temps += QString().number(nombre) +" "+ m_JeuEnCours->m_sNameElem;
			renderText(0,0,0,temps,font);
			glPopMatrix();
		}
		//Affichage du scroe au jeu précédent
		if (m_Menu.m_sAffichage_temps != "")
			{
					glPushMatrix();
					glLoadIdentity();
					QFont maFonte("Arial", 40, QFont::DemiBold);
					glColor4f(1,0,0,0.5);
					QString temps ("Ton temps au dernier Jeu est de : ");
					temps += m_Menu.m_sAffichage_temps + " secondes";
					renderText(1,0,0,temps,maFonte);
					glPopMatrix();
			}
		glDisable(GL_BLEND);

		// viseur :
		CMatrix position;
		NewtonBodyGetMatrix(m_3Dperso->GetBody(), &position.m_Mat[0][0]);
		glColor3f(1,1,1);
		glPushMatrix();
		glMultMatrixf(&position.m_Mat[0][0]);
		glBegin(GL_LINES);
			glVertex3f(0,1,0);
			glVertex3f(-500,1+m_PosY,0);
		glEnd();
		glPopMatrix();
	
		// Dessiner Skybox
		glDisable(GL_LIGHTING);
		glPushMatrix();
			glScalef(50,10,50);
			glTranslatef(0,-100,0);
			m_3Dskybox->Dessiner();
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}
}