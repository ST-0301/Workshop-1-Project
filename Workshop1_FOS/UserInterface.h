#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "OrderManager.h"
#include "DeliveryManager.h"
#include "MealManager.h"
#include "CustomerManager.h"
#include "EmployeeManager.h"

#include <vector>

std::string getInputLine(const std::string& prompt);

void displayLogo();
void displayLogoReceipt();
void displayLogoReport();

void setColor(int color);
void printTitle(const std::string& text);
void printHeader(const std::string& text);
void println(const std::string& text);
void displayInvalidChoice();
void pressEnterToContinue();
bool getConfirmation(const std::string& message);



int addingOrder(int customerID);
double addingDetail(int orderID);
void generatingReceipt(int customerID, int orderID);
void modifyingOrder(int orderID, int customerID);
void cancellingOrder(int orderID);
void displayOrdersHistory(int customerID);
void displayOrderDetailsHistory(int orderID);


int addingDelivery(int orderID);
void updatingDeliveryAddress(int orderID);
void updatingDeliveryStatus(const std::string& employeeID, const std::string& deliveryStatus);
void updatingOrderStatus(int orderID, const std::string& orderStatus);
void displayDeliveryHeader();
void displayDeliveryData(Delivery* delivery);
void displayDeliveryByOrderID();
void displayDeliveryStatus(int choiceStatus);


void displayOrderHeader();
void displayOrderData(Order* order); 
void displayReportYearly();
void displayReportMonthly();
void displayReportMealID();

void addingMeal();
void updatingMeal();
void deletingMeal();
void displayMealHeader();
void displayMealData(Meal* meal);
void displayMealID();
void displayMealsCategory(int choiceCategory);
void displayMealsMaxPrice();


int addingCustomer();
int loginCustomer();
void updatingCustomerProfile(int customerID);
void updatingCustomerPassword(int customerID);
void deletingCustomer();
void displayCustomerHeader();
void displayCustomerData(Customer* customer);
void displayCustomerID(int customerID);
void displayCustomersCity();
void displayAllCustomers();


void addingEmployee();
std::string loginEmployee();
void updatingEmployeeProfile(std::string& employeeID);
void updatingEmployeePassword(std::string& employeeID);
void deletingEmployee();
void displayEmployeeHeader();
void displayEmployeeData(Employee* employee);
void displayEmployeeID(std::string& employeeID);
void displayEmployeesPosition(int choicePosition);
void displayEmployeesMaxAnnualSalary();

#endif