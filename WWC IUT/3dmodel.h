/*
 * Classe provenant du 3ds Loader Glut
 *
 * Adaptation au Projet WWC faite par Aubree Lois :
 * Liste des Adaptations :
 *
 *  - methode LoadTextures entierement réecrite pour permettre un chargement avec QImage
 *  - Heritage Newton 
 *  - Initialiser et InitialiserPerso methodes permettant de rattacher les modeles 3d à newton
 *  - Dessiner est un methode permettant l'affichage  des modeles 3ds (3DSLoaderGlut) suivant les multiplications matricielles de newton
 *
 */

#ifndef _3DMODEL_H
#define _3DMODEL_H

#include <vector>
#include "vector.h"
#include "Objet.h"
#include <QImage>

using namespace std;

// Face composées de 3 points et de 3 coordonnées de texture
typedef struct
{
	int vertIndex[3];
	int coordIndex[3];
} tFace;

// Objet 3D
typedef struct 
{
	int  numOfVerts;			// Nombre de points
	int  numOfFaces;			// Nombre de faces
	int  numTexVertex;			// Nombre de coordonnées de texture
	int  materialID;			// Texture ID, dans le tableau de textures de la classe C3DModel
	bool bHasTexture;			// Vrai si l'objet a une texture
	char strName[255];			// Nom de l'objet
	CVector3  *pVerts;			// Points de l'objet
	CVector3  *pNormals;		// Normales de l'objet
	CVector2  *pTexVerts;		// Coordonnées de textures
	tFace *pFaces;				// Faces de l'objet
} t3DObject;

// Info d'un materiel (texture)
typedef struct 
{
	char  strName[255];			// Nom de la texture
	char  strFile[255];			// Fichier contenant la texture
	unsigned char color[3];		// Couleur (R, G, B)
	int   texureId;				// Texture ID
	float uTile;				// u tiling of texture  (Currently not used)
	float vTile;				// v tiling of texture	(Currently not used)
	float uOffset;			    // u offset of texture	(Currently not used)
	float vOffset;				// v offset of texture	(Currently not used)
} tMaterialInfo;


// ###############
// Classe C3DModel
// ###############
class C3DModel : 
	public CObjet
{

public:
	// Nombre d'objets du model
	int numOfObjects;
	// Nombre de materiels pour le model
	int numOfMaterials;
	// Liste des infos de materiel (textures et couleurs)
	vector<tMaterialInfo> pMaterials;
	// Liste des objets du model
	vector<t3DObject> pObject;

	// ============
	// Constructeur
	// ============
	C3DModel();

	// ===========
	// Destructeur
	// ===========
	~C3DModel();

	// ===============
	// Détruit l'objet
	// ===============
	void Destroy();

	// ============================
	// Charge les textures du model
	// ============================
	int LoadTextures();

	// True si l'objet touche la camera
	bool m_bTouchCam;

	/* Methodes virtuelles heritées de CObjet*/
	void Dessiner();
	void Initialiser (NewtonWorld *nWorld, GLboolean mobile=false, GLfloat masse=1.0f, GLboolean isSol=false);
	void InitialiserPerso (NewtonWorld *nWorld, const CVector3 &taille, const CVector3 &position, GLboolean mobile, GLfloat masse=1.0f);
private:

	// Nombre maximum de textures
	#define MAX_TEXTURES  200

	// Tableau contenant les ids des textures du model
	GLuint m_nTextures[MAX_TEXTURES];						// This holds the texture info, referenced by an ID

	CVector3 m_vTaillePerso;
	

};

#endif

