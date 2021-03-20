/*
*
* Auteur : Aubree Lois et Loic Teyssier
* 
* Classe WidgetOGL
* Usage :	Gere les collisions avec les paneaux.
*			Gere l'interaction avec les touches et la souris.
*			Importe les modeles presents sur le plateau de jeu.
*
*
*/
#ifndef WIDGETOGL_H
#define WIDGETOGL_H

#include <QTimer>
#include <QKeyEvent>
#include <QEvent>
#include <QSound>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QImage>
#include "Newton/newton.h"
#include "Camera.h"
#include "MiniJeuVache.h"
#include "MiniJeuBouteille.h"
#include "MiniJeuCaisse.h"
#include "3ds.h"
#include "Menu.h"


#define M_PI 3.14159265

class CWidgetOGL : public QGLWidget
{
	Q_OBJECT

protected:
	virtual void initializeGL() = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL() = 0;

	CWidgetOGL(int fps = 0, QWidget *parent = 0, char *titre = 0);
	~CWidgetOGL();

	static const int m_nsNbBalles=10;
	static int m_nImageMenu;

	CMiniJeu *m_JeuEnCours;

	/************* Test Model 3ds***************/
	CLoad3DS * m_Loader;
	
	C3DModel * m_3Dsol;
	C3DModel * m_3Dplateau;
	C3DModel * m_3Dperso;
	C3DModel * m_3Dskybox;
	C3DModel * m_3DpanCaisse;
	C3DModel * m_3DpanBout;
	C3DModel * m_3DpanVache;
	C3DModel * m_3DpanEntree;
	C3DModel * m_3Dlimite;
	C3DModel * m_3Dcactus;
	C3DModel ** m_tTableBalle;
	

	CCamera * m_Camera;

	int m_PosY; // position en Y du viseur.(le perso est l'origine)
	
	Menu m_Menu;
	

	bool TestCollision(NewtonBody* body1, NewtonBody* body2);

private:

	static bool m_bsBoolJeuVache, m_bsBoolJeuBouteille, m_bsBoolJeuCaisse, m_bsBoolAskJeuVache,m_bsBoolAskJeuBouteille,m_bsBoolAskJeuCaisse,m_bsBoolAskEntree;

	static int ContactBegin (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
	static int ContactProcessPanneauVache (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessPanneauBouteille (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessPanneauCaisse (const NewtonMaterial* material, const NewtonContact* contact);
	static int ContactProcessPanneauEntree (const NewtonMaterial* material, const NewtonContact* contact);
	static void ContactEnd (const NewtonMaterial* material);

	void mousePressEvent( QMouseEvent *mouseEvent);
	void keyPressEvent( QKeyEvent *keyEvent );
	void mouseMoveEvent(QMouseEvent *moveEvent);
	void mouseReleaseEvent(QMouseEvent *mouseEvent);
	void wheelEvent(QWheelEvent * event);

	NewtonWorld *m_World;
	void NewtonInit();
	void SetTabBalles();

	int m_NumBalle;
	int m_nMouseX , m_nMouseY; // position de la souris.
	bool m_bMoveCam;


	static bool m_bsVacheOk, m_bsBouteilleOk, m_bsCaisseOk; // vraies lorsque le jeu en question est 
															//	fait au moins une fois.

	QTimer *t_Timer;

public slots:
    virtual void timeOutSlot();
	void CommencerMiniJeu(int numJeu);
	void FinirMiniJeu(int temps);

signals:
	void LancerMiniJeu(int numJeu);

};

#endif // WIDGETOGL_H
