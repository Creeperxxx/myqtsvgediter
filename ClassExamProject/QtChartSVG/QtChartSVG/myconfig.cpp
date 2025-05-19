#include <QJsonDocument>
#include <QFile>
#include <QJsonValue>
#include "myconfig.h"


myconfig& myconfig::getInstance()
{
	static myconfig instance;
	return instance;
}

void myconfig::loadUserJson(QString filepath)
{
	QFile configfile(filepath);
	if (!configfile.open(QIODevice::ReadOnly))
		throw std::runtime_error("error");
	QByteArray jsondata = configfile.readAll();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsondata);
	configfile.close();
	if(!jsonDoc.isObject())
		throw std::runtime_error("error");
	m_devjson = jsonDoc.object();
}

QString myconfig::getRectName()
{
	static QString rectName = m_devjson["name"].toObject()["diagram"].toObject()["rectangle"].toString();
	return rectName;
}

QString myconfig::getCircleName()
{
	static QString circleName = m_devjson["name"].toObject()["diagram"].toObject()["circle"].toString();
	return circleName;
}

QString myconfig::getTriangleName()
{
	static QString triangleName = m_devjson["name"].toObject()["diagram"].toObject()["triangle"].toString();
	return triangleName;
}

QString myconfig::getLineName()
{
	static QString lineName = m_devjson["name"].toObject()["diagram"].toObject()["line"].toString();
	return lineName;
}

QString myconfig::getMouseName()
{
	static QString mouseName = m_devjson["name"].toObject()["diagram"].toObject()["mouse"].toString();
	return mouseName;
}

QString myconfig::getChooseName()
{
	static QString chooseName = m_devjson["name"].toObject()["diagram"].toObject()["choose"].toString();
	return chooseName;
}

QString myconfig::getTextName()
{
	static QString textName = m_devjson["name"].toObject()["diagram"].toObject()["text"].toString();
	return textName;
}

QString myconfig::getCanvasWidthName()
{
	static QString canvasWidthName = m_devjson["name"].toObject()["canvas"].toObject()["widthname"].toString();
	return canvasWidthName;
}

QString myconfig::getCanvasHeightName()
{
	static QString canvasHeightName = m_devjson["name"].toObject()["canvas"].toObject()["heightname"].toString();
	return canvasHeightName;
}

QString myconfig::getCanvasName()
{
	static QString canvasName = m_devjson["name"].toObject()["canvas"].toObject()["canvasname"].toString();
	return canvasName;
}

QString myconfig::getNameName()
{
	static QString nameName = m_devjson["name"].toObject()["other"].toObject()["namename"].toString();
	return nameName;
}

QString myconfig::getCenterHOffsetName()
{
	static QString centerHOffsetName = m_devjson["name"].toObject()["other"].toObject()["centerhoffsetname"].toString();
	return centerHOffsetName;
}

QString myconfig::getCenterVOffsetName()
{
	static QString centerVOffsetName = m_devjson["name"].toObject()["other"].toObject()["centervoffsetname"].toString();
	return centerVOffsetName;
}

QString myconfig::getSpaceHeightName()
{
	static QString spaceHeightName = m_devjson["name"].toObject()["other"].toObject()["spaceheightname"].toString();
	return spaceHeightName;
}

QString myconfig::getSpaceWidthName()
{
	static QString spaceWidthName = m_devjson["name"].toObject()["other"].toObject()["spacewidthname"].toString();
	return spaceWidthName;
}

QString myconfig::getPenColorName()
{
	static QString penColorName = m_devjson["name"].toObject()["other"].toObject()["pencolorname"].toString();
	return penColorName;
}

QString myconfig::getPenWdithName()
{
	static QString penWidthName = m_devjson["name"].toObject()["other"].toObject()["penwidthname"].toString();
	return penWidthName;
}

QString myconfig::getBrushColorName()
{
	static QString brushColorName = m_devjson["name"].toObject()["other"].toObject()["brushcolorname"].toString();
	return brushColorName;
}

QString myconfig::getRotateAngleName()
{
	static QString rotateAngleName = m_devjson["name"].toObject()["other"].toObject()["rotateanglename"].toString();
	return rotateAngleName;
}

QString myconfig::getScaleName()
{
	static QString scaleName = m_devjson["name"].toObject()["other"].toObject()["scalename"].toString();
	return scaleName;
}

QString myconfig::getRectRadioName()
{
	static QString rectRadioName = m_devjson["name"].toObject()["other"].toObject()["rectradioname"].toString();
	return rectRadioName;
}

QString myconfig::getCircleRadioName()
{
	static QString circleRadioName = m_devjson["name"].toObject()["other"].toObject()["circleradioname"].toString();
	return circleRadioName;
}

QString myconfig::getTriangleRadioName()
{
	static QString triangleRadioName = m_devjson["name"].toObject()["other"].toObject()["triangleradioname"].toString();
	return triangleRadioName;
}

QString myconfig::getEdgeTypeName()
{
	static QString edgeTypeName = m_devjson["name"].toObject()["other"].toObject()["edgetypename"].toString();
	return edgeTypeName;
}

QString myconfig::getFontFamilyName()
{
	static QString fontFamilyName = m_devjson["name"].toObject()["other"].toObject()["fontfamilyname"].toString();
	return fontFamilyName;
}

QString myconfig::getFontSizeName()
{
	static QString fontSizeName = m_devjson["name"].toObject()["other"].toObject()["fontsizename"].toString();
	return fontSizeName;
}




myconfig::myconfig()
{
	loadDevJson();
}

void myconfig::loadDevJson()
{
	QFile configfile(":/new/devconfig.json");
	if (!configfile.open(QIODevice::ReadOnly))
		throw std::runtime_error("error");
	QByteArray jsondata = configfile.readAll();
	QJsonDocument jsonDoc= QJsonDocument::fromJson(jsondata);
	configfile.close();
	if(!jsonDoc.isObject())
		throw std::runtime_error("error");

	m_devjson = jsonDoc.object();

}
