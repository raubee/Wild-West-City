#pragma once
#include <QTimer>
#include <QSound>
/*
*
* Auteur : Loic Teyssier
*
* Classe CMiniJeu
* Usage :	Lance le timer et le chronometre des jeux.
*			Declenche un signal lorsque la variable reussi a atteint le nombre attendu
*				par le minijeu lancé.
*/

#include <QTime>
#include <QGLWidget>
#include <QMessageBox>
#include <QString>
#include "Matrix.h"
#include "3dmodel.h"
#include "3ds.h"


class CMiniJeu: public QGLWidget
{
	Q_OBJECT

public:
	CMiniJeu(const int nb);
	~CMiniJeu();

	virtual void Dessiner()=0;
	static int m_nReussi;
	QString m_sNameElem;
	QTime t_Time;

	int getNbElem() {return m_nNbElem;}

protected:
	QTimer *t_Timer;
	

	NewtonWorld *m_nWorld;

	const int m_nNbElem;
	CLoad3DS * m_JeuLoader;

signals:
	void miniJeuFini(int temps);

public slots:
    void timeOutSlot();

};