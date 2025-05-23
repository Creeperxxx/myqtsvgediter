#pragma once
#include "canvasdrawstragety.h"
#include <qpainterpath.h>

shapeDrawStragety::shapeDrawStragety(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(params)
{

}

void shapeDrawStragety::handlePress(QPoint point)
{
	m_startpoint = point;
	m_params->setCenter(point);
}

void shapeDrawStragety::handleMove(QPoint point)
{
	m_endpoint = point;

	m_params->setCenter((m_startpoint + m_endpoint) / 2);
	m_params->setSpacesize(QSize(std::max(1, std::abs(m_startpoint.x() - m_endpoint.x()))
		, std::max(1, std::abs(m_startpoint.y() - m_endpoint.y()))));
}

void shapeDrawStragety::handleRelease()
{

}

mouseDrawStragety::mouseDrawStragety(std::shared_ptr<IDidgramDrawParams> params)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsMouse>(params);
	if (p == nullptr)
		throw std::runtime_error("error");
	m_params = p;
}

void mouseDrawStragety::handlePress(QPoint point)
{
	m_startpoint = point;
	m_path = std::make_shared<QPainterPath>();
	m_path->moveTo(m_startpoint);
	m_params->setPath(m_path);
}

void mouseDrawStragety::handleMove(QPoint point)
{
	m_endpoint = point;
	m_path->lineTo(m_endpoint);
}

void mouseDrawStragety::handleRelease()
{

}

TextDrawStragety::TextDrawStragety(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent)
	:m_params(nullptr)
	, m_parent(parent)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsText>(params);
	if (p == nullptr)
		throw std::runtime_error("error");
	m_params = p;
}

void TextDrawStragety::handlePress(QPoint point)
{
	m_startpoint = point;
	m_params->setCenter(m_startpoint);

	TextLineEdit::createTextLineEdit(m_params, m_parent);
}

void TextDrawStragety::handleMove(QPoint point)
{
	m_endpoint = point;

	m_params->setCenter((m_startpoint + m_endpoint) / 2);
	m_params->setSpacesize(QSize(std::max(1, std::abs(m_startpoint.x() - m_endpoint.x()))
		, std::max(1, std::abs(m_startpoint.y() - m_endpoint.y()))));
}

void TextDrawStragety::handleRelease()
{
	m_params->getTextEdit()->show();
}
