#ifndef ORDERDETAIL_H
#define ORDERDETAIL_H

class OrderDetail
{
public:
	int orderID, mealID;
	double current_meal_price;
	int quantity;

	OrderDetail* next;
};

#endif