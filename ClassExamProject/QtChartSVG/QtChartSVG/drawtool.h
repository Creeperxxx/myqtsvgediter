#pragma once
#include <memory>
#include <qpoint.h>
#include <qbrush.h>
#include <qpen.h>
#include <stdexcept>
#include <qstring.h>
#include <shuxingwidget.h>
#include "configmanager.h"
#include "config.h"


constexpr auto rectname = "rect";
constexpr auto circlename = "circle";
constexpr auto trianglename = "triangle";
constexpr auto linename = "line";


enum class ShapeType
{
	Rect,
	Circle,
	Triangle,
	Line
};

namespace ShapeTypeTool
{
	QString shapetypeEnumToQstring(ShapeType type);
	ShapeType shapetypeQstringToEnum(const QString& type);
}

//旋转角度由double变int


class IDidgramDrawParams : QObject
{
	Q_OBJECT
public:
signals:
	void SignalParamsChanged();


public:

	virtual ~IDidgramDrawParams() {}

	friend QDataStream& operator<<(QDataStream& out, const IDidgramDrawParams& params);
	friend QDataStream& operator>>(QDataStream& in, IDidgramDrawParams& params);
	virtual void serialize(QDataStream& out) const;
	virtual void deserialize(QDataStream& in);


	QPointF m_center;
	QSizeF m_spacesize;
	ShapeType m_type;
	qreal m_scale;
	QPen m_pen;
	QBrush m_brush;
	qreal m_rotate;
	bool m_paramChanged = true;
	
};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsRect() {}
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;


	qreal m_boundingrectradio;
};

class DiagramDrawParamsCircle :public IDidgramDrawParams
{
public:
	qreal m_boundingrectradio;
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;

	~DiagramDrawParamsCircle() {}
};

class DiagramDrawParamsTriangle : public IDidgramDrawParams
{
public:
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;

	enum class EdgeType
	{
		Left,
		Right,
		Bottom
	};
	static EdgeType edgetypeStringToEnum(const QString& edgetype);
	static QString edgetypeEnumToString(EdgeType edgetype);
	~DiagramDrawParamsTriangle() {}
	class TriangleSizeRadios
	{
	public:
		TriangleSizeRadios();
		TriangleSizeRadios(qreal bottom, qreal left, qreal right);
		qreal m_bottom;
		qreal m_left;
		qreal m_right;
	};
	TriangleSizeRadios m_triangleSizeRadios;
	EdgeType m_edgetype;
};

class DiagramDrawParamsLine : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsLine() {}
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;
};

class ICreateParams
{
public:
	std::shared_ptr<IDidgramDrawParams> create();
protected:
	virtual std::shared_ptr<IDidgramDrawParams> createSpecial() = 0;
private:
	void createRest(std::shared_ptr<IDidgramDrawParams> params);
};

class createParamsRect : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsCircle : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsTriangle : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsLine : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsInterface
{
public:
	static createParamsInterface& getInstance();
	createParamsInterface& add(ShapeType type, std::function<std::shared_ptr<ICreateParams>()> creator);
	std::shared_ptr<ICreateParams> getParams(ShapeType type);
private:
	createParamsInterface();

	std::map<ShapeType, std::function<std::shared_ptr<ICreateParams>()>> m_map;

};






















class ICreatePropertyData
{
public:
	std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> createPropertyData(std::shared_ptr<IDidgramDrawParams> params);
protected:
	virtual void buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) = 0;;
private:
	void onRotateChanged(QVariant rotate, std::shared_ptr<IDidgramDrawParams> params);
	void onScaleChanged(QVariant scale, std::shared_ptr<IDidgramDrawParams> params);
	void onPencolorChanged(QVariant color, std::shared_ptr<IDidgramDrawParams> params);
	void onPenwidthChanged(QVariant width, std::shared_ptr<IDidgramDrawParams> params);
	void onBrushChanged(QVariant brush, std::shared_ptr<IDidgramDrawParams> params);
	void onSpacesizeWidthChanged(QVariant width, std::shared_ptr<IDidgramDrawParams> params);
	void onSpacesizeHeightChanged(QVariant height, std::shared_ptr<IDidgramDrawParams> params);

	void createPropertyDataAll(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);

	void buildPenAndBrush(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> data);
	void buildRotate(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
	void buildSpacesize(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
	void buildScale(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);

	
};

class CreatePropertyDataRect : public ICreatePropertyData
{
protected:
	void buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
private:
	void onRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsRect> params);
	void buildRadio(std::shared_ptr<DiagramDrawParamsRect> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
	void buildName(std::shared_ptr<DiagramDrawParamsRect> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
};

class CreatePropertyDataCircle : public ICreatePropertyData
{
protected:
	void buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
private:
	void onRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsCircle> params);
	void buildRadio(std::shared_ptr<DiagramDrawParamsCircle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
	void buildName(std::shared_ptr<DiagramDrawParamsCircle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
};

class CreatePropertyDataTriangle : public ICreatePropertyData
{
protected:
	void buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
private:
	void onEdgetypeChanged(QVariant edgetype, std::shared_ptr<DiagramDrawParamsTriangle> params);
	void onBottomRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsTriangle> params);
	void onLeftRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsTriangle> params);
	void onRightRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsTriangle> params);
	void buildEdgetype(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
	void buildBottomRadio(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
	void buildLeftRadio(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
    void buildRightRadio(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
	void buildName(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
};

class CreatePropertyDataLine : public ICreatePropertyData
{
protected:
	void buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
private:
	void buildName(std::shared_ptr<DiagramDrawParamsLine> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
};

class PropertyDataInterface
{
public:
	static PropertyDataInterface& getInstance();
	PropertyDataInterface& addCreator(ShapeType type, std::function<std::shared_ptr<ICreatePropertyData>()> creator);
	std::shared_ptr<ICreatePropertyData> getCreator(ShapeType type);
private:
	PropertyDataInterface();
	PropertyDataInterface(const PropertyDataInterface& other);


	std::map<ShapeType, std::function<std::shared_ptr<ICreatePropertyData>()>> m_creatorMap;
};




