#ifndef CANVASDIAGRAM_H
#define CANVASDIAGRAM_H

#include <qobject.h>
#include "propertyset.h"
#include "diagramdrawparams.h"
#include "drawer.h"

class canvasDiagram : public QObject
{
	Q_OBJECT
signals:
	void signalRepaint(QRect rect);
public:
	canvasDiagram();

	std::shared_ptr<propertySetManager> getSetManager();
	void setSetManager(std::shared_ptr<propertySetManager> setmanager);
	void setDrawer(std::shared_ptr<IDiagramDrawer> drawer);
	std::shared_ptr<IDiagramDrawer> getDrawer();
	void setParams(std::shared_ptr<IDidgramDrawParams> params);
	std::shared_ptr<IDidgramDrawParams> getParams();

	void onDealValueChanged();

private:
	std::shared_ptr<propertySetManager> m_propertySetManager;
	std::shared_ptr<IDiagramDrawer> m_drawer;
	std::shared_ptr<IDidgramDrawParams> m_params;
};
#endif // CANVASDIAGRAM_H
