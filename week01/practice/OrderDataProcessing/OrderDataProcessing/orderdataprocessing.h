#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>

constexpr const char* ORDERINPUTTYPEFILE = "file";
constexpr const char* ORDERINPUTTYPECONSOLE = "console";
constexpr const char* CONSOLEORDERINPUTEND = "END";
constexpr const char* ORDERSTATUSCOMPLETED = "Completed";
constexpr const char* ORDERSTATUSPENDING = "Pending";
constexpr const char* ORDERSTATUSCANCELED = "Canceled";



enum class OrderStatus
{
	OrderStatusCompleted,
	OrderStatusPending,
	OrderStatusCanceled
};

class Order
{
private:
	int m_id;
	double m_amount;
	OrderStatus m_status;
public:
	Order(int id, double amount, OrderStatus status) :m_id(id), m_amount(amount), m_status(status) {}
	static std::string orderStatusEnumToString(OrderStatus orderStatusEnum);
	static OrderStatus orderStatusStringToEnum(const std::string& orderStatusString);
	
};

class IOrderAnalysis
{
public:
	virtual std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::string>>& vec) = 0;
};

class CSVOrderAnalysis : public IOrderAnalysis
{
public: 
	std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::string>>& vec) override;
};

class PipedDelimitedOrderAnalysis : public IOrderAnalysis
{
public:
	std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::string>>& vec) override;
};

class OrderValider
{
private:
	std::unordered_set<int> m_ids;
public:
	bool isValid(const Order& order, std::string& invalidReason);
};

class OrderStatistics
{
public:
	void statisticsAndOutput(const std::shared_ptr<std::vector<Order>>& orders, const std::shared_ptr<std::vector<int>>& validOrdersIndex, const std::shared_ptr<std::vector<std::pair<int, std::string>>>& invalidOrdersIndex);

};

class IOrderInputSource
{
public:
	//virtual std::vector<std::string> getOrderFromSource() = 0;
	virtual std::shared_ptr<std::vector<std::string>> getOrderFromSource() = 0;
};

class FileOrderInput : public IOrderInputSource
{
private:
	std::string m_filePath;
public:
	FileOrderInput(const std::string& filePath) :m_filePath(filePath) {}
public:
	//std::vector<std::string> getOrderFromSource() override;
	std::shared_ptr<std::vector<std::string>> getOrderFromSource() override;
};

class ConsoleOrderInput : public IOrderInputSource
{
public:
	//std::vector<std::string> getOrderFromSource() override;
	std::shared_ptr<std::vector<std::string>> getOrderFromSource() override;
};

class OrderInputFactory
{
public:
	static std::unique_ptr<IOrderInputSource> createOrderInput(const std::string& type, const std::string& filePath);
};




