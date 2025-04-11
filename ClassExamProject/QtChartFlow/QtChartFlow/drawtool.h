#pragma once
#include <memory>
#include <qpoint.h>
#include <qbrush.h>
#include <qpen.h>
//#include "tuxing.h"
//#include "huabu.h"


//class DiagramItem;

enum class ShapeType
{
	Rect,
	Circle,
	Triangle,
	RightTriangle
};

enum class DiagramItemType
{
	tuxingku,
	tuxingkupixmap,
	huabu
};

class Tool
{
public:
	static ShapeType shapetypestringtoenum(const std::string& str);
};


class IDidgramDrawParams
{
public:
	virtual ~IDidgramDrawParams() {}
	QPoint m_center;
	QSize m_spacesize;
	ShapeType m_type;
	QPen m_pen;
	QBrush m_brush;
	//QPainter* m_painter;
};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsRect() {}
	float m_radio;
};

class DiagramDrawParamsCircle :public IDidgramDrawParams
{
public:
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
	~DiagramDrawParamsTriangle() {}
	struct TriangleSizeRadios
	{
		double m_bottom;
		double m_left;
		double m_right;
	};
	TriangleSizeRadios m_triangleSizeRadios;
	EdgeType m_edgetype;
	qreal m_rotationAngle;
};








class IbuildAllDiagramParams
{
public:
	virtual std::shared_ptr<IDidgramDrawParams> build(QWidget* item) = 0;
};




class DiagramItem;



class Ibuildtuxingjiedianparamsfordiagram : public IbuildAllDiagramParams
{
public:
	std::shared_ptr<IDidgramDrawParams> build(QWidget* item) override;
private:
	virtual std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) = 0;
};

class buildtuxingjiedianparamsfrodiagramrect : public Ibuildtuxingjiedianparamsfordiagram
{
private:
	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
};

class buildtuxingjiedianparamsfordiagramcircle : public Ibuildtuxingjiedianparamsfordiagram
{
private:
	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
};

class buildtuxingjiedianparamsfordiagramtriangle : public Ibuildtuxingjiedianparamsfordiagram
{
private:
	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
};






class factorybuildtuxingjiedianparamsfordiagram
{
public:
	static std::unique_ptr<Ibuildtuxingjiedianparamsfordiagram> create(ShapeType type);
};






class IbuildDiagramPixmapParams : public IbuildAllDiagramParams
{
public:
	std::shared_ptr<IDidgramDrawParams> build(QWidget* item) override;
private:
	virtual std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) = 0;
};

class buildDiagramPixmapParamsRect : public IbuildDiagramPixmapParams
{
private:
	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
};

class buildDiagramPixmapParamsCircle :public IbuildDiagramPixmapParams
{
private:
	std::shared_ptr<IDidgramDrawParams> specialbuild(DiagramItem* item) override;
};



enum class ShapeType;
enum class DiagramItemType;



class FactoryBuildDiagramPixmapParams
{
public:
	static std::unique_ptr<IbuildDiagramPixmapParams> create(ShapeType type);
};


//class FactoryBuildDiagramParams
//{
//	static std::unique_ptr<IbuildAllDiagramParams> create(DiagramItemType type);
//};

class factoryall
{
public:
	static std::unique_ptr<IbuildAllDiagramParams> create(DiagramItemType widgettype, ShapeType shapetype);
};











