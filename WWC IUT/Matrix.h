#pragma once
#include "vector.h"

class CMatrix
{
public:
	GLfloat m_Mat[4][4];

	void setIdentite();

	CMatrix(void);
	~CMatrix(void);

	CVector3 getPosition();
};
