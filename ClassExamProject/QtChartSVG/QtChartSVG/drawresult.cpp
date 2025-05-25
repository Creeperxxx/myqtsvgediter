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

void DrawResultRect::setRect(QPolygonF rect)
{
	m_rect = rect;
}

QPolygonF DrawResultRect::getRect()
{
	return m_rect;
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

void DrawResultCircle::setCircle(QPolygonF circle)
{
	m_circle = circle;
}

QPolygonF DrawResultCircle::getCircle()
{
	return m_circle;
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

void DrawResultTriangle::setTriangle(QPolygonF triangle)
{
	m_triangle = triangle;
}

QPolygonF DrawResultTriangle::getTriangle()
{
	return m_triangle;
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

void DrawResultLine::setLine(QLineF line)
{
	m_line = line;
}

QLineF DrawResultLine::getLine()
{
	return m_line;
}

qreal DrawResultLine::distanceToLine(const QLineF& line, const QPointF& point)
{
	QPointF AP = point - line.p1();
	QPointF AB = line.p2() - line.p1();

	qreal ab2 = AB.x() * AB.x() + AB.y() * AB.y();
	qreal ap_ab = AP.x() * AB.x() + AP.y() * AB.y();

	qreal t = ap_ab / ab2;

	if (t < 0.0) {
		return QLineF(line.p1(), point).length();
	}
	else if (t > 1.0) {
		return QLineF(line.p2(), point).length();
	}

	QPointF projection = line.p1() + t * AB;
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

void DrawResultMouse::setPath(QPainterPath path)
{
	m_path = path;
}

QPainterPath DrawResultMouse::getPath()
{
	return m_path;
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

void DrawResultText::setText(QString text)
{
	m_text = text;
}

QString DrawResultText::getText()
{
	return m_text;
}

void DrawResultText::setFont(QFont font)
{
	m_font = font;
}

QFont DrawResultText::setFont()
{
	return m_font;
}

void DrawResultText::setRect(QRectF rect)
{
	m_rect = rect;
}

QRectF DrawResultText::getRect()
{
	return m_rect;
}

void DrawResult::setPen(QPen pen)
{
	m_painterpen = pen;
}

QPen DrawResult::getPen()
{
	return m_painterpen;
}

void DrawResult::setBrush(QBrush brush)
{
	m_painterbrush = brush;
}

QBrush DrawResult::getBrush()
{
	return m_painterbrush;
}
