#include "orderdataprocessing.h"
#include <iostream>


void OrderDataProcessing()
{
	std::string inputType;
	std::cout << "choose a type" << std::endl;
	std::cout << "file  console" << std::endl;
	std::cin >> inputType;

	std::unique_ptr<IOrderInputSource> orderInputSource;
	if (inputType == ORDERINPUTTYPEFILE)
	{
		std::cout << "input your file path" << std::endl;
		std::string filePath;
		std::cin >> filePath;
		orderInputSource = OrderInputFactory::createOrderInput(ORDERINPUTTYPEFILE, filePath);
	}
	else if (inputType == ORDERINPUTTYPECONSOLE)
	{
		orderInputSource = OrderInputFactory::createOrderInput(ORDERINPUTTYPECONSOLE,"");
	}
	else
	{
		std::cout << "wrong input type!" << std::endl;
		return;
	}
	std::shared_ptr<std::vector<std::string>> orders = orderInputSource->getOrderFromSource();
	
	std::shared_ptr<std::vector<std::string>> csvStringOrders = std::make_shared<std::vector<std::string>>();
	std::shared_ptr<std::vector<std::string>> pipeDelimitedStringOrders = std::make_shared<std::vector<std::string>>();
	std::unique_ptr<CSVOrderAnalysis> csvAnalysis = std::make_unique<CSVOrderAnalysis>();
	std::unique_ptr<PipedDelimitedOrderAnalysis> pipeDelimitedAnalysis = std::make_unique<PipedDelimitedOrderAnalysis>();
	for (const auto &order : *orders)
	{
		if (order.find(',') != std::string::npos)
		{
			csvStringOrders->push_back(order);
		}
		else if (order.find('|') != std::string::npos)
		{
			pipeDelimitedStringOrders->push_back(order);
		}
		else
		{
			continue;
		}
	}
	std::shared_ptr<std::vector<Order>> csvOrders = csvAnalysis->analyse(csvStringOrders);
	std::shared_ptr<std::vector<Order>> pipeDelimitedOrders = pipeDelimitedAnalysis->analyse(pipeDelimitedStringOrders);
	csvOrders->insert(csvOrders->begin(), pipeDelimitedOrders->begin(), pipeDelimitedOrders->end());
	std::shared_ptr<std::vector<Order>> allOrders = csvOrders;
	std::unique_ptr<OrderValider> orderValider;
	std::string invalidReason;
	std::shared_ptr<std::vector<int>> validOrderIndexVec;
	std::shared_ptr<std::vector<std::pair<int, std::string>>> invalidOrderIndexVec;
	for (int i = 0; i < allOrders->size(); i++)
	{
		if (orderValider->isValid(allOrders->at(i), invalidReason) == true)
		{
			validOrderIndexVec->push_back(i);
		}
		else
		{
			invalidOrderIndexVec->push_back(std::pair<int, std::string>(i, invalidReason));
		}
	}
	std::unique_ptr<OrderStatistics> orderStatisticsInstance;
	orderStatisticsInstance->statisticsAndOutput(allOrders, validOrderIndexVec, invalidOrderIndexVec);
}

int main()
{
	OrderDataProcessing();
	return 0;
}