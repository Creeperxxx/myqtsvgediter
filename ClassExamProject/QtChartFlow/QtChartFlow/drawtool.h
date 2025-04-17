#pragma once
#include <memory>
#include <qpoint.h>
#include <qbrush.h>
#include <qpen.h>
#include <stdexcept>
//#include "huabu.h"
//#include "tuxing.h"
//#include "huabu.h"


//class DiagramItem;

enum class ShapeType
{
	Rect,
	Circle,
	Triangle,
	Line
};

//enum class DiagramItemType
//{
//	tuxingku,
//	tuxingkupixmap,
//	huabu
//};

//class Tool
//{
//public:
//	static ShapeType shapetypestringtoenum(const std::string& str);
//};


class IDidgramDrawParams
{
public:
	virtual ~IDidgramDrawParams() {}
	QPointF m_center;
	QSizeF m_spacesize;
	ShapeType m_type;
	QPen m_pen;
	QBrush m_brush;
	//QPainter* m_painter;
};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsRect() {}
	double m_boundingrectradio;
};

class DiagramDrawParamsCircle :public IDidgramDrawParams
{
public:
	double m_boundingrectradio;
	~DiagramDrawParamsCircle() {}
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
	static EdgeType edgetypestringtoenum(const std::string& edgetype);
	~DiagramDrawParamsTriangle() {}
	class TriangleSizeRadios
	{
	public:
		TriangleSizeRadios();
		TriangleSizeRadios(double bottom, double left, double right);
		double m_bottom;
		double m_left;
		double m_right;
	};
	TriangleSizeRadios m_triangleSizeRadios;
	EdgeType m_edgetype;
	qreal m_rotationAngle;
};

class DiagramDrawParamsLine : public IDidgramDrawParams
{
public:
	double m_rotationAngle;
};

//
//
//
//class ParamsforbuildParams
//{
//
//};
//
//
//
//
//class IbuildAllDiagramParams
//{
//public:
//	virtual std::shared_ptr<IDidgramDrawParams> build(QWidget* item) = 0;
//};
//
//
//
//
//class DiagramItem;
//
//
//
//class Ibuildtuxingjiedianparamsfordiagram : public IbuildAllDiagramParams
//{
//public:
//	std::shared_ptr<IDidgramDrawParams> build(QWidget* item) override;
//private:
//	virtual std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) = 0;
//};
//
//class buildtuxingjiedianparamsfrodiagramrect : public Ibuildtuxingjiedianparamsfordiagram
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//};
//
//class buildtuxingjiedianparamsfordiagramcircle : public Ibuildtuxingjiedianparamsfordiagram
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//};
//
//class buildtuxingjiedianparamsfordiagramtriangle : public Ibuildtuxingjiedianparamsfordiagram
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//};
//
//class buildtuxingjiedianparamsfordiagramline : public Ibuildtuxingjiedianparamsfordiagram
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//};
//
//
//
//
//
//class factorybuildtuxingjiedianparamsfordiagram
//{
//public:
//	static std::unique_ptr<Ibuildtuxingjiedianparamsfordiagram> create(ShapeType type);
//};
//
//
//
//
//
//
//class IbuildDiagramPixmapParams : public IbuildAllDiagramParams
//{
//public:
//	std::shared_ptr<IDidgramDrawParams> build(QWidget* item) override;
//private:
//	virtual std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) = 0;
//};
//
//class buildDiagramPixmapParamsRect : public IbuildDiagramPixmapParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//};
//
//class buildDiagramPixmapParamsCircle :public IbuildDiagramPixmapParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//};
//
//class buildDiagramPixmapParamsTriangle : public IbuildDiagramPixmapParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//};
//
//class buildDiagramPixmapParamsLine : public IbuildDiagramPixmapParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
//
//};
//
//
//
//enum class ShapeType;
//enum class DiagramItemType;
//
//
//
//class FactoryBuildDiagramPixmapParams
//{
//public:
//	static std::unique_ptr<IbuildDiagramPixmapParams> create(ShapeType type);
//};
//
//
////class FactoryBuildDiagramParams
////{
////	static std::unique_ptr<IbuildAllDiagramParams> create(DiagramItemType type);
////};
//
//class huabu;
//
//class IbuildHuabuParams : public IbuildAllDiagramParams
//{
//public:
//	std::shared_ptr<IDidgramDrawParams> build(QWidget* item) override;
//private:
//	virtual std::shared_ptr<IDidgramDrawParams> specialbuild(huabu* item) = 0;
//};
//
//class buildHuabuParamsRect : public IbuildHuabuParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(huabu* item) override;
//};
//
//class buildHuabuParamsCircle : public IbuildHuabuParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(huabu* item) override;
//};
//
//class buildHuabuParamsTriangle : public IbuildHuabuParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(huabu* item) override;
//};
//
//class buildHuabuParamsLine : public IbuildHuabuParams
//{
//private:
//	std::shared_ptr<IDidgramDrawParams> specialbuild(huabu* item) override;
//
//};
//
//class FactorybuildHuabuParams
//{
//public:
//	static std::unique_ptr<IbuildHuabuParams> create(ShapeType type);
//};
//
//
//class factoryall
//{
//public:
//	static std::unique_ptr<IbuildAllDiagramParams> create(DiagramItemType widgettype, ShapeType shapetype);
//};











