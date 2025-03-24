#include "orderdataprocessing.h"


std::unique_ptr<IOrderInputSource> OrderInputFactory::createOrderInput(const std::string& type, const std::string& filePath)
{
	if (type == ORDERINPUTTYPEFILE)
	{
		return std::make_unique<FileOrderInput>(filePath);
	}
	else if (type == ORDERINPUTTYPECONSOLE)
	{
		return std::make_unique<ConsoleOrderInput>();
	}
	else
	{
		return nullptr;
	}
}

std::shared_ptr<std::vector<std::string>> FileOrderInput::getOrderFromSource()
{
	std::ifstream file(m_filePath);
	if (!file.is_open())
	{
		return {};
	}
	std::shared_ptr<std::vector<std::string>> orders = std::make_shared<std::vector<std::string>>();
	std::string line;
	while (std::getline(file, line))
	{
		orders->push_back(line);
	}
	return orders;
}

std::shared_ptr<std::vector<std::string>> ConsoleOrderInput::getOrderFromSource()
{
	std::shared_ptr<std::vector<std::string>> orders = std::make_shared<std::vector<std::string>>();
	std::string line;
	std::cout << "input your orders ( input " << "END " << "to end input ) " << std::endl;
	while (std::getline(std::cin, line))
	{
		if (line == CONSOLEORDERINPUTEND)
		{
			break;
		}
		else
		{
			orders->push_back(line);
		}
	}
	return orders;
}

//std::shared_ptr<std::vector<Order>> CSVOrderAnalysis::analyse(std::shared_ptr<std::vector<std::string>>& vec)
//{
//	std::shared_ptr<std::vector<Order>> ordersVec = std::make_shared<std::vector<Order>>();
//	std::string id;
//	std::string amount;
//	OrderStatus status;
//	bool isValidFieldCount;
//	for (const auto &orderStr : *vec)
//	{
//		std::tuple<std::string,std::string,OrderStatus,bool> orderInfoTuple = analyseOneOrderString(orderStr,ORDERANALYSISDELIMITERCSV);
//		id = std::get<0>(orderInfoTuple);
//		amount = std::get<1>(orderInfoTuple);
//		status = std::get<2>(orderInfoTuple);
//		isValidFieldCount = std::get<3>(orderInfoTuple);
//		ordersVec->push_back(Order(id, amount, status, isValidFieldCount));
//	}
//	return ordersVec;
//}

std::string Order::orderStatusEnumToString(OrderStatus orderStatusEnum)
{
	switch (orderStatusEnum)
	{
	case OrderStatus::OrderStatusCompleted:
		return ORDERSTATUSCOMPLETED;
	case OrderStatus::OrderStatusPending:
		return ORDERSTATUSPENDING;
	case OrderStatus::OrderStatusCanceled:
		return ORDERSTATUSCANCELED;
	case OrderStatus::OrderStatusWrong:
		return ORDERSTATUSWRONG;
	default:
		return ""; //TODO:orderStatus枚举结构体被新增时这里要修改
	}
}

bool Order::isMatchOrderStatusString(const std::string& status, const std::string& statusPattern)
{
	std::string pattern = R"(^\s*)" + statusPattern + R"(\s*$)";
	std::regex regexPattern(pattern);
	return std::regex_match(status, regexPattern);
}

