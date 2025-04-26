#include "configmanager.h"

void configmanager::init(const std::string& filepath)
{
	static bool flag_once = true;
	if (flag_once)
	{
		getinstance().loadconfig(filepath);
		flag_once = false;
	}
}

configmanager& configmanager::getinstance()
{
	static configmanager instance;
	return instance;
}

void configmanager::loadconfig(const std::string& filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		throw std::runtime_error("error");//todo:except
	file >> m_config;
	m_filepath = filepath;
}

nlohmann::json& configmanager::operator[](const std::string& key)
{
	if(!m_config.contains(key))
		throw std::runtime_error("error");//todo:except
	return m_config[key];
}
