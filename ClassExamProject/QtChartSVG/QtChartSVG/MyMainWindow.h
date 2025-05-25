#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <qmainwindow.h>
#include <qscrollarea.h>
#include <qnetworkreply.h>
#include <qsettings.h>

class canvas;
class PropertyWidgetManager; 
class MyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MyMainWindow(QWidget *parent = nullptr);
	~MyMainWindow();

protected:
	void resizeEvent(QResizeEvent* event) override;
	void showEvent(QShowEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

private:
	void init();
	void initconfig(QString filepath = "userconfig.json");

	void setupMainLayout();
	void setupToolbar();
	void setupAction();
	void fetchAndSetTooltips();

	void savesvg();
	void loadfile();
	void savepng();
	void newcanvas();
	void copyDiagram();
	void pasteDiagram();
	void selectAllDiagram();
	void deleteDiagram();

	void dealNetworkReply(QNetworkReply* reply);

	canvas* m_huabuwidget;
	QWidget* m_huabuparentwidget;
	QScrollArea* m_huabuparentscroll;

	PropertyWidgetManager* m_propertyWidgetManager;
	QSettings m_settings;
	QMap<QString, QWidget*> m_tooltipsWidgets;
};

#endif // MYMAINWINDOW_H