OrderStatus Order::orderStatusStringToEnum(const std::string& orderStatusString)
{
	if (true == isMatchOrderStatusString(orderStatusString, ORDERSTATUSCOMPLETED))
	{
		return OrderStatus::OrderStatusCompleted;
	}
	else if (true == isMatchOrderStatusString(orderStatusString, ORDERSTATUSPENDING))
	{
		return OrderStatus::OrderStatusPending;
	}
	else if (true == isMatchOrderStatusString(orderStatusString, ORDERSTATUSCANCELED))
	{
		return OrderStatus::OrderStatusCanceled;
	}
	else
	{
		return OrderStatus::OrderStatusWrong;
	}

	//if (orderStatusString == ORDERSTATUSCOMPLETED)
	//	return OrderStatus::OrderStatusCompleted;
	//else if (orderStatusString == ORDERSTATUSPENDING)
	//	return OrderStatus::OrderStatusPending;
	//else if (orderStatusString == ORDERSTATUSCANCELED)
	//	return OrderStatus::OrderStatusCanceled;
	//else if (orderStatusString == ORDERSTATUSWRONG)
	//	return OrderStatus::OrderStatusWrong;
	//else
	//	return OrderStatus::OrderStatusWrong;
		//todo:orderStatus enum结构体新增成员时这里要修改
}
//std::shared_ptr<std::vector<Order>> PipedDelimitedOrderAnalysis::analyse(std::shared_ptr<std::vector<std::string>>& vec)
//{
//	std::shared_ptr<std::vector<Order>> ordersVec = std::make_shared<std::vector<Order>>();
//	std::string singleWord = "";
//	int orderId = 0;
//	double orderAmount = 0;
//	OrderStatus orderStatus = OrderStatus::OrderStatusWrong;
//	int fieldCount = 0;
//	for (const auto &orderStr : *vec)
//	{
//		std::tuple<int, double, OrderStatus, int> orderInfoTuple = analyseOneOrderString(orderStr, ORDERANALYSISDELIMITERPIPE);
//		orderId = std::get<0>(orderInfoTuple);
//		orderAmount = std::get<1>(orderInfoTuple);
//		orderStatus = std::get<2>(orderInfoTuple);
//		fieldCount = std::get<3>(orderInfoTuple);
//		ordersVec->push_back(Order(orderId, orderAmount, orderStatus,fieldCount));
//	}
//	return ordersVec;
//}

//bool OrderValider::isValid(const Order& order, std::string& invalidReason)
//{
//	invalidReason = "";
//	if (order.getOrderFieldCount() != 3)
//	{
//		invalidReason += ORDERINVALIDREASONINVALIDFIELDCOUNT;
//		invalidReason += ' ';
//		return false;
//	}
//	if (order.getOrderId() < 0)
//	{
//		invalidReason += ORDERINVALIDREASONINVALIDORDERID;
//		invalidReason += ' ';
//		return false;
//	}
//	if (isOrderIdDuplicated(order.getOrderId()) == true)
//	{
//		invalidReason += ORDERINVALIDREASONDUPLICATEORDERID;
//		invalidReason += ' ';
//		return false;
//	}
//	if (order.getOrderAmount() < 0)
//	{
//		invalidReason += ORDERINVALIDREASONINVALIDORDERAMOUNT;
//		invalidReason += ' ';
//		return false;
//	}
//	if (order.getOrderStatus() == OrderStatus::OrderStatusWrong)
//	{
//		invalidReason += ORDERINVALIDREASONINVALIDORDERSTATUS;
//		invalidReason += ' ';
//		return false;
//	}
//	return true;
//}

int Order::getOrderId() const
{
	return m_id;
}
double Order::getOrderAmount() const
{
	return m_amount;
}
OrderStatus Order::getOrderStatus() const
{
	return m_status;
}
//int Order::getOrderFieldCount() const
//{
	//return m_fieldCount;
//}


