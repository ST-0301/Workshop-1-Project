#include "Employee.h"

#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

class EmployeeManager
{
public:
	int addEmployee(Employee* employee);
	int getMaxEmployeeID(const std::string& position);
	bool loginEmployee(const std::string& employeeID, const std::string& password);
	int updateEmployee(Employee* employee);
	int updateEmployeePassword(const std::string& employeeID, const std::string& password);
	int deleteEmployee(Employee* employee);

	Employee* getEmployee(const std::string& employeeID);
	Employee* getEmployees(const std::string& position);
	Employee* getEmployees(double maxAnnualSalary);
	Employee* getEmployees();
};

#endif