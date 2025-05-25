#ifndef DIAGRAMDRAWPARAMS_H 
#define DIAGRAMDRAWPARAMS_H

#include <qstring.h>
#include <qobject.h>
#include <qsize.h>
#include <qpen.h>
#include <qbrush.h>
#include <qlineedit.h>
#include <qdatastream.h>
#include "namespace.h"

class IDidgramDrawParams : QObject            //绘制图形所需参数
{
	Q_OBJECT
signals:
	void SignalParamsChanged();

public:
	virtual std::shared_ptr<IDidgramDrawParams> clone() = 0;
	IDidgramDrawParams(const IDidgramDrawParams& other);
	IDidgramDrawParams();

	friend QDataStream& operator<<(QDataStream& out, const IDidgramDrawParams& params);
	friend QDataStream& operator>>(QDataStream& in, IDidgramDrawParams& params);
	virtual void serialize(QDataStream& out) const;
	virtual void deserialize(QDataStream& in);

	QPoint getCenter();
	void setCenter(QPoint center);
	QSize getSpacesize();
	void setSpacesize(QSize size);
	void setType(myqtsvg::ShapeType type);
	myqtsvg::ShapeType getType();
	void setScale(double scale);
	double getScale();
	void setPenColor(QColor color);
	QColor getPenColor();
	void setPenwidth(int width);
	int getPenwidth();
	void setPenStyle(Qt::PenStyle style);
	Qt::PenStyle getPenstyle();
	void setPen(QPen pen);
	QPen getPen();
	void setBrushColor(QColor color);
	QColor getBrushColor();
	void setRotate(int rotate);
	int getRotate();
	void setCenterHOffset(int offset);
	int getCenterHOffset();
	void setCenterVOffset(int offset);
	int getCenterVOffset();
	void setIsDrawInCanvas(bool flag);
	bool getIsDrawInCanvas();


private:
	QPoint m_center;       //绘制空间的中心
	QSize m_spacesize;     //绘制空间的size
	myqtsvg::ShapeType m_type;
	double m_scale;        //缩放
	QPen m_pen;
	QBrush m_brush;
	int m_rotate;          //旋转
	int m_centerHoffset;   //中心偏移
	int m_centerVoffset;
	bool m_isdrawInHuabu;  //是否在画布上作画

};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	DiagramDrawParamsRect(const DiagramDrawParamsRect& other);
	DiagramDrawParamsRect();
	std::shared_ptr<IDidgramDrawParams> clone() override;

	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	void setRadio(double radio);
	double getRadio();
private:

	double m_radio;
};

class DiagramDrawParamsCircle :public IDidgramDrawParams
{
public:
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;
	DiagramDrawParamsCircle(const DiagramDrawParamsCircle& other);
	DiagramDrawParamsCircle();

	void setRadio(double radio);
	double getRadio();
private:
	double m_radio;
};

class DiagramDrawParamsTriangle : public IDidgramDrawParams
{
public:
	enum class EdgeType
	{
		Left,
		Right,
		Bottom
	};

	class sideRadios
	{
	public:
		sideRadios();
		sideRadios(const sideRadios& other);
		sideRadios(int bottom, int left, int right);
		void setBottom(int bottom);
		int getBottom() const;
		void setLeft(int left);
		int getLeft() const;
		void setRight(int right);
		int getRight() const;
	private:
		int m_bottom;
		int m_left;
		int m_right;
	};

	DiagramDrawParamsTriangle(const DiagramDrawParamsTriangle& other);
	DiagramDrawParamsTriangle();

	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;
	std::shared_ptr<IDidgramDrawParams> clone() override;

	static EdgeType edgetypeStringToEnum(const QString& edgetype);
	static QString edgetypeEnumToString(EdgeType edgetype);

	void setBottomRadio(int bottom);
	void setLeftRadio(int left);
	void setRightRadio(int right);
	int getBottomRadio();
	int getLeftRadio();
	int getRightRadio();
	void setRadios(sideRadios radios);
	sideRadios getRadios();
	void setEdgeType(EdgeType type);
	EdgeType getEdgeType();

private:
	sideRadios m_sideRadios;
	EdgeType m_edgetype;

};

Q_DECLARE_METATYPE(DiagramDrawParamsTriangle::sideRadios)

class DiagramDrawParamsLine : public IDidgramDrawParams
{
public:
	DiagramDrawParamsLine(const DiagramDrawParamsLine& other);
	DiagramDrawParamsLine();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;
};

class DiagramDrawParamsMouse : public IDidgramDrawParams
{
public:
	DiagramDrawParamsMouse(const DiagramDrawParamsMouse& other);
	DiagramDrawParamsMouse();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;

	void setPath(std::shared_ptr<QPainterPath> path);
	std::shared_ptr<QPainterPath> getPaht();

private:
	std::shared_ptr<QPainterPath> m_path;
};

class DiagramDrawParamsChoose : public IDidgramDrawParams
{
public:
	DiagramDrawParamsChoose(const DiagramDrawParamsChoose& other);
	DiagramDrawParamsChoose();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;
};


class TextLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	TextLineEdit(QWidget* parent = nullptr);
	TextLineEdit(const TextLineEdit& other);
	void setTextColor(QColor textcolor);
	void setBackGroundColor(QColor color);

	void showEvent(QShowEvent* event)override;
	void setFont(const QFont& font);

	void focusOutEvent(QFocusEvent* event)override;

	void adjustsize();

	static void createTextLineEdit(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent = nullptr);
signals:
	void signalHasFocusOut();

private:
	QColor m_textcolor;
};

class DiagramDrawParamsText : public IDidgramDrawParams
{
public:
	DiagramDrawParamsText(const DiagramDrawParamsText& other);
	DiagramDrawParamsText();

	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;

	void setFontFamily(QString family);
	QString getFontFamily();
	void setFontSize(int size);
	int getFontSize();
	void setFont(QFont font);
	QFont getFont();
	void setTextEdit(TextLineEdit* edit);
	TextLineEdit* getTextEdit();

private:
	QFont m_font;
	TextLineEdit* m_textedit;
};

class DiagramDrawParamsPentagon : public IDidgramDrawParams
{
public:
	DiagramDrawParamsPentagon(const DiagramDrawParamsPentagon& params);
	DiagramDrawParamsPentagon();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;
};

class DiagramDrawParamsHexagon : public IDidgramDrawParams
{
public:
	DiagramDrawParamsHexagon(const DiagramDrawParamsHexagon& params);
	DiagramDrawParamsHexagon();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;
};

class DiagramDrawParamsStar : public IDidgramDrawParams
{
public:
	DiagramDrawParamsStar(const DiagramDrawParamsHexagon& params);
	DiagramDrawParamsStar();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	std::shared_ptr<IDidgramDrawParams> clone() override;
};










#endif //DIAGRAMDRAWPARAMS_H