bool OrderValider::isOrderIdDuplicated(const int id)
{
	if (m_ids.find(id) != m_ids.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

OrderValider::OrderValider() {}

void OrderValider::addOrderId(const int id)
{
	m_ids.insert(id);
}


void OrderStatistics::statisticsAndOutput(const std::shared_ptr<std::vector<Order>>& orders, const std::shared_ptr<std::vector<int>>& validOrdersIndex, const std::shared_ptr<std::vector<std::pair<int, std::string>>>& invalidOrdersIndex)
{
	outputAllLinesCount();
	outputValidOrdersCount(static_cast<int>(validOrdersIndex->size()));
	outputInvalidOrdersCount(static_cast<int>(invalidOrdersIndex->size()));
	double orderAmountSum = 0.0;
	for (const auto& i : *validOrdersIndex)
	{
		orderAmountSum += orders->at(i).getOrderAmount();
	}
	outputOrderAmountSum(orderAmountSum);
	double orderAmountAverageval = orderAmountSum / validOrdersIndex->size();
	outputOrderAmountAverage(orderAmountAverageval);

}

void OrderStatistics::outputAllLinesCount()
{
	std::cout << "总行数为：" << m_allLinesCount << std::endl;
}


void OrderStatistics::outputValidOrdersCount(int validOrdersCount)
{
	std::cout << "有效订单数为：" << validOrdersCount << std::endl;
}
void OrderStatistics::outputInvalidOrdersCount(int invalidOrdersCount)
{
	std::cout << "无效订单数为：" << invalidOrdersCount << std::endl;
}


void OrderStatistics::outputOrderAmountSum(double orderAmountSum)
{
	std::cout << "订单总金额为：" << orderAmountSum << std::endl;
}

void OrderStatistics::outputOrderAmountAverage(double orderAmountAverage)
{
	//std::cout << " 订单平均金额为：" << orderAmountAverage << std::endl;
	std::cout << "订单平均金额为：" << std::fixed << std::setprecision(2) << orderAmountAverage << std::endl;
	std::cout.unsetf(std::ios_base::floatfield);
}


void OrderStatistics::addInvalidOrderLineAndReason(int a, const std::string& reason)
{
	//invalidOrderLineToReasonVec->push_back(std::pair<int, std::string>(a, reason));
	m_invalidOrderLineToReasonMap->insert({ a,reason });
}


//Order::Order(const std::string& id, const std::string& amount, OrderStatus status, bool isValidFieldCount, int lineIndex)
Order::Order(const std::string& id, const std::string& amount, const std::string& status, bool isValidFieldCount, int lineIndex)
	: m_stringId(id)
	, m_stringAmount(amount)
	, m_stringStatus(status)
	, m_isValidFieldCount(isValidFieldCount)
	, m_lineIndex(lineIndex)
	, m_isValid(true)
	, m_id(0)
	, m_amount(0.0)
	, m_status(OrderStatus::OrderStatusWrong) {}


int Order::getOrderLineIndex() const
{
	return m_lineIndex;
}

Order::Order() {}

std::shared_ptr<std::vector<Order>> CSVOrderAnalysis::analyse(std::shared_ptr<std::vector<std::pair<int, std::string>>>& vec)
{
	std::shared_ptr<std::vector<Order>> ordersVec = std::make_shared<std::vector<Order>>();
	Order oneOrder;
	for (const auto& orderPair : *vec)
	{
		oneOrder = analyseOneOrderString(orderPair.second, orderPair.first, ORDERANALYSISDELIMITERCSV);
		ordersVec->emplace_back(oneOrder);
		/*std::tuple<std::string, OrderInfoTuple = analyseOneOrderString(orderPair.second, ORDERANALYSISDELIMITERCSV);
		id = std::get<0>(OrderInfoTuple);
		amount = std::get<1>(OrderInfoTuple);
		status = std::get<2>(OrderInfoTuple);
		fieldCount = std::get<3>(OrderInfoTuple);
		ordersVec->push_back(Order(id, amount, status, fieldCount, orderPair.first));*/
	}
	return ordersVec;

}

std::shared_ptr<std::vector<Order>> PipedDelimitedOrderAnalysis::analyse(std::shared_ptr<std::vector<std::pair<int, std::string>>>& vec)
{
	std::shared_ptr<std::vector<Order>> ordersVec = std::make_shared<std::vector<Order>>();
	Order oneOrder;
	for (const auto& orderPair : *vec)
	{
		oneOrder = analyseOneOrderString(orderPair.second, orderPair.first, ORDERANALYSISDELIMITERPIPE);
		ordersVec->emplace_back(oneOrder);
		/*std::tuple<int, double, OrderStatus, int> OrderInfoTuple = analyseOneOrderString(orderPair.second, ORDERANALYSISDELIMITERPIPE);
		id = std::get<0>(OrderInfoTuple);
		amount = std::get<1>(OrderInfoTuple);
		status = std::get<2>(OrderInfoTuple);
		fieldCount = std::get<3>(OrderInfoTuple);
		ordersVec->push_back(Order(id, amount, status, fieldCount, orderPair.first));*/
	}
	return ordersVec;
}


//Order::Order(int id, double amount, OrderStatus status, int fieldCount)
//		: m_id(id) 
//		, m_amount(amount)
//		, m_status(status)
//		, m_fieldCount(fieldCount)
//		, m_isValid(true)
//		, m_lineIndex(0) {}


Order IOrderAnalysis::analyseOneOrderString(const std::string& orderString, int lineIndex, const char delimiter)
{
	std::istringstream iss(orderString);
	std::string singleWord = "";
	std::string orderId = "";
	std::string orderAmount = "";
	bool isValidFieldCount = true;
	//OrderStatus orderStatus = OrderStatus::OrderStatusWrong;
	std::string orderStatus = "";
	//int fieldCount;
	if (std::getline(iss, singleWord, delimiter))
	{
		orderId = singleWord;
		singleWord.clear();
	}
	else
	{
		isValidFieldCount = false;
	}
	if (std::getline(iss, singleWord, delimiter))
	{
		orderAmount = singleWord;
		singleWord.clear();
	}
	else
	{
		isValidFieldCount = false;
	}
	if (std::getline(iss, singleWord))
	{
		orderStatus = singleWord;
	}
	else
	{
		isValidFieldCount = false;
	}
	return Order(orderId, orderAmount, orderStatus, isValidFieldCount, lineIndex);
	//return std::tuple<std::string, std::string, OrderStatus, bool>(orderId, orderAmount, orderStatus, isValidFieldCount);
	//int orderId = 0;
	//double orderAmount = 0;
	//OrderStatus orderStatus = OrderStatus::OrderStatusWrong;
	//int filedCount = 0;
	////todo：下面出问题了，字符串到实际数值的转换应该延迟到验证之后?
	//if (std::getline(iss, singleWord, delimiter))
	//{
	//	orderId = std::stoi(singleWord);
	//	filedCount++;
	//}
	//if (std::getline(iss, singleWord, delimiter))
	//{
	//	orderAmount = std::stod(singleWord);
	//	filedCount++;
	//}
	//if (std::getline(iss, singleWord))
	//{
	//	orderStatus = Order::orderStatusStringToEnum(singleWord);
	//	filedCount++;
	//}
	//return std::tuple<int, double, OrderStatus, int>(orderId, orderAmount, orderStatus, filedCount);
}

void Order::setInvalidReason(const std::string& reason)
{
	m_invalidReason = reason;
}
const std::string& Order::getInvalidReason() const
{
	return m_invalidReason;
}

void Order::setIsValid(bool isValid)
{
	m_isValid = isValid;
}
bool Order::getIsValid() const
{
	return m_isValid;
}


bool Order::getIsVaildFieldCount() const
{
	return m_isValidFieldCount;
}

const std::string& Order::getStringOrderId() const
{
	return m_stringId;
}

void Order::setOrderId(int orderId)
{
	m_id = orderId;
}

const std::string& Order::getStringOrderAmount() const
{
	return m_stringAmount;
}

void Order::setOrderAmount(double orderAmount)
{
	m_amount = orderAmount;
}

const std::string& Order::getStringStatus() const
{
	return m_stringStatus;
}
void OrderValider::isValid(Order& order)
{
	std::ostringstream oss;
	//std::string invalidReason = "";
	bool isvalid = true;
	
	std::vector<bool(OrderValider::*)(const Order&,std::ostringstream&)>validators =
	{
		&OrderValider::validateFieldCount,
		&OrderValider::validateOrderIdMatch,
		&OrderValider::validateOrderIdRange,
		&OrderValider::validateOrderIdUnDuplicated,
		&OrderValider::validateOrderAmountMatch,
		&OrderValider::validateOrderAmountRange,
		&OrderValider::validateOrderStatue
	};
	for (auto validator : validators)
	{
		if (false == (this->*validator)(order, oss))
		{
			isvalid = false;
			break;
		}
	}
	order.setIsValid(isvalid);
	order.setInvalidReason(oss.str());

	////0. 先判断字段数量是否正确
	////1.判断id和amount转int和double
	////2.转了后再进行后面的
	//if (order.getIsVaildFieldCount() == false)
	//{
	//	invalidReason += ORDERINVALIDREASONINVALIDFIELDCOUNT;
	//	invalidReason += ' ';
	//	isvalid = false;
	//	order.setIsValid(isvalid);
	//	order.setInvalidReason(invalidReason);
	//	return;
	//}
	//else
	//{
	//	std::string orderId = order.getStringOrderId();
	//	std::regex integerRegex(R"(^[+-]?\d+$)");
	//	if (!std::regex_match(orderId, integerRegex))
	//	{
	//		isvalid = false;
	//		invalidReason += ORDERINVALIDREASONINVALIDORDERID;
	//		invalidReason += "：";
	//		invalidReason += orderId;
	//	}
	//	else
	//	{
	//		order.setOrderId(std::stoi(orderId));
	//		if (order.getOrderId() < 0)
	//		{
	//			invalidReason += ORDERINVALIDREASONINVALIDORDERID;
	//			invalidReason += ": ";
	//			invalidReason += order.getOrderId();
	//			isvalid = false;
	//		}
	//		if (isOrderIdDuplicated(order.getOrderId()) == true)
	//		{
	//			invalidReason += ORDERINVALIDREASONDUPLICATEORDERID;
	//			invalidReason += ": ";
	//			invalidReason += order.getStringOrderId();
	//			isvalid = false;
	//		}
	//		addOrderId(order.getOrderId());

	//	}

	//	std::string orderAmount = order.getStringOrderAmount();
	//	std::regex floatRegex(R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d*)?$)");
	//	if (!std::regex_match(orderAmount, floatRegex))
	//	{
	//		isvalid = false;
	//		invalidReason += ORDERINVALIDREASONINVALIDORDERAMOUNT;
	//		invalidReason += "：";
	//		invalidReason += orderAmount;
	//	}
	//	else
	//	{
	//		order.setOrderAmount(std::stod(orderAmount));
	//		if (order.getOrderAmount() < 0)
	//		{
	//			invalidReason += ORDERINVALIDREASONINVALIDORDERAMOUNT;
	//			invalidReason += "：";
	//			invalidReason += order.getStringOrderAmount();
	//			isvalid = false;
	//		}

	//	}

	//	//if (order.getOrderFieldCount() != 3)
	//	//{
	//	//	invalidReason += ORDERINVALIDREASONINVALIDFIELDCOUNT;
	//	//	invalidReason += ' ';
	//	//	isvalid = false;
	//	//}
	//	OrderStatus orderStatusEnum = Order::orderStatusStringToEnum(order.getStringStatus());
	//	if (orderStatusEnum == OrderStatus::OrderStatusWrong)
	//	{
	//		invalidReason += ORDERINVALIDREASONINVALIDORDERSTATUS;
	//		invalidReason += "：";
	//		invalidReason += order.getStringStatus();
	//		isvalid = false;
	//	}
	//}

	//order.setIsValid(isvalid);
	//order.setInvalidReason(invalidReason);
}


void OrderStatistics::statisticsAndOutput(const std::shared_ptr<std::vector<Order>>& orders)
{
	outputValidOrderHeadline();
	outputSeperator(ORDERSTATISTICSSEPARATORLENGTH);
	outputAllLinesCount();
	int validOrdersCount = 0;
	int invalidOrdersCount = 0;
	double orderAmountSum = 0.0;
	for (const auto& order : *orders)
	{
		if (order.getIsValid() == true)
		{
			validOrdersCount++;
			orderAmountSum += order.getOrderAmount();
		}
		else
		{
			invalidOrdersCount++;
			addInvalidOrderLineAndReason(order.getOrderLineIndex(), order.getInvalidReason());
		}
	}
	outputValidOrdersCount(invalidOrdersCount);
	outputInvalidOrdersCount(validOrdersCount);
	outputOrderAmountSum(orderAmountSum);
	double orderAmountAverageval = orderAmountSum / validOrdersCount;
	outputOrderAmountAverage(orderAmountAverageval);
	outputSeperator(ORDERSTATISTICSSEPARATORLENGTH);

	std::cout << std::endl;
	outputInvalidOrderHeadline();
	outputAllInvalidOrderLineIndexAndReason();	
	outputSeperator(ORDERSTATISTICSSEPARATORLENGTH);
}


OrderStatistics::OrderStatistics(int allLinesCount)
	: m_allLinesCount(allLinesCount)
{
	m_invalidOrderLineToReasonMap = std::make_unique<std::map<int, std::string>>();
}


void OrderStatistics::outputAllInvalidOrderLineIndexAndReason()
{
	for (const auto& it : *m_invalidOrderLineToReasonMap)
	{
		std::cout << "行号：" << std::setw(2) << it.first << " 原因：" << it.second << std::endl;
	}
}

//Order::Order(const std::string& id, const std::string& amount, OrderStatus status, bool isValidFieldCount)
//	: m_stringId(id)
//	, m_stringAmount(amount)
//	, m_status(status)
//	, m_isValidFieldCount(isValidFieldCount) {}

bool OrderValider::validateFieldCount(const Order& order, std::ostringstream& oss)
{
	if (order.getIsVaildFieldCount() == false)
	{
		oss << ORDERINVALIDREASONINVALIDFIELDCOUNT;
		return false;
	}
	else
	{
		return true;
	}
}
bool OrderValider::validateOrderIdMatch(const Order& order, std::ostringstream& oss)
{
	std::string orderId = order.getStringOrderId();
	//std::regex integerRegex(R"(^[+-]?\d+$)");
	static std::regex integerRegex(ORDERISVALIDORDERIDFORMAT);
	if (false == std::regex_match(orderId, integerRegex))
	{
		//isvalid = false;
		//invalidReason += ORDERINVALIDREASONINVALIDORDERID;
		//invalidReason += "：";
		//invalidReason += orderId;
		oss << ORDERINVALIDREASONINVALIDORDERID << ": " << orderId;
		return false;
	}
	else
	{
		Order* inconstOrder = const_cast<Order*>(&order);
		inconstOrder->setOrderId(std::stoi(orderId));
		return true;
	}

}
bool OrderValider::validateOrderIdRange(const Order& order, std::ostringstream& oss)
{
	int id = order.getOrderId();
	if (id < 0)
	{
		oss << ORDERINVALIDREASONINVALIDORDERID << ": " << id;
		return false;
	}
	else
	{
		return true;
	}
}
bool OrderValider::validateOrderIdUnDuplicated(const Order& order, std::ostringstream& oss)
{
	int id = order.getOrderId();
	if (true == isOrderIdDuplicated(id))
	{
		oss << ORDERINVALIDREASONDUPLICATEORDERID << ": " << id;
		return false;
	}
	else
	{
		addOrderId(id);
		return true;
	}
}
bool OrderValider::validateOrderAmountMatch(const Order& order, std::ostringstream& oss)
{
	std::string orderAmountString = order.getStringOrderAmount();
	static std::regex floatRegex(ORDERISVALIDORDERAMOUNTFORMAT);
	if (false == std::regex_match(orderAmountString, floatRegex))
	{
		oss << ORDERINVALIDREASONINVALIDORDERAMOUNT << ": " << orderAmountString;
		return false;
	}
	else
	{
		Order* inconstOrder = const_cast<Order*>(&order);
		inconstOrder->setOrderAmount(std::stod(orderAmountString));
		return true;
	}
}
bool OrderValider::validateOrderAmountRange(const Order& order, std::ostringstream& oss)
{
	double amount = order.getOrderAmount();
	if (amount < 0)
	{
		oss << ORDERINVALIDREASONINVALIDORDERAMOUNT << ": " << amount;
		return false;
	}
	else
	{
		return true;
	}
}
bool OrderValider::validateOrderStatue(const Order& order, std::ostringstream& oss)
{
	OrderStatus orderStatusEnum = Order::orderStatusStringToEnum(order.getStringStatus());
	if (OrderStatus::OrderStatusWrong == orderStatusEnum)
	{
		oss << ORDERINVALIDREASONINVALIDORDERSTATUS << ": " << order.getStringStatus();
		return false;
	}
	else
	{
		return true;
	}
}
bool Order::compareLineIndex(const Order& a, const Order& b)
{
	return a.getOrderLineIndex() < b.getOrderLineIndex();
}

void OrderStatistics::outputValidOrderHeadline()
{
	std::cout << "订单统计摘要：" << std::endl;
}

void OrderStatistics::outputInvalidOrderHeadline()
{
	std::cout << "无效订单详情：" << std::endl;
}

void OrderStatistics::outputSeperator(int l)
{
	std::cout << std::string(l, '-') << std::endl;
}
