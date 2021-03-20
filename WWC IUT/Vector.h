/*
 * 05/28/2003
 *
 * vector.h
 * Classe permettant la gestion des vecteurs 3D 
 *
 * Thierry Vouriot (Yéri)
 * yeri@fr.st
 * http://www.yeri.fr.st
 * thierry.vouriot@free.fr
 * http://thierry.vouriot.free.fr
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _VECTOR_H
#define _VECTOR_H

#include <math.h>
#include <QGLWidget>

// Type scalaire
typedef float GLfloat;

// ###############
// Classe CVector3
// ###############
class CVector3
{
public:
	
	// Les 3 composantes d'un vecteur
	GLfloat x, y, z;

	// =======================
	// Constructeur par defaut
	// =======================
	CVector3() { x = 0.0; y = 0.0; z = 0.0; }

	// ===========
	// Destructeur
	// ===========
	~CVector3() { }

	// ====================================================================
	// Constructeur permetant de creer un vecteur avec des valeurs definies
	// ====================================================================
	CVector3(GLfloat X, GLfloat Y, GLfloat Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// ===========================================================
	// Surdefinition de l'operateur +, pour l'addition de vecteurs
	// ===========================================================
	CVector3 operator+(CVector3 vVector)
	{
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// ================================================================
	// Surdefinition de l'operateur -, pour la soustraction de vecteurs
	// ================================================================
	CVector3 operator-(CVector3 vVector)
	{
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	CVector3 operator-()
	{
		return CVector3(-x, -y, -z);
	}
	
	// ===================================================================================
	// Surdefinition de l'operateur *, pour la multiplication d'un vecteur par un scalaire
	// ===================================================================================
	CVector3 operator*(GLfloat num)
	{
		return CVector3(x * num, y * num, z * num);
	}

	// =============================================================================
	// Surdefinition de l'operateur /, pour la division d'un vecteur par un scalaire
	// =============================================================================
	CVector3 operator/(GLfloat num)
	{
		num = 1/num;
		return CVector3(x * num, y * num, z * num);
	}

	// ================================================================================
	// Surdefinition de l'operateur ^, pour le produit vectoriel de vecteurs
	// Le produit Vectoriel renvoi le vecteur perpendiculaire aux 2 vecteurs multiplies
	// ================================================================================
	CVector3 operator^(CVector3 vVector)
	{
		CVector3 vNormal;
		// Valeur X:  (V1.y * V2.z) - (V1.z * V2.y)	
		vNormal.x = ((y * vVector.z) - (z * vVector.y));								
		// Valeur Y:  (V1.z * V2.x) - (V1.x * V2.z)
		vNormal.y = ((z * vVector.x) - (x * vVector.z));
		// Valeur Z:  (V1.x * V2.y) - (V1.y * V2.x)
		vNormal.z = ((x * vVector.y) - (y * vVector.x));
		return vNormal;		
	}

	// =================================================================
	// Surdefinition de l'operateur %, pour le "dot" produit de vecteurs
	// Le "dot" produit renvoi le cos de l'angle entre les 2 vecteurs
	// Attention les 2 vecteurs doivent etre de longueur 1 (normalise)
	// =================================================================
	const GLfloat operator%(const CVector3& vVector) const
	{
		return x*vVector.x + y*vVector.y + z*vVector.z;
	}
	
	// =====================
	// Longueur d'un vecteur
	// =====================
	static GLfloat Length(CVector3 vNormal)
	{
		// length = sqrt(V.x^2 + V.y^2 + V.z^2)  V est le vecteur
		return (GLfloat)sqrt((vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z));
	}

	// =====================
	// Longueur d'un vecteur
	// =====================
	GLfloat Length()
	{
		// length = sqrt(V.x^2 + V.y^2 + V.z^2)  V est le vecteur
		return (GLfloat)sqrt((x * x) + (y * y) + (z * z));
	}

	// ==========================
	// Normalisation d'un vecteur
	// ==========================
	static CVector3 Normalize(CVector3 vVector)
	{
		GLfloat len = Length(vVector);
		if (len != 0.0)
			return vVector / len;	
		else return vVector;
	}

	// ==========================
	// Normalisation d'un vecteur
	// ==========================
	void Normalize()
	{
		GLfloat len = Length();
		if (len != 0.0)
		{
			len = 1/len;
			x = x * len; y = y * len; z = z * len;
		}
	}
	void setCoordonnees(GLfloat X,GLfloat Y , GLfloat Z)
	{
		x = X; y = Y; z = Z;
	}
};


// ##############
// Classe CVector
// ##############
class CVector2 
{
public:
	// Les 2 composantes d'un vecteur
	GLfloat x, y;

	// =======================
	// Constructeur par defaut
	// =======================
	CVector2() { }

	// ===========
	// Destructeur
	// ===========
	~CVector2() { }

	// ====================================================================
	// Constructeur permetant de creer un vecteur avec des valeurs definies
	// ====================================================================
	CVector2(GLfloat X, GLfloat Y) 
	{ 
		x = X; y = Y;
	}
};

#endif

