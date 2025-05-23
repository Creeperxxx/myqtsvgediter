#include "drawresult.h"
#include "myconfig.h"

bool DrawResultRect::iscontainPoint(QPointF point)
{
	return m_rect.containsPoint(point, Qt::OddEvenFill);
}

QPainterPath DrawResultRect::getPainterPath()
{
	QPainterPath path;
	path.addPolygon(m_rect);
	path.closeSubpath();
	return path;
}

QRect DrawResultRect::getBoundingRect()
{
	auto rect = m_rect.boundingRect().toRect();
	rect.adjust(-10, -10, 10, 10);
	return rect;
}

bool DrawResultCircle::iscontainPoint(QPointF point)
{
	return m_circle.containsPoint(point, Qt::OddEvenFill);
}

QPainterPath DrawResultCircle::getPainterPath()
{
	QPainterPath path;
	path.addPolygon(m_circle);
	path.closeSubpath();
	return path;
}

QRect DrawResultCircle::getBoundingRect()
{
	auto rect = m_circle.boundingRect().toRect();
	rect.adjust(-10, -10, 10, 10);
	return rect;
}

bool DrawResultTriangle::iscontainPoint(QPointF point)
{
	return m_triangle.containsPoint(point, Qt::OddEvenFill);
}

QPainterPath DrawResultTriangle::getPainterPath()
{
	QPainterPath path;
	path.addPolygon(m_triangle);
	path.closeSubpath();
	return path;
}

QRect DrawResultTriangle::getBoundingRect()
{
	auto rect = m_triangle.boundingRect().toRect();
	rect.adjust(-10, -10, 10, 10);
	return rect;
}

bool DrawResultLine::iscontainPoint(QPointF point)
{
	qreal distance = distanceToLine(m_line, point);
	return distance <= myconfig::getInstance().getClickTolerance();
}


QPainterPath DrawResultLine::getPainterPath()
{
	QPainterPath path;
	path.moveTo(m_line.p1());
	path.lineTo(m_line.p2());

	QPainterPathStroker stroker;
	stroker.setWidth(myconfig::getInstance().getClickTolerance());
	return stroker.createStroke(path);
}


QRect DrawResultLine::getBoundingRect()
{
	QSize size = QSize(m_line.p2().x() - m_line.p1().x()
		, m_line.p2().y() - m_line.p1().y());
	QRect rect = QRect(m_line.p1().toPoint(), size);
	rect.adjust(-10, -10, 10, 10);
	return rect;
}

qreal DrawResultLine::distanceToLine(const QLineF& line, const QPointF& point)
{
	// 向量AP (从line.p1()到point)
	QPointF AP = point - line.p1();
	// 向量AB (线段方向向量)
	QPointF AB = line.p2() - line.p1();

	// 计算AB的长度平方
	qreal ab2 = AB.x() * AB.x() + AB.y() * AB.y();
	// 计算AP与AB的点积
	qreal ap_ab = AP.x() * AB.x() + AP.y() * AB.y();

	// 计算归一化参数t (点在AB上的投影位置)
	qreal t = ap_ab / ab2;

	// 如果t在[0,1]之外，计算到端点的距离
	if (t < 0.0) {
		return QLineF(line.p1(), point).length();
	}
	else if (t > 1.0) {
		return QLineF(line.p2(), point).length();
	}

	// 计算投影点
	QPointF projection = line.p1() + t * AB;
	// 返回点到投影点的距离
	return QLineF(point, projection).length();

}

bool DrawResultMouse::iscontainPoint(QPointF point)
{
	QPainterPath circle;
	int tolerance = myconfig::getInstance().getClickTolerance();
	circle.addEllipse(point, tolerance, tolerance);

	return m_path.intersects(circle);
}

QPainterPath DrawResultMouse::getPainterPath()
{
	QPainterPath path(m_path);
	path.closeSubpath();

	return path;
}

QRect DrawResultMouse::getBoundingRect()
{
	QRect rect = m_path.boundingRect().toRect();
	rect.adjust(-10, -10, 10, 10);
	return rect;
}

QPainterPath DrawResultText::getPainterPath()
{
	QPainterPath path;
	path.addRect(m_rect);

	return path;
}

bool DrawResultText::iscontainPoint(QPointF point)
{
	return m_rect.contains(point);
}

QRect DrawResultText::getBoundingRect()
{
	QRect rect = m_rect.toRect();
	rect.adjust(-10, -10, 10, 10);
	return rect;
}
