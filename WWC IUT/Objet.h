#pragma once
#include <iostream>
#include "Newton/newton.h"
#include "Matrix.h"


class CObjet
{

friend void ApplyForceAndTorqueCallback (const NewtonBody * nBody);

protected:
	CVector3 m_vTaille;
	NewtonBody *m_pBody;
	GLfloat m_masse;

public:
	CObjet(void);
	~CObjet(void);

	virtual void Dessiner() = 0;
	virtual void Initialiser(){};
	NewtonBody* GetBody();

};
