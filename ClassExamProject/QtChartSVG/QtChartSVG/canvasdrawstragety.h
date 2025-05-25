#pragma once
#ifndef CANVASDRAWSTRAGETY_H 
#define CANVASDRAWSTRAGETY_H

#include <qpoint.h>
#include <memory>
#include <diagramdrawparams.h>

class drawStragety
{
public:
	virtual void handlePress(QPoint point) = 0;  //画布上画画时处理press
	virtual void handleMove(QPoint point) = 0;  //画布上画画时处理move
	virtual void handleRelease() = 0;           //画布上画画时处理release

protected:
	QPoint m_startpoint;
	QPoint m_endpoint;
};

class shapeDrawStragety : public drawStragety
{
public:
	shapeDrawStragety(std::shared_ptr<IDidgramDrawParams> params);
	void handlePress(QPoint point) override;
	void handleMove(QPoint point) override;
	void handleRelease() override;
private:
	std::shared_ptr<IDidgramDrawParams> m_params;
};

class mouseDrawStragety : public drawStragety   //处理鼠标画线
{
public:
	mouseDrawStragety(std::shared_ptr<IDidgramDrawParams> params);
	void handlePress(QPoint point) override;
	void handleMove(QPoint point) override;
	void handleRelease() override;

private:
	std::shared_ptr<DiagramDrawParamsMouse> m_params;
	std::shared_ptr<QPainterPath> m_path;
};

class TextDrawStragety : public drawStragety   //处理文本绘制
{
public:
	TextDrawStragety(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent);
	void handlePress(QPoint point) override;
	void handleMove(QPoint point) override;
	void handleRelease() override;

private:
	std::shared_ptr<DiagramDrawParamsText> m_params;
	QWidget* m_parent;

};
#endif //CANVASDRAWSTRAGETY_H
