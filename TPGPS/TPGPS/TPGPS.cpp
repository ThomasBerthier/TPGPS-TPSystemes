//*********************************************************************************************
//* Programme : TPGPS.cpp								Date : 30/11/2021
//*														Derni�re MAJ : 30/11/2021
//*--------------------------------------------------------------------------------------------
//*Programmeurs :	BERTHIER Thomas							Classe : BTSSN2
//*					COLSON Baptiste
//*					LARIDANT Julien 
//*--------------------------------------------------------------------------------------------
//*
//* BUT : Permet de r�cup�rer les trames GPS pendant 15 secondes et d'afficher la trame compl�te et ses parties.
//*
//*********************************************************************************************
#include "TPGPS.h"

TPGPS::TPGPS(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//Ouverture de la conexion s�rie entre le GPS et l'application
	port = new QSerialPort(this);
	port->setPortName("COM1");
	port->open(QIODevice::ReadWrite);
	port->setBaudRate(QSerialPort::Baud4800);
	port->setDataBits(QSerialPort::DataBits::Data8);
	port->setParity(QSerialPort::Parity::NoParity);
	port->setStopBits(QSerialPort::StopBits::OneStop);
	port->setFlowControl(QSerialPort::NoFlowControl);
	qDebug() << "Port connect�";
	//Instancie un timer
	timer = new QTimer();
}

//Slot qui permet de d�marrer le timer et la lecture du port s�rie
void TPGPS::startAcquisition()
{
	//Lance le timer d'une dur�e de 15 secondes ( le timer est instanci� a 17 secondes pour �viter une acquisition en moins si l'utilisateur lance le timer sur une trame d�ja commenc�e )
	timer->start(17000);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(stopTimerAcquisition()));
	//efface le contenu du tampon s�rie
	port->clear();
	QObject::connect(port, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()));
}

//Slot qui stop l'acquisition des donn�es et vide les champs de l'interface utilisateur
void TPGPS::stopTimerAcquisition()
{
	timer->stop();
	QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(stopTimerAcquisition()));
	QObject::disconnect(port, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()));
	i = 0;
	ui.latitude->setText("");
	ui.longitude->setText("");
	ui.trame->setText("");
	ui.timeDisplay->setText("");
}

//Slot qui r�cup�re la trame et effectue les modifications n�cessaires pour extraire les informations utiles � l'utilisateur
void TPGPS::onSerialPortReadyRead() {
	//R�cup�re les chaines de caract�res et les ajoutent entre elles jusqu'a ce qu'elles soient compl�tes
	QByteArray dataR = port->readAll();
	data = data + dataR.toStdString().c_str();

	QRegExp regExp("\\$GPGGA(.+)(\\*)"); //\\$
	//Si la trame est compl�te ( correspond � l'expressi�on r�guli�re )
	if (regExp.indexIn(data) > -1)
	{
		QStringList resData = regExp.capturedTexts();
		QString resDataTemp = resData[0];
		//S�pare la chaine de caract�res en utilisant les virgules de la trame
		dataList = resDataTemp.split(QLatin1Char(','), Qt::SkipEmptyParts);

		//Si la trame est correcte on traite la trame
		if (dataList[0] == "$GPGGA") {
			ui.trame->setText(resDataTemp);
			// R�cuperation du temps
			QString time = dataList[1].toStdString().c_str();
			//Changement du format de chaine de caract�res � un format d'affichage de l'heure
			QStringList ListTime = time.split(QLatin1Char('.'), Qt::SkipEmptyParts);
			QString timeWithoutColon = ListTime[0].toStdString().c_str();
			QTime timeWithColon = QTime::fromString(timeWithoutColon, "hhmmss");
			QString usableTime = timeWithColon.toString("hh:mm:ss");
			ui.timeDisplay->setText(usableTime);
			//R�cuperation latitude N/S
			QString latitude = dataList[2].toUtf8();
			//Calcul pour transformer les chiffres repr�sentant la latitude(chiffre d�cimal) en latitude utilisable(degr�e)
			int latitudeDot = latitude.indexOf(".");
			latitude.insert(latitudeDot - 2, ",");
			QStringList latitudeSplit = latitude.split(",");
			//La partie enti�re du nombre 
			double latitudeIntegerDivide = latitudeSplit.at(1).toDouble() / 60;
			//La partie d�cimale du nombre
			double latitudeDecimalDivide = latitudeSplit.at(0).toDouble();
			double latitudeRes = latitudeIntegerDivide + latitudeDecimalDivide;

			//Si latitude Sud on rend celle-ci n�gative
			if (dataList[3] == 'S') {
				latitudeRes = latitudeRes * (-1);
			}
			
			ui.latitude->setText(QString::number(latitudeRes));

			//R�cuperation longitude E/O
			QString longitude = dataList[4].toUtf8();
			//Calcul pour transformer les chiffres repr�sentant la longitude(chiffre d�cimal) en longitude utilisable(degr�e)
			int longitudeDot = longitude.indexOf(".");
			longitude.insert(longitudeDot - 2, ",");
			QStringList longitudeSplit = longitude.split(",");
			//La partie enti�re du nombre 
			double longitudeIntegerDivide = longitudeSplit.at(1).toDouble() / 60;
			//La partie d�cimale du nombre
			double longitudeDecimalDivide = longitudeSplit.at(0).toDouble();
			double longitudeRes = longitudeIntegerDivide + longitudeDecimalDivide;

			//Si longitude Ouest on rend celle-ci n�gative
			if (dataList[5] == 'O') {
				longitudeRes = longitudeRes * (-1);
			}

			ui.longitude->setText(QString::number(longitudeRes));

			//Supprime les donn�es des tableaux et la trame pour recevoir une nouvelle trame
			//qDebug() << dataList;
			data.clear();
			dataList.clear();

		}
		else {
			//Supprime les donn�es des tableaux et la trame si la trame recue est invalide pour recevoir une nouvelle trame
			data.clear();
			dataList.clear();
		}

	}

}



