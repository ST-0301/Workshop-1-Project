#ifndef ORDER_H
#define ORDER_H

class Order
{
public:
	int orderID;
	std::string order_date_time;
	std::string order_status;
	int customerID, mealID, quantity;
	double total_amount;
	std::string payment_status;
	int deliveryID;
	int month;

	Order* next;
};

#endif