#include "Vector.h"

CVector::CVector ()
   : x (0.0f), y (0.0f), z (0.0f)
{
}

CVector::CVector (const GLfloat fX, const GLfloat fY, const GLfloat fZ)
   : x (fX), y (fY), z (fZ)
{
}

CVector::~CVector()
{
}

void CVector::setCoordonnees (const GLfloat fX, const GLfloat fY, const GLfloat fZ)
{
   x = fX;
   y = fY;
   z = fZ;
}

void CVector::getNorme()
{
	GLfloat norme;

	norme = x*x + y*y + z*z;
	norme = sqrt(norme);

	x /= norme;
	y /= norme;
	z /= norme;

	x*=10;
	y*=10;
	z*=10;
}

void CVector::ajouter(CVector vect)
{
	x += vect.x;
	y += vect.y;
	z += vect.z;
}