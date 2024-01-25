#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "Order.h"
#include <vector>

struct MonthlySales 
{
	int month;
	double month_sales;
};

class OrderManager
{
public:
	int addOrder(Order* cusOrder);
	int getMaxOrderID();
	int updateOrderStatus(Order* cusOrder);
	int updatePayment(Order* order);
	int deleteOrder(int orderID);

	Order* getOrder(int orderID);
	Order* getOrder(int customerID, int orderID);
	Order* getOrdersHistory(int customerID);
	bool orderExist(int year);
	std::pair<std::unique_ptr<MonthlySales[]>, int> getReportYearly(int year);
	Order* getReportMonthly(int year, int month);
	Order* getOrders();
};

#endif