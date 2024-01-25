#ifndef DELIVERY_H
#define DELIVERY_H

class Delivery
{
public:
	int deliveryID;
	//time_t delivered_date_time;
	std::string delivered_date_time;
	std::string delivery_status, delivery_street;
	int delivery_postcode;
	std::string delivery_city, delivery_state;
	int orderID;
	std::string employeeID;

	Delivery* next;
};

#endif