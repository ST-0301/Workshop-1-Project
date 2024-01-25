#include "Customer.h"

#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

class CustomerManager
{
public:
	int addCustomer(Customer* customer);
	int getMaxCustomerID();
	bool loginCustomer(int customerID, const std::string& password);
	int updateCustomer(Customer* customer);
	int updateCustomerPassword(int customerID, const std::string& password);
	int deleteCustomer(Customer* customer);

	Customer* getCustomer(int customerID);
	Customer* getCustomers(const std::string& city);
	Customer* getCustomers();
};

#endif