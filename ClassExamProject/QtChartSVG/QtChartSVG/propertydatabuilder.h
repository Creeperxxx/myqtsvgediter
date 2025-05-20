#pragma once
#include <memory>
#include <vector>

class propertydata;
class IpropertySet;
class IpropertyDataBuilder
{
public:
	virtual void build(std::shared_ptr<IpropertySet> set
		, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) = 0;

};

class drawParamsPropertySet;
class IDrawParamsPropertyDataBuilder :public IpropertyDataBuilder
{
public:
	void build(std::shared_ptr<IpropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
protected:
	virtual void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) = 0;
};

class PenColorDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class PenWidthDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class BrushDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class RotateDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class SpacewidthDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class SpaceheightDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class ScaleDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class CenterHoffsetDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};

class CenterVoffsetDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
};




class RectRadioDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) override;
};

class CircleRadioDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) override;
};

class TriangleRadioDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) override;
};



class TriangleEdgetypeDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) override;
};

class TextFontFamilyDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) override;

};

class TextFontSizeDrawParamsPropertyDataBuilder : public IDrawParamsPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
};





class otherPropertySet;
class IOtherPropertyDataBuilder : public IpropertyDataBuilder
{
public:
	void build(std::shared_ptr<IpropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)override;
protected:
	virtual void probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec) = 0;
};

class NamePropertyDataBuilder : public IOtherPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec);
};

class HuabuHeightPropertyDataBuilder : public IOtherPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> data);
};

class HuabuWidthPropertyDataBuilder : public IOtherPropertyDataBuilder
{
protected:
	void probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> data);
};







class propertyDataVecOfPropertySetCreator
{
public:
	std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> create(std::shared_ptr<IpropertySet> set);
	void addBuilder(std::shared_ptr<IpropertyDataBuilder> builder);

	std::vector<std::shared_ptr<IpropertyDataBuilder>> m_builders;
};

class propertyDataVecOfPropertySetCreatorFactor
{
public:
	static propertyDataVecOfPropertySetCreatorFactor& getInstance();
	std::shared_ptr<propertyDataVecOfPropertySetCreator> create(std::shared_ptr<std::vector<QString>> propertynamevec);
	propertyDataVecOfPropertySetCreatorFactor& addCreator(QString name, std::function<std::shared_ptr<IpropertyDataBuilder>()> func);


private:
	propertyDataVecOfPropertySetCreatorFactor();
	propertyDataVecOfPropertySetCreatorFactor(const propertyDataVecOfPropertySetCreatorFactor&) = delete;

	std::map<QString, std::function<std::shared_ptr<IpropertyDataBuilder>()>> m_builderCreatefunc;
};