//*********************************************************************************************
//* Programme : main.cpp							Date : 30/11/2021
//*													Dernière MAJ : 30/11/2021
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					COLSON Baptiste
//*					LARIDANT Julien 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet d'instancier la classe TPGPS et de démarrer l'interface utilisateur.
//*
//*********************************************************************************************
#include "TPGPS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TPGPS w;
    w.show();
    return a.exec();
}
