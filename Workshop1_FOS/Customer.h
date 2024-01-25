#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer
{
public:
	int customerID;
	std::string f_name, l_name, tel_no, street;
	int postcode;
	std::string city, state, gender, password;

	Customer* next;
};

#endif