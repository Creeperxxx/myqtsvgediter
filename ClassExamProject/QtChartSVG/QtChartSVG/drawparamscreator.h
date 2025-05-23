#pragma once
#include <memory>
#include "diagramdrawparams.h"
#include "namespace.h"

class ICreateParams
{
public:
	std::shared_ptr<IDidgramDrawParams> create();
protected:
	virtual std::shared_ptr<IDidgramDrawParams> createSpecial() = 0;
private:
	void createRest(std::shared_ptr<IDidgramDrawParams> params);
};

class createParamsRect : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsCircle : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsTriangle : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsLine : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsMouse : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial()override;
};

class createParamsChoose : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial()override;
};

class createParamsText : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial()override;
};

class createParamsInterface
{
public:
	static createParamsInterface& getInstance();
	createParamsInterface& add(myqtsvg::ShapeType type, std::function<std::shared_ptr<ICreateParams>()> creator);
	std::shared_ptr<ICreateParams> getParams(myqtsvg::ShapeType type);
private:
	createParamsInterface();
	createParamsInterface(const createParamsInterface& other) = delete;
	void defaultInit();

	std::map<myqtsvg::ShapeType, std::function<std::shared_ptr<ICreateParams>()>> m_map;

};
