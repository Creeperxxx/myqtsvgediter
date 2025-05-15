#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <optional>
#include "config.h"


//也许可以预先将配置项加载到静态变量中这样就能直接使用

constexpr const char keyfengefu = '.';

class configmanager;

template <typename T>
T cfggetval(const std::string key, const std::optional<T> defaultvalue = std::nullopt)
{
	return configmanager::getinstance().getvalue(key, defaultvalue);
}


class configmanager
{
public:
	configmanager(const configmanager& other) = delete;
	configmanager(configmanager&& other) = delete;
	configmanager& operator=(const configmanager& other) = delete;
	static configmanager& getinstance();
	static void init(const std::string& filepath);
	nlohmann::json& operator[](const std::string& key);
	template <typename T>
	T getvalue(const std::string& key, const std::optional<T> defaultvalue = std::nullopt);
private:
	configmanager() = default;
	~configmanager() = default;
	void loadconfig(const std::string& filepath);

	std::string m_filepath;
	nlohmann::json m_config;
};

template <typename T>
T configmanager::getvalue(const std::string& key, const std::optional<T> defaultvalue)
{
	std::istringstream iss(key);
	std::vector<std::string> words;
	std::string word;
	
	while (std::getline(iss, word, keyfengefu))
	{
		if (!word.empty())
			words.push_back(word);
	}

	nlohmann::json ret = m_config;
	bool found = true;

	for (const auto& k : words)
	{
		if (ret.contains(k))
			ret = ret[k];
		else
		{
			found = false;
			break;
		}
	}
	if (found)
		return ret.get<T>();
	else
	{
		if (defaultvalue.has_value())
			return defaultvalue.value();
		else
			throw std::runtime_error("error");
	}
}

