#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>
#include <qdebug.h>
#include <stdexcept>
#include <qpainterpath.h>
#include <unordered_map>
#include <functional>
#include <qlineedit.h>
#include "drawtool.h"
#include "configmanager.h"
#include "drawparamitem.h"

constexpr const double linetolerance = 5.0;

class DrawResult
{

public:
	virtual bool iscontainPoint(QPointF point) = 0;
	virtual QPainterPath getPainterPath() = 0;


	QPen m_painterpen;
	QBrush m_painterbrush;
};

class DrawResultRect : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;

	QPolygonF m_rect;
};

class DrawResultCircle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QPolygonF m_circle;
};

class DrawResultTriangle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point)override;
	QPainterPath getPainterPath() override;

	QPolygonF m_triangle;
};

class DrawResultLine : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QLineF m_line;
private:
	qreal distanceToLine(const QLineF& line, const QPointF& point);
};

class DrawResultMouse : public DrawResult
{
public:
	bool iscontainPoint(QPointF point)override;
	QPainterPath getPainterPath()override;

	QPainterPath m_path;
};

class DrawResultText : public DrawResult
{
public:

    virtual QPainterPath getPainterPath()override;
    virtual bool iscontainPoint(QPointF point)override;
    QString m_text;
    QFont m_font;
	QRectF m_rect;
};






















class IDiagramDrawer
{
public:

	virtual void build() = 0;
	virtual void draw(QPainter& painter) = 0;
	virtual std::shared_ptr<DrawResult> getResult() = 0;

protected:
	QPoint calcuRealCenter(QPoint center, int hmove, int vmove);
};

class DiagramDrawerRect : public IDiagramDrawer
{
public:
	DiagramDrawerRect(std::shared_ptr<IDidgramDrawParams> param);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;



private:
	QPolygonF calcuRect();
	QPolygonF calcuBasicalRect();
	QTransform calcuRotateTransform(QPointF center);
	QTransform calcuTranslateTransform(QPointF center);
	QTransform calcuScaleTransform(QRectF rect);

	std::shared_ptr<DiagramDrawParamsRect> m_params;
	std::shared_ptr<DrawResultRect> m_result;
	//bool m_isbuild;
	QPolygonF m_rect;
};

class DiagramDrawerCircle : public IDiagramDrawer
{
public:
	DiagramDrawerCircle(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;
private:
	QPolygonF calcuBasicalCircle();
	QTransform calcurotatetransform(QPointF center);
	qreal calcuscaleFactor(QRectF bound);
	QTransform calcuscaleTransform(QPointF center);
	QTransform calcuTranslateTransform(QPointF mycenter, QPointF targetpoint);

	std::shared_ptr<DiagramDrawParamsCircle> m_params;
	QPolygonF m_circle;
	qreal m_scale;
	const int m_initheight;
	std::shared_ptr<DrawResultCircle> m_result;
};

class DiagramDrawerTriangle : public IDiagramDrawer
{
public:
	DiagramDrawerTriangle(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
    std::shared_ptr<DrawResult> getResult() override;
private:
	QPolygonF calcuTriangle();
	QPolygonF calcuBasicalTriangle();
	QTransform calcuRotateTransform();
	QTransform calcuTranslateTransfrom(QPointF trianglecenter);
	QTransform calcuScaleTransform(QRectF trianglerect);

	std::shared_ptr<DiagramDrawParamsTriangle> m_params;
	std::shared_ptr<DrawResultTriangle> m_result;
	QPolygonF m_triangle;
	
};

class DiagramDrawerLine : public IDiagramDrawer
{
public:
	DiagramDrawerLine(std::shared_ptr<IDidgramDrawParams> params);
	void build()override;
	void draw(QPainter& painter)override;
    std::shared_ptr<DrawResult> getResult()override;
private:
	QLineF calcuLine();
	
	std::shared_ptr<DiagramDrawParamsLine> m_params;
	std::shared_ptr<DrawResultLine> m_result;
	QLineF m_line;
};

class DiagramDrawerMouse : public IDiagramDrawer
{
public:
	DiagramDrawerMouse(std::shared_ptr<IDidgramDrawParams> params);
	void build()override;
	void draw(QPainter& painter)override;
	std::shared_ptr<DrawResult> getResult()override;


	std::shared_ptr<DiagramDrawParamsMouse> m_params;
	std::shared_ptr<QPainterPath> m_path;
	std::shared_ptr<DrawResultMouse> m_result;
};

class TransparentEditableLabel : public QLineEdit {
	Q_OBJECT
public:
	explicit TransparentEditableLabel(QWidget* parent = nullptr)
		: QLineEdit(parent), m_isEditing(false) {

		// 初始设置为透明样式
		setStyleSheet("QLineEdit {"
			"background: transparent;"
			"border: 1px solid transparent;"
			"}");

		// 设置文本对齐方式
		setAlignment(Qt::AlignCenter);

		// 连接信号
		connect(this, &QLineEdit::textEdited, this, &TransparentEditableLabel::onTextEdited);
	}

protected:
	void focusInEvent(QFocusEvent* event) override {
		m_isEditing = true;
		// 进入编辑模式时设置不透明背景
		setStyleSheet("QLineEdit {"
			"background: white;"
			"border: 1px solid gray;"
			"}");
		QLineEdit::focusInEvent(event);
	}

	void focusOutEvent(QFocusEvent* event) override {
		m_isEditing = false;
		// 退出编辑模式时恢复透明背景
		setStyleSheet("QLineEdit {"
			"background: transparent;"
			"border: 1px solid transparent;"
			"}");
		QLineEdit::focusOutEvent(event);

		emit editingFinished();
	}

	void mouseDoubleClickEvent(QMouseEvent* event) override {
		// 双击进入编辑模式
		setFocus();
		selectAll();
		QLineEdit::mouseDoubleClickEvent(event);
	}

	void paintEvent(QPaintEvent* event) override {
		QLineEdit::paintEvent(event);

		// 非编辑状态下绘制额外效果（可选）
		if (!m_isEditing && !hasFocus()) {
			QPainter painter(this);
			QPen pen(Qt::lightGray, 1, Qt::DashLine);
			painter.setPen(pen);
			painter.drawRect(rect().adjusted(0, 0, -1, -1));
		}
	}

private slots:
	void onTextEdited(const QString& text) {
		// 文本编辑时的处理
		emit textChanged(text);
	}

private:
	bool m_isEditing;

signals:
	void textChanged(const QString& newText);
signals:
	void editingFinished();
};

class DiagramDrawerText : public IDiagramDrawer
{
public:
	DiagramDrawerText(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter);
	std::shared_ptr<DrawResult> getResult() override;

	std::shared_ptr<DiagramDrawParamsText> m_params;
	TransparentEditableLabel* m_lineedit;
	std::shared_ptr<DrawResultText> m_result;
};

class DiagramDrawerChoose : public IDiagramDrawer
{
public:
	DiagramDrawerChoose(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;


	std::shared_ptr<DiagramDrawParamsChoose> m_params;
};


















class DiagramDrawInterface
{
public:					                       
	static DiagramDrawInterface& getInstance();
	DiagramDrawInterface& addDrawerCreator(ShapeType type, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)> drawer);
	std::shared_ptr<IDiagramDrawer> getDrawer(std::shared_ptr<IDidgramDrawParams> params);
private:
	std::unordered_map<ShapeType, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)>> m_drawerMap;
};

