//*********************************************************************************************
//* Programme : TPGPS.h								Date : 30/11/2021
//*													Dernière MAJ : 30/11/2021
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					COLSON Baptiste
//*					LARIDANT Julien 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet de récupérer les trames GPS pendant 15 secondes et d'afficher la trame complète et ses parties.
//*
//*********************************************************************************************
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TPGPS.h"
#include <QObject>
#include <QSerialPort>
#include <qdebug.h>
#include <QRegExp>
#include <qtimer.h>
#include <QTime> 

class TPGPS : public QMainWindow
{
    Q_OBJECT

public:
    TPGPS(QWidget *parent = Q_NULLPTR);

private:
    Ui::TPGPSClass ui;
	QSerialPort * port;
	QString data;
	QStringList dataList;
	QTimer * timer;
	int i = 0;

public slots:
	void onSerialPortReadyRead();
	void startAcquisition();
	void stopTimerAcquisition();

};
