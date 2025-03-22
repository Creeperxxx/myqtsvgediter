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

std::shared_ptr<std::vector<Order>> CSVOrderAnalysis::analyse(std::shared_ptr<std::vector<std::string>>& vec)
{
	std::shared_ptr<std::vector<Order>> ordersVec = std::make_shared<std::vector<Order>>();
	for (const auto &orderStr : *vec)
	{
		std::istringstream iss(orderStr);
		std::string singleWord;
		int orderId;
		double orderAmount;
		OrderStatus orderStatus;
		if (std::getline(iss, singleWord, ','))
		{
			orderId = std::stoi(singleWord);
		}
		if (std::getline(iss, singleWord, ','))
		{
			orderAmount = std::stod(singleWord);
		}
		if (std::getline(iss, singleWord))
		{
			orderStatus = Order::orderStatusStringToEnum(singleWord);
		}
		ordersVec->push_back(Order(orderId, orderAmount, orderStatus));
	}
	return ordersVec;
}

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
	default:
		return ""; //TODO:orderStatus枚举结构体被新增时这里要修改
	}
}

OrderStatus Order::orderStatusStringToEnum(const std::string& orderStatusString)
{
	if (orderStatusString == ORDERSTATUSCOMPLETED)
		return OrderStatus::OrderStatusCompleted;
	else if (orderStatusString == ORDERSTATUSPENDING)
		return OrderStatus::OrderStatusPending;
	else if (orderStatusString == ORDERSTATUSCANCELED)
		return OrderStatus::OrderStatusCanceled;
		//todo:orderStatus enum结构体新增成员时这里要修改
}
std::shared_ptr<std::vector<Order>> PipedDelimitedOrderAnalysis::analyse(std::shared_ptr<std::vector<std::string>>& vec)
{
	std::shared_ptr<std::vector<Order>> ordersVec = std::make_shared<std::vector<Order>>();
	for (const auto &orderStr : *vec)
	{
		std::istringstream iss(orderStr);
		std::string singleWord;
		int orderId;
		double orderAmount;
		OrderStatus orderStatus;
		if (std::getline(iss, singleWord, '|'))
		{
			orderId = std::stoi(singleWord);
		}
		if (std::getline(iss, singleWord, '|'))
		{
			orderAmount = std::stod(singleWord);
		}
		if (std::getline(iss, singleWord))
		{
			orderStatus = Order::orderStatusStringToEnum(singleWord);
		}
		ordersVec->push_back(Order(orderId, orderAmount, orderStatus));
	}
	return ordersVec;
}


