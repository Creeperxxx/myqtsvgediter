#include "canvasdiagram.h"

canvasDiagram::canvasDiagram()
	:m_propertySetManager(nullptr)
	, m_drawer(nullptr)
	, m_params(nullptr)
{

}

std::shared_ptr<propertySetManager> canvasDiagram::getSetManager()
{
	return m_propertySetManager;
}

void canvasDiagram::setSetManager(std::shared_ptr<propertySetManager> setmanager)
{
	m_propertySetManager = setmanager;
}

void canvasDiagram::setDrawer(std::shared_ptr<IDiagramDrawer> drawer)
{
	m_drawer = drawer;
}

std::shared_ptr<IDiagramDrawer> canvasDiagram::getDrawer()
{
	return m_drawer;
}

void canvasDiagram::setParams(std::shared_ptr<IDidgramDrawParams> params)
{
	m_params = params;
}

std::shared_ptr<IDidgramDrawParams> canvasDiagram::getParams()
{
	return m_params;
}