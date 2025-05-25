#include <QJsonDocument>
#include <QFile>
#include <QJsonValue>
#include <qcolor.h>
#include <stdexcept>
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
	if (!jsonDoc.isObject())
		throw std::runtime_error("error");
	m_userjson = jsonDoc.object();
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

QString myconfig::getTriangleRadioBottomName()
{
	static QString triangleRadioBottomName = m_devjson["name"].toObject()["other"].toObject()["triangleradiobottomname"].toString();
	return triangleRadioBottomName;
}

QString myconfig::getTriangleRadioLeftName()
{
	static QString triangleRadioLeftName = m_devjson["name"].toObject()["other"].toObject()["triangleradioleftname"].toString();
	return triangleRadioLeftName;
}

QString myconfig::getTriangleRadioRightName()
{
	static QString triangleRadioRightName = m_devjson["name"].toObject()["other"].toObject()["triangleradiorightname"].toString();
	return triangleRadioRightName;
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

double myconfig::getRectRadio()
{
	static double rectRadio = m_userjson["diagram"].toObject()["rectangle"].toObject()["radio"].toDouble();
	return rectRadio;
}

int myconfig::getRectRotate()
{
	static int rectRotate = m_userjson["diagram"].toObject()["rectangle"].toObject()["rotate"].toInt();
	return rectRotate;
}

double myconfig::getRectScale()
{
	static double rectScale = m_userjson["diagram"].toObject()["rectangle"].toObject()["scale"].toDouble();
	return rectScale;
}

double myconfig::getCircleRadio()
{
	static double circleRadio = m_userjson["diagram"].toObject()["circle"].toObject()["radio"].toDouble();
	return circleRadio;
}

int myconfig::getCircleRotate()
{
	static int circleRotate = m_userjson["diagram"].toObject()["circle"].toObject()["rotate"].toInt();
	return circleRotate;
}

double myconfig::getCircleScale()
{
	static double circleScale = m_userjson["diagram"].toObject()["circle"].toObject()["scale"].toDouble();
	return circleScale;
}

int myconfig::getTriangleBottomRadio()
{
	static int triangleBottomRadio = m_userjson["diagram"].toObject()["triangle"].toObject()["edgeradio"].toObject()["bottom"].toInt();
	return triangleBottomRadio;
}

int myconfig::getTriangleLeftRadio()
{
	static int triangleLeftRadio = m_userjson["diagram"].toObject()["triangle"].toObject()["edgeradio"].toObject()["left"].toInt();
	return triangleLeftRadio;
}

int myconfig::getTriangleRightRadio()
{
	static int triangleRightRadio = m_userjson["diagram"].toObject()["triangle"].toObject()["edgeradio"].toObject()["right"].toInt();
	return triangleRightRadio;
}

QString myconfig::getTriangleEdgetype()
{
	static QString triangleEdgetype = m_userjson["diagram"].toObject()["triangle"].toObject()["edgetype"].toString();
	return triangleEdgetype;
}

int myconfig::getTriangleRotate()
{
	static int triangleRotate = m_userjson["diagram"].toObject()["triangle"].toObject()["rotate"].toInt();
	return triangleRotate;
}

int myconfig::getTriangleScale()
{
	static int triangleScale = m_userjson["diagram"].toObject()["triangle"].toObject()["scale"].toInt();
	return triangleScale;
}

int myconfig::getLineRotate()
{
	static int lineRotate = m_userjson["diagram"].toObject()["line"].toObject()["rotate"].toInt();
	return  lineRotate;
}

double myconfig::getLineScale()
{
	static int lineScale = m_userjson["diagram"].toObject()["line"].toObject()["scale"].toDouble();
	return lineScale;
}

QString myconfig::getTextFamily()
{
	static QString textFamily = m_userjson["diagram"].toObject()["text"].toObject()["family"].toString();
	return textFamily;
}

int myconfig::getTextSize()
{
	static int textSize = m_userjson["diagram"].toObject()["text"].toObject()["size"].toInt();
	return textSize;
}

QString myconfig::getDefaultName()
{
	static QString defaultString = m_devjson["other"].toObject()["defaultstring"].toString();
	return defaultString;
}

double myconfig::getRectRadioMax()
{
	static double rectRadioMax = m_userjson["diagram"].toObject()["rectangle"].toObject()["radiomax"].toDouble();
	return rectRadioMax;
}

double myconfig::getRectRadioMin()
{
	static double rectRadioMin = m_userjson["diagram"].toObject()["rectangle"].toObject()["radiomin"].toDouble();
	return rectRadioMin;
}

double myconfig::getRectRadioStep()
{
	static double rectRadioStep = m_userjson["diagram"].toObject()["rectangle"].toObject()["radiostep"].toDouble();
	return rectRadioStep;
}

double myconfig::getCircleRadioMax()
{
	static double circleRadioMax = m_userjson["diagram"].toObject()["circle"].toObject()["radiomax"].toDouble();
	return circleRadioMax;
}

double myconfig::getCircleRadioMin()
{
	static double circleRadioMin = m_userjson["diagram"].toObject()["circle"].toObject()["radiomin"].toDouble();
	return circleRadioMin;
}

double myconfig::getCircleRadioStep()
{
	static double circleRadioStep = m_userjson["diagram"].toObject()["circle"].toObject()["radiostep"].toDouble();
	return circleRadioStep;
}

int myconfig::getTriangleRadioMax()
{
	static int triangleRadioMax = m_userjson["diagram"].toObject()["triangle"].toObject()["radiomax"].toInt();
	return triangleRadioMax;
}

QColor myconfig::getPenColor()
{
	static QColor penColor = QColor(m_userjson["painter"].toObject()["pen"].toObject()["color"].toString());
	return penColor;
}

int myconfig::getPenWidth()
{
	static int penWidth = m_userjson["painter"].toObject()["pen"].toObject()["width"].toInt();
	return penWidth;
}

int myconfig::getPenWidthMax()
{
	static int penWidthMax = m_userjson["painter"].toObject()["pen"].toObject()["widthmax"].toInt();
	return penWidthMax;
}

QColor myconfig::getBrushColor()
{

	static QColor brushColor = QColor(m_userjson["painter"].toObject()["brush"].toObject()["color"].toString());
	return brushColor;
}

int myconfig::getSpaceWidth()
{
	static int spaceWidth = m_userjson["spacesize"].toObject()["width"].toInt();
	return spaceWidth;
}

int myconfig::getSpaceHeight()
{
	static int spaceHeight = m_userjson["spacesize"].toObject()["height"].toInt();
	return spaceHeight;
}

int myconfig::getSpaceLengthMax()
{
	static int spaceLengthMax = m_userjson["spacesize"].toObject()["lengthmax"].toInt();
	return spaceLengthMax;
}

int myconfig::getCanvasWidth()
{
	static int canvasWidth = m_userjson["canvas"].toObject()["width"].toInt();
	return canvasWidth;
}

int myconfig::getCanvasHeight()
{
	static int canvasHeight = m_userjson["canvas"].toObject()["height"].toInt();
	return canvasHeight;
}

int myconfig::getCanvasLengthMax()
{
	static int canvasLengthMax = m_userjson["canvas"].toObject()["lengthmax"].toInt();
	return canvasLengthMax;
}

QColor myconfig::getDiagramButtonBackgroundColor()
{
	static QColor color = QColor(m_userjson["diagrambutton"].toObject()["backgroundcolor"].toString());
	return color;
}

bool myconfig::getDiagramButtonIsSizeFixed()
{
	static bool isSizeFixed = m_userjson["diagrambutton"].toObject()["isfixsize"].toBool();
	return isSizeFixed;
}

double myconfig::getDiagramButtonWidgetRadio()
{
	static double radio = m_userjson["diagrambutton"].toObject()["widgetradio"].toDouble();
	return radio;
}

QSize myconfig::getDiagramButtonFixedSize()
{
	static QSize size = QSize(m_userjson["diagrambutton"].toObject()["fixwidth"].toInt()
		, m_userjson["diagrambutton"].toObject()["fixheight"].toInt());
	return size;
}

QSize myconfig::getDiagramButtonMaxSize()
{
	static QSize size = QSize(m_userjson["diagrambutton"].toObject()["maxwidth"].toInt()
		, m_userjson["diagrambutton"].toObject()["maxheight"].toInt());
	return size;
}

QSize myconfig::getDiagramButtonMinSize()
{
	static QSize size = QSize(m_userjson["diagrambutton"].toObject()["minwidth"].toInt()
		, m_userjson["diagrambutton"].toObject()["minheight"].toInt());
	return size;
}

QString myconfig::getMimetype()
{
	static QString mimetype = m_userjson["mimetype"].toString();
	return mimetype;
}

QColor myconfig::getCanvasBackgroundColor()
{
	static QColor color = QColor(m_userjson["canvas"].toObject()["backgroundcolor"].toString());
	return color;
}

int myconfig::getClickTolerance()
{
	static int clickTolerance = m_devjson["other"].toObject()["clicktolerance"].toInt();
	return clickTolerance;
}

QString myconfig::getCanvasScaleName()
{
	static QString name = m_devjson["name"].toObject()["other"].toObject()["canvasscalename"].toString();
	return name;
}

QString myconfig::getPenStyleName()
{
	static QString name = m_devjson["name"].toObject()["other"].toObject()["penstylename"].toString();
	return name;
}

QString myconfig::getPenStyle()
{
	static QString style = m_userjson["painter"].toObject()["pen"].toObject()["style"].toString();
	return style;
}

QString myconfig::getDrawParamsSetName()
{
	static QString name = m_devjson["other"].toObject()["drawparamssetname"].toString();
	return name;
}

QString myconfig::getOtherSetName()
{
	static QString name = m_devjson["other"].toObject()["othersetname"].toString();
	return name;
}

double myconfig::getScaleMax()
{
	static double max = m_devjson["other"].toObject()["scalemax"].toDouble();
	return max;
}

int myconfig::getFontSizeMax()
{
	static int max = m_devjson["other"].toObject()["fontsizemax"].toInt();
	return max;
}

QString myconfig::getPentagonName()
{
	static QString name = m_devjson["name"].toObject()["diagram"].toObject()["pentagon"].toString();
	return name;
}

QString myconfig::getHexagonName()
{
	static QString name = m_devjson["name"].toObject()["diagram"].toObject()["hexagon"].toString();
	return name;
}

QString myconfig::getStarName()
{
	static QString name = m_devjson["name"].toObject()["diagram"].toObject()["star"].toString();
	return name;
}

int myconfig::getPentagonRotate()
{
	static int rotate = m_userjson["diagram"].toObject()["pentagon"].toObject()["rotate"].toInt();
	return rotate;
}

int myconfig::getHexagonRotate()
{
	static int rotate = m_userjson["diagram"].toObject()["hexagon"].toObject()["rotate"].toInt();
	return rotate;
}

int myconfig::getStarRotate()
{
	static int rotate = m_userjson["diagram"].toObject()["star"].toObject()["rotate"].toInt();
	return rotate;
}

double myconfig::getPentagonScale()
{
	static double scale = m_userjson["diagram"].toObject()["pentagon"].toObject()["scale"].toDouble();
	return scale;
}

double myconfig::getHexagonScale()
{
	static double scale = m_userjson["diagram"].toObject()["hexagon"].toObject()["scale"].toDouble();
	return scale;
}

double myconfig::getStarScale()
{
	static double scale = m_userjson["diagram"].toObject()["star"].toObject()["scale"].toDouble();
	return scale;
}

QString myconfig::getTooltipInterfaceUrl()
{
	static QString url = m_devjson["other"].toObject()["tooltipurl"].toString();
	return url;
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
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsondata);
	configfile.close();
	if (!jsonDoc.isObject())
		throw std::runtime_error("error");

	m_devjson = jsonDoc.object();
}


