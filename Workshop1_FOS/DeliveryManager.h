#include "Delivery.h"

#ifndef DELIVERYMANAGER_H
#define DELIVERYMANAGER_H

class DeliveryManager
{
public:
	int addDelivery(Delivery* delivery);
	int getMaxDeliveryID();
	int updateDelivery(Delivery* delivery);
	int assignDelivery(Delivery* delivery);
	int completeDelivery(Delivery* delivery);
	int deleteDelivery(int orderID);

	Delivery* getDelivery(int deliveryID);
	Delivery* getDeliveryByOrderID(int orderID);
	Delivery* getDeliveries(int orderID);
	Delivery* getDeliveries(const std::string& deliveryStatus);
	Delivery* getDeliveries();
};

#endif