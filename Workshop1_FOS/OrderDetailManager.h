#include "OrderDetail.h"

#ifndef ORDERDETAILMANAGER_H
#define ORDERDETAILMANAGER_H

class OrderDetailManager
{
public:
	int addDetail(OrderDetail* orderDetail);
	int updateDetail(OrderDetail* orderDetail, int mealID);
	int deleteDetail(int orderID);
	std::pair<int, int> getReportMealID();

	OrderDetail* getDetail(int orderID, int mealID);
	OrderDetail* getDetails(int orderID);
};

#endif