#pragma once
#include <QJsonObject>
#include <QString>

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
	QString getEdgeTypeName();
	QString getFontFamilyName();
	QString getFontSizeName();
		
private:
	myconfig();
	myconfig(const myconfig&);

	void loadDevJson();


	QJsonObject m_userjson;
	QJsonObject m_devjson;
};
