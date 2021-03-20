#include "MiniJeu.h"

int CMiniJeu::m_nReussi = 0;

/***************************
Auteur : Loïc Teyssier

Usage : Lance le timer et l chronmettre des jeux et réinitialise reussi.
***************************/
CMiniJeu::CMiniJeu(const int nb)
:m_nNbElem(nb)
{
	int seconde = 1000; // 1 seconde = 1000 ms
    int timerInterval = seconde / 60; // l'intervalle entre deux images
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    t_Timer->start( timerInterval );

	CMiniJeu::m_nReussi = 0;
	t_Time.start();
}

CMiniJeu::~CMiniJeu()
{
}

/***************************
Auteur : Loïc Teyssier

Usage : Renvoie un signal quand le jeu est fini. Passe en paramettre le temps au chronometre.
***************************/
void CMiniJeu::timeOutSlot()
{
	if (CMiniJeu::m_nReussi == m_nNbElem)
	{
		int temps;
		temps = t_Time.elapsed();
		emit miniJeuFini(temps);
		CMiniJeu::m_nReussi = 0;
	}
}