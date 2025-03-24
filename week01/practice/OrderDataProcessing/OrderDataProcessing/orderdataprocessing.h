#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
//#include <unordered_map>
#include <regex>
#include <map>
#include <algorithm>

constexpr const char* ORDERINPUTTYPEFILE = "file";
constexpr const char* ORDERINPUTTYPECONSOLE = "console";
constexpr const char* CONSOLEORDERINPUTEND = "END";
constexpr const char* ORDERSTATUSCOMPLETED = "completed";
constexpr const char* ORDERSTATUSPENDING = "pending";
constexpr const char* ORDERSTATUSCANCELED = "canceled";
constexpr const char* ORDERSTATUSWRONG = "Wrong";
constexpr const char* ORDERINVALIDREASONINVALIDFIELDCOUNT = "invalid field count";
constexpr const char* ORDERINVALIDREASONDUPLICATEORDERID = "duplicate order id";
constexpr const char* ORDERINVALIDREASONINVALIDORDERID = "invalid order id";
constexpr const char* ORDERINVALIDREASONINVALIDORDERAMOUNT = "invalid order amount";
constexpr const char* ORDERINVALIDREASONINVALIDORDERSTATUS = "invalid order status";
constexpr const char* ORDERINVALIDREASONINVALIDORDERSTRINGFORMAT = "invalid order string format";
//constexpr const char* ORDERINVALIDREASONINVALIDFIELDCOUNT = "invalid field count";
constexpr const char ORDERANALYSISDELIMITERCSV = ',';
constexpr const char ORDERANALYSISDELIMITERPIPE = '|';
//constexpr const char* ORDERISVALIDORDERSTATUSSTRINGFORMATCOMPLETED = "(^\s*completed\s*$)"
//constexpr const char* ORDERISVALIDORDERSTATUSSTRINGFORMATPENDING = "(^\s*pending\s*$)";
//constexpr const char* ORDERISVALIDORDERSTATUSSTRINGFORMATCANCELED = "(^\s*canceled\s*$)";
constexpr const char* ORDERISVALIDORDERIDFORMAT = R"(^[+-]?\d+$)";
constexpr const char* ORDERISVALIDORDERAMOUNTFORMAT = R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d*)?$)";
constexpr const int ORDERSTATISTICSSEPARATORLENGTH = 30;


enum class OrderStatus
{
	OrderStatusCompleted,
	OrderStatusPending,
	OrderStatusCanceled,
	OrderStatusWrong
};

class Order 
{
private:
	static bool isMatchOrderStatusString(const std::string& status,const std::string& statusPattern);
	int m_id;
	double m_amount;
	OrderStatus m_status;
	bool m_isValidFieldCount;
	//int m_fieldCount;
	int m_lineIndex;
	std::string m_invalidReason;
	bool m_isValid;
	std::string m_stringId;
	std::string m_stringAmount;
	std::string m_stringStatus;
public:
	//Order(const std::string& id, const std::string& amount, OrderStatus status,bool isValidFieldCount, int lineIndex);
	Order(const std::string& id, const std::string& amount, const std::string& status,bool isValidFieldCount, int lineIndex);
	Order();
	//Order(const std::string& id, const std::string& amount, OrderStatus status, bool isValidFieldCount);
	//Order(int id, double amount, OrderStatus status, int fieldCount);
	static std::string orderStatusEnumToString(OrderStatus orderStatusEnum);
	static OrderStatus orderStatusStringToEnum(const std::string& orderStatusString);
	int getOrderId() const;
	double getOrderAmount() const;
	OrderStatus getOrderStatus() const;
	//int getOrderFieldCount() const;
	int getOrderLineIndex() const;
	void setInvalidReason(const std::string& reason);
	const std::string& getInvalidReason() const;
	void setIsValid(bool isValid);
	bool getIsValid() const;
	bool getIsVaildFieldCount() const;	
	const std::string& getStringOrderId() const;
	void setOrderId(int orderId);
	const std::string& getStringOrderAmount() const;
	void setOrderAmount(double orderAmount);
	const std::string& getStringStatus() const;
	static bool compareLineIndex(const Order& a, const Order& b);
};

class IOrderAnalysis
{
public:
	//virtual std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::string>>& vec) = 0;
	virtual std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::pair<int, std::string>>>& vec) = 0;
protected:
	Order analyseOneOrderString(const std::string& orderString, int lineIndex, const char dilimiter);
};

class CSVOrderAnalysis : public IOrderAnalysis
{
public: 
	//std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::string>>& vec) override;
	std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::pair<int, std::string>>>& vec) override;
};

class PipedDelimitedOrderAnalysis : public IOrderAnalysis
{
public:
	//std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::string>>& vec) override;
	std::shared_ptr<std::vector<Order>> analyse(std::shared_ptr<std::vector<std::pair<int, std::string>>>& vec) override;
};

class OrderValider
{
private:
	//bool isValidOrderStatusString(const std::string& status);
	bool validateFieldCount(Order& order, std::ostringstream& oss);
	bool validateOrderIdMatch(Order& order, std::ostringstream& oss);
	bool validateOrderIdRange(Order& order, std::ostringstream& oss);
	bool validateOrderIdUnDuplicated(Order& order, std::ostringstream& oss);
	bool validateOrderAmountMatch(Order& order, std::ostringstream& oss);
	bool validateOrderAmountRange(Order& order, std::ostringstream& oss);
	bool validateOrderStatue(Order& order, std::ostringstream& oss);

	std::unordered_set<int> m_ids;
public:
	//bool isValid(const Order& order, std::string& invalidReason);
	void isValid(Order& order);
	bool isOrderIdDuplicated(const int id);
	void addOrderId(const int id);
	OrderValider();
};

class OrderStatistics
{
public:
	OrderStatistics(int allLinesCount);
	void statisticsAndOutput(const std::shared_ptr<std::vector<Order>>& orders, const std::shared_ptr<std::vector<int>>& validOrdersIndex, const std::shared_ptr<std::vector<std::pair<int, std::string>>>& invalidOrdersIndex);
	void statisticsAndOutput(const std::shared_ptr<std::vector<Order>>& orders);
	void addInvalidOrderLineAndReason(int a, const std::string& reason);
private:
	void outputValidOrderHeadline();
	void outputInvalidOrderHeadline();
	void outputSeperator(int length);
	void outputAllLinesCount();
	void outputValidOrdersCount(int validOrdersCount);
	void outputInvalidOrdersCount(int invalidOrdersCount);
	void outputOrderAmountSum(double orderAmountSum);
	void outputOrderAmountAverage(double orderAmountAverage);
	void outputAllInvalidOrderLineIndexAndReason();
	int m_allLinesCount;
	//std::unique_ptr<std::vector<std::pair<int, std::string>>> invalidOrderLineToReasonVec;
	//std::unique_ptr<std::unordered_map<int, std::string>> m_invalidOrderLineToReasonMap;
	std::unique_ptr<std::map<int, std::string>> m_invalidOrderLineToReasonMap;
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




