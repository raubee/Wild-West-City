#include "Matrix.h"

CMatrix::CMatrix(void)
{
	 for (int x = 0 ; x < 4 ; ++x)
      for (int y = 0 ; y < 4 ; y++)
            m_Mat[x][y] = 0.0f;
}

CMatrix::~CMatrix(void)
{
}

void CMatrix::setIdentite()
{
	for (int x = 0 ; x < 4 ; ++x)
      for (int y = 0 ; y < 4 ; y++)
      {
         if (x == y)
            m_Mat [x][y] = 1.0f;
         else
            m_Mat [x][y] = 0.0f;
       }
}

CVector3 CMatrix::getPosition()
{
	return CVector3 (m_Mat [3][0],m_Mat [3][1],m_Mat [3][2]);
}
