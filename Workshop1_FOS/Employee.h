#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee
{
public:
	std::string employeeID;
	std::string f_name, l_name, tel_no, street;
	int postcode;
	std::string city, state, gender, position;
	double monthly_salary;
	std::string password;

	Employee* next;
};

#endif