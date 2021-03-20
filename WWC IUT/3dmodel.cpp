#include "3dmodel.h"
#include <QString>
#include <algorithm>
#include <functional>

// ###############
// Classe C3DModel
// ###############

// ============
// Constructeur
// ============
C3DModel::C3DModel() 
{ 
	numOfObjects = 0;
	numOfMaterials = 0;
	m_bTouchCam = false;
	for (int i=0; i<MAX_TEXTURES; i++) m_nTextures[i] = 0;
}


// ===========
// Destructeur
// ===========
C3DModel::~C3DModel() 
{ 
	Destroy();
}


// ===============
// Détruit l'objet
// ===============
void C3DModel::Destroy()
{
	numOfObjects = 0;
	numOfMaterials = 0;
	pObject.clear();
	pMaterials.clear();
	glDeleteTextures(MAX_TEXTURES, m_nTextures);

	NewtonDestroyBody(NewtonBodyGetWorld(m_pBody), m_pBody);
	m_pBody = NULL;
}


// ============================
// Charge les textures du model
// ============================
int C3DModel::LoadTextures()
{
	QString name; 
	QImage t;
	QImage b;

	for(int i = 0; i < numOfMaterials; i++)
	{
		name = "Textures/";
		name+= pMaterials[i].strFile;
		if (!b.load(name))
		 {
		 }
		
		 t = QGLWidget::convertToGLFormat( b );
		 glGenTextures( 1, &m_nTextures[i] );
		 glBindTexture( GL_TEXTURE_2D, m_nTextures[i] );
		 glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
		 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	 }
	return 1;
}


// ================
// Dessine le model
// ================
void C3DModel::Dessiner() 
{ 
	CMatrix matrice;
	if(m_pBody!=NULL)
		NewtonBodyGetMatrix (m_pBody, &matrice.m_Mat [0][0]);

   glPushMatrix ();
   {
		// Pour tous les objets du model
		for(int i = 0; i < numOfObjects; i++)
		{
			// On regarde si on a une liste valide d'objets
			if(pObject.size() <= 0) break;

			// Recupere l'objet courant qu'on va afficher
			t3DObject *object = &(pObject[i]);
			
			// Si l'objet a une texture
			if(object->bHasTexture) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, m_nTextures[object->materialID]);
				if(m_bTouchCam)
				{
					glEnable(GL_BLEND);
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
				}
			} 
			else {
				glDisable(GL_TEXTURE_2D);	
			}

			// Couleur par défaut
			glColor3ub(255, 255, 255);
			if(m_pBody!=NULL)
				glMultMatrixf (&matrice.m_Mat [0][0]);
			// Dessine l'objet
			glBegin(GL_TRIANGLES);
		
				// Pour toutes les faces de l'objet
				for(int j = 0; j < object->numOfFaces; j++)
				{
					// Pour chaque sommet d'une face
					for(int whichVertex = 0; whichVertex < 3; whichVertex++)
					{
						// Recupere l'index du point courant
						int index = object->pFaces[j].vertIndex[whichVertex];
				
						// Donne à OpenGL la normale en ce point
						glNormal3f(object->pNormals[ index ].x, object->pNormals[ index ].y, object->pNormals[ index ].z);
					
						// Si l'objet a une texture, on donne a OpenGL la coordonnee de texture
						if(object->bHasTexture) {
							if(object->pTexVerts) {	// Verifie la validite du tableau des coordonnees
								glTexCoord2f(object->pTexVerts[ index ].x, object->pTexVerts[ index ].y);
							}
						} else {

							// Si il existe un materiel pour cet objet
							if(pMaterials.size() && object->materialID >= 0) 
							{
								// Recupere la couleur du materiel
								unsigned char *pColor = pMaterials[object->materialID].color;

								// Assigne la couleur du materiel au point courant
								glColor3ub(pColor[0], pColor[1], pColor[2]);
							}
						}

						// Donne a OpenGL, les coordoonees du point courant
						glVertex3f(object->pVerts[ index ].x, object->pVerts[ index ].y, object->pVerts[ index ].z);
					}
				}
			glEnd();
			glDisable(GL_BLEND);
		}
   }
   glPopMatrix();
}
void C3DModel::InitialiserPerso (NewtonWorld *nWorld, const CVector3 &taille, const CVector3 &position, GLboolean mobile, GLfloat masse)
{
	// On initialise le vecteur de dimensions
   m_vTaillePerso.x = taille.x;
   m_vTaillePerso.y = taille.y;
   m_vTaillePerso.z = taille.z;

   // On définit la masse de l'objet
   m_masse = masse;

   CMatrix matrice; // On créé une matrice
   matrice.setIdentite();

   // On définit la matrice de manière à ce que l'objet soit placé aux positions
   // spécifiées en utilisant la dernière colonne de la matrice
   matrice.m_Mat[3][0] = position.x;
   matrice.m_Mat [3][1] = position.y;
   matrice.m_Mat [3][2] = position.z;

   // On initialise la boîte de collision
   NewtonCollision * collision = NULL;
   // On créé la boite de collision aux dimensions de l'objet
   collision = NewtonCreateBox (nWorld, m_vTaillePerso.x, m_vTaillePerso.y, m_vTaillePerso.z, NULL);
   // On initialise le corps avec la boite de collision
   m_pBody = NewtonCreateBody (nWorld, collision);

   if (m_pBody == NULL)
      std::cerr << "Impossible d'initialiser le corps.";
   // On détruit la boite de collision, on n'en a plus besoin
   NewtonReleaseCollision (nWorld, collision);

   // Enfin, on affecte notre matrice (qui représente donc sa position dans l'espace)
   // à notre corps grâce à la fonction NewtonBodySetMatrix
   NewtonBodySetMatrix (m_pBody, &matrice.m_Mat [0][0]);

   // On initialise à présent les propriétés physiques de l'objet. Toutefois, donner
   // à un objet qui ne bougera pas une masse, lui associer un callback,... n'a aucun
   // intêret, on vérifie donc si l'objet sera mobile ou immobile
   if (mobile == true)
   {
      // On calcul l'inertie du corps, en passant par une petite formule
      CVector3 inertie;

      inertie.x = 0.7f * m_masse * (m_vTaillePerso.y * m_vTaillePerso.y + m_vTaillePerso.z * m_vTaillePerso.z) / 12;
      inertie.y = 0.7f * m_masse * (m_vTaillePerso.x * m_vTaillePerso.x + m_vTaillePerso.z * m_vTaillePerso.z) / 12;
      inertie.z = 0.7f * m_masse * (m_vTaillePerso.x * m_vTaillePerso.x + m_vTaillePerso.y * m_vTaillePerso.y) / 12;

      // On définit ensuite la masse et l'inertie pour ce corps
      NewtonBodySetMassMatrix (m_pBody, m_masse, inertie.x, inertie.y, inertie.z);

      // On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
      NewtonBodySetForceAndTorqueCallback (m_pBody, ApplyForceAndTorqueCallback);
   }
}
void C3DModel::Initialiser(NewtonWorld *nWorld, GLboolean mobile, GLfloat masse, GLboolean isSol)
{
	
	m_masse=masse;
	// Création de l'arbre de collision pour le sol
	   NewtonCollision * nCollision = NULL;

	   nCollision = NewtonCreateTreeCollision(nWorld,NULL);

	if(isSol)
	{
		// Recupere l'objet sol et fait les HeightMaps du sol
		t3DObject *object = &(pObject[0]);
	   NewtonTreeCollisionBeginBuild(nCollision);

	   GLfloat fVert[9];
	   GLint strideInBytes = sizeof(GLfloat) * 3;

	// Pour toutes les faces de l'objet
		for(int j = 0; j < object->numOfFaces; j++)
		{
			// Pour chaque sommet d'une face
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Recupere l'index du point courant
				int index = object->pFaces[j].vertIndex[whichVertex];

				// Donne a OpenGL, les coordoonees du point courant
						fVert[0+3*whichVertex]=object->pVerts[ index ].x, 
						fVert[1+3*whichVertex]=object->pVerts[ index ].y, 
						fVert[2+3*whichVertex]=object->pVerts[ index ].z;
			}
			//On ajoute la face à l'abre de collision
			NewtonTreeCollisionAddFace(nCollision,3,&fVert[0],strideInBytes,0);
		}
		NewtonTreeCollisionEndBuild (nCollision, 1);
		m_pBody = NewtonCreateBody (nWorld, nCollision);
		
		NewtonReleaseCollision (nWorld, nCollision); // On libère l'arbre de collision
	}
	else
	{
		vector <NewtonCollision *> enveloppeConvexe;
		GLint strideInBytes = sizeof(GLfloat) * 3;

		// Pour tous les objets du model
		for(int i = 0; i < numOfObjects; i++)
		{
			t3DObject *object = &(pObject[i]);
			nCollision = NewtonCreateConvexHull(nWorld,object->numOfVerts,&object->pVerts[0].x,strideInBytes,NULL);
			enveloppeConvexe.push_back (nCollision);
		}
		nCollision = NewtonCreateCompoundCollision(nWorld,numOfObjects,&enveloppeConvexe[0]);
		m_pBody = NewtonCreateBody (nWorld, nCollision);

		// libérér vector d'enveloppe convexe
		std::for_each (enveloppeConvexe.begin (), enveloppeConvexe.end (), bind1st(ptr_fun(&NewtonReleaseCollision), nWorld));
		
	   NewtonReleaseCollision (nWorld, nCollision); // On libère la primitive de collision

		if (mobile == true)
		{
		  CVector3 inertie;
		  CVector3 origine;
		  CVector3 inertieFinale;

		  NewtonCollision * nCollision = NewtonBodyGetCollision(m_pBody);

		  //Calcul de l'inertie du corps
		  NewtonConvexCollisionCalculateInertialMatrix(nCollision,&inertie.x,&origine.x);
		  NewtonBodySetCentreOfMass(m_pBody,&origine.x);

		  //on multiplie les valeurs de l'inertie par la masse
		  inertieFinale.x = inertie.x * m_masse;
		  inertieFinale.y = inertie.y * m_masse;
		  inertieFinale.z = inertie.z * m_masse;

		  // Definir Masse et inertie du corps
		  NewtonBodySetMassMatrix (m_pBody, m_masse, inertieFinale.x, inertieFinale.y, inertieFinale.z);

		  // CallBack
		  NewtonBodySetForceAndTorqueCallback (m_pBody, ApplyForceAndTorqueCallback);
		}
	}
}