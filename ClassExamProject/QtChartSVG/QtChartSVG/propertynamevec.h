#ifndef PROPERTYNAMEVEC_H
#define PROPERTYNAMEVEC_H


#include <map>
#include "namespace.h"
#include <qstring.h>
#include <vector>
#include <memory>

class propertyNameVecInterface
{
public:
	static propertyNameVecInterface& getinstance();
	void addPropertyNameVec(myqtsvg::ShapeType type, std::vector<QString> vec);
	std::vector<QString> getPropertyNameVec(myqtsvg::ShapeType type, std::vector<QString> vec = {});

private:
	propertyNameVecInterface();
	void defaultinit();
	void setRectPropertyNameVec();
	void setTrianglePropertyNameVec();
	void setCirclePropertyNameVec();
	void setLinePropertyNameVec();
	void setChoosePropertyNameVec();
	void setMousePropertyNameVec();
	void setTextPropertyNameVec();
	void setPentagonPropertyNameVec();
	void setHexagonPropertyNameVec();
	void setStarPropertyNameVec();
	


	std::map<myqtsvg::ShapeType, std::vector<QString>> m_propertyNameMap;
};

#endif // PROPERTYNAMEVEC_H
