#pragma once
#include <QJsonObject>
#include <QString>
#include <qsize.h>

class myconfig
{
public:

	static myconfig& getInstance();
	void loadUserJson(QString filepath);

	QString getRectName();
	QString getCircleName();
	QString getTriangleName();
	QString getLineName();
	QString getMouseName();
	QString getChooseName();
	QString getTextName();

	QString getCanvasWidthName();
	QString getCanvasHeightName();
	QString getCanvasName();

	QString getNameName();
	QString getCenterHOffsetName();
	QString getCenterVOffsetName();
	QString getSpaceHeightName();
	QString getSpaceWidthName();
	QString getPenColorName();
	QString getPenWdithName();
	QString getBrushColorName();
	QString getRotateAngleName();
	QString getScaleName();
	QString getRectRadioName();
	QString getCircleRadioName();
	QString getTriangleRadioName();
	QString getTriangleRadioBottomName();
	QString getTriangleRadioLeftName();
	QString getTriangleRadioRightName();
	QString getEdgeTypeName();
	QString getFontFamilyName();
	QString getFontSizeName();
	double getRectRadio();
	int getRectRotate();
	double getRectScale();
	double getCircleRadio();
	int getCircleRotate();
	double getCircleScale();
	int getTriangleBottomRadio();
	int getTriangleLeftRadio();
    int getTriangleRightRadio();
	QString getTriangleEdgetype();
	int getTriangleRotate();
	int getTriangleScale();
	int getLineRotate();
	double getLineScale();
	QString getTextFamily();
	int getTextSize();
	QString getDefaultName();
	double getRectRadioMax();
	double getRectRadioMin();
	double getRectRadioStep();
	double getCircleRadioMax();
	double getCircleRadioMin();
	double getCircleRadioStep();
	int getTriangleRadioMax();
	QColor getPenColor();
	int getPenWidth();
	int getPenWidthMax();
	QColor getBrushColor();
	int getSpaceWidth();
	int getSpaceHeight();
	int getSpaceLengthMax();
	int getCanvasWidth();
	int getCanvasHeight();
	int getCanvasLengthMax();
	QColor getDiagramButtonBackgroundColor();
	bool getDiagramButtonIsSizeFixed();
	double getDiagramButtonWidgetRadio();
	QSize getDiagramButtonFixedSize();
	QSize getDiagramButtonMaxSize();
	QSize getDiagramButtonMinSize();
	QString getMimetype();
	QColor getCanvasBackgroundColor();
	int getClickTolerance();
	QString getCanvasScaleName();
	QString getPenStyleName();
	QString getPenStyle();
	QString getDrawParamsSetName();
	QString getOtherSetName();


		
private:
	myconfig();
	myconfig(const myconfig&);

	void loadDevJson();


	QJsonObject m_userjson;
	QJsonObject m_devjson;
};


QString PenStyleToQstring(Qt::PenStyle style);
Qt::PenStyle QstringToPenStyle(QString style);
