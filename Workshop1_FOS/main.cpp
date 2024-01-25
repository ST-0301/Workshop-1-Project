#include <iostream>
#include "UserInterface.h"

void displayCustomerMenu();
void displayCustomerHome(int customerID);

void displayEmployeeMenu();
void displayEmployeeHome(std::string& employeeID);

void displayManagerMenu();
void displayStaffMenu();
void displayRiderMenu(std::string& employeeID);

void manageMeal();
void manageCustomer();
void manageEmployee();
void manageDelivery(std::string& employeeID);
void analysisReport();

int main()
{
    int choice;

    do {
        displayLogo();
        println("Are you a?");
        println("1-Customer");
        println("2-Employee");
        std::cout << "\n";
        println("Select: ");
        std::cin >> choice;

        system("cls");
        switch (choice)
        {
        case 1:
            displayCustomerMenu();
            break;
        case 2:
            displayEmployeeMenu();
            break;
        default:
            displayInvalidChoice();
            continue;
        }
    } while (true);
    return 0;
}


void displayCustomerMenu()
{
    int choice, customerID;

    do {
        printTitle("CUSTOMER REGISTER / LOGIN");
        println("1-Register");
        println("2-Login\n");
        println("Select: ");
        std::cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            customerID = addingCustomer();
            break;
        case 2:
            customerID = loginCustomer();
            break;
        default:
            displayInvalidChoice();
            continue;
        }
        // Invalid login, back to select user
        if (customerID == 0) {
            system("cls");
            return;
        }
        
        pressEnterToContinue();
        system("cls");
        displayCustomerHome(customerID); 
        return;     // Prevent re-entry
    } while (true);
}

void displayCustomerHome(int customerID)
{
    int choice, choiceProfile, choiceOrder, choiceSearch, choiceCategory;
    bool again = false;
    int orderID;
    
    do {
        printTitle("CUSTOMER HOMESCREEN");
        println("1. Place Order\n");
        println("2. Change Delivery Address\n");
        println("3. Modify Order\n");
        println("4. Cancel Order\n");
        println("5. View Order History\n");
        println("6. View Profile\n");
        println("7. Exit\n");
        println("Select: ");
        std::cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            printTitle("PLACE ORDER");

            // To display Food
            choiceCategory = 1;
            displayMealsCategory(choiceCategory);

            // To display Drink
            choiceCategory = 2;
            displayMealsCategory(choiceCategory);

            // To add order
            orderID = addingOrder(customerID);

            pressEnterToContinue();
            system("cls");
            do {
                printTitle("ORDER SCREEN");
                println("1. Modify Current Order\n");
                println("2. Cancel Current Order\n");
                println("3. Search Meal\n");
                println("4. Back to Customer Homescreen\n");
                println("Select: ");
                std::cin >> choiceOrder;

                system("cls");
                switch (choiceOrder) {
                case 1:
                    modifyingOrder(orderID, customerID);
                    break;
                case 2:
                    cancellingOrder(orderID);
                    break;
                case 3:
                    do {
                        printTitle("SEARCH MEAL\n");
                        println("1. Meal ID\n");
                        println("2. Category\n");
                        println("3. Maximum price (Meals that have a price lower than...)\n");
                        println("4. Back to Order screen\n");
                        println("Search by: ");
                        std::cin >> choiceSearch;

                        system("cls");
                        switch (choiceSearch) {
                        case 1:
                            displayMealID();
                            break;
                        case 2:
                            printTitle("SEARCH BY MEAL CATEGORY\n");
                            displayMealsCategory(choiceCategory);
                            break;
                        case 3:
                            displayMealsMaxPrice();
                            break;
                        case 4:
                            break;
                        default:
                            displayInvalidChoice();
                        }
                        if (choiceSearch != 4) {
                            again = getConfirmation("\n\n\tBack to previous screen?");
                            system("cls");
                        }
                    } while (choiceSearch != 4 && again);
                    break;
                case 4:
                    break;  // Break out of the inner loop to go back to the homescreen
                default:
                    displayInvalidChoice();
                }
                pressEnterToContinue();
                system("cls");
            } while (choiceOrder != 4);  // Continue until the user chooses to go back to the homescreen
            orderID = 0;
            break;
        case 2:
            updatingDeliveryAddress(orderID);
            break;
        case 3:
            modifyingOrder(orderID, customerID);
            break;
        case 4:
            cancellingOrder(orderID);
            break;
        case 5:
            printTitle("ORDER HISTORY\n");
            displayOrdersHistory(customerID);
            break;
        case 6:
            do {
                printTitle("CUSTOMER PROFILE\n");
                displayCustomerID(customerID);

                println("1. Update profile\n");
                println("2. Change password\n");
                println("3. Back to Customer Homescreen\n");
                println("Select: ");
                std::cin >> choiceProfile;

                system("cls");
                switch (choiceProfile) {
                case 1:
                    updatingCustomerProfile(customerID);
                    break;
                case 2:
                    updatingCustomerPassword(customerID);
                    break;
                case 3:
                    break;  // Break out of the inner loop to go back to the homescreen
                default:
                    displayInvalidChoice();
                }
                pressEnterToContinue();
                system("cls");
            } while (choiceProfile != 3);  // Continue until the user chooses to go back to the homescreen
            break;
        case 7:
            return;
        default:
            displayInvalidChoice();
        }
        again = getConfirmation("\n\n\tBack to Customer Homescreen? (Note: press N to log out)\n\t");
        system("cls");
    } while (again);
}

void displayEmployeeMenu()
{
    std::string employeeID = loginEmployee();

    if (employeeID == "")
        exit(1);

    pressEnterToContinue();
    system("cls");
    displayEmployeeHome(employeeID);
}

void displayEmployeeHome(std::string& employeeID)
{
    int choice, choiceProfile;
    bool again = false;

    do {
        printTitle("EMPLOYEE HOMESCREEN");
        println("1. View Profile\n");
        println("2. Next\n");
        println("3. Exit\n");
        println("Select: ");
        std::cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            do {
                printTitle("EMPLOYEE PROFILE\n");
                displayEmployeeID(employeeID);

                println("1. Update profile\n");
                println("2. Change password\n");
                println("3. Back to Employee Homescreen\n");
                println("Select: ");
                std::cin >> choiceProfile;

                system("cls");
                switch (choiceProfile) {
                case 1:
                    updatingEmployeeProfile(employeeID);
                    break;
                case 2:
                    updatingEmployeePassword(employeeID);
                    break;
                case 3:
                    break;
                default:
                    displayInvalidChoice();
                }
                system("cls");
            } while (choiceProfile != 3);
            break;
        case 2:
            if (employeeID != "") {
                std::string positionLetter = employeeID.substr(0, 1);

                if (positionLetter == "m")
                    displayManagerMenu();
                else if (positionLetter == "s")
                    displayStaffMenu();
                else if (positionLetter == "r")
                    displayRiderMenu(employeeID);
                else
                    return;
            }
            break;
        case 3:
            return;
        default:
            displayInvalidChoice();
        }
        again = getConfirmation("\n\n\tBack to Employee Homescreen? (Note: press N to log out)\n\t");
        system("cls");
    } while (again);
}


void displayManagerMenu()
{
    int choice;

    do {
        printTitle("MANAGER HOMESCREEN");
        println("1. Manage Meal\n");
        println("2. Manage Customer\n");
        println("3. Manage Employee\n");
        println("4. Report Analysis\n");
        println("5. Back to Employee Homescreen\n");
        println("Select: ");
        std::cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            manageMeal();
            continue;
        case 2:
            manageCustomer();
            continue;
        case 3:
            manageEmployee();
            continue;
        case 4:
            analysisReport();
            continue;
        case 5:
            break;
        default:
            displayInvalidChoice();
        }
        system("cls");
    } while (choice != 5);
}

void displayStaffMenu()
{
    int choice, orderID = 0;
    std::string orderStatus;

    do {
        printTitle("STAFF HOMESCREEN");
        println("1. Update Order Status\n");
        println("2. Manage Meal\n");
        println("3. Manage Customer\n");
        println("4. Back to Employee Homescreen\n");
        println("Select: ");
        std::cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            orderStatus = "Prepared";
            updatingOrderStatus(orderID, orderStatus);
            continue;
        case 2:
            manageMeal();
            continue;
        case 3:
            manageCustomer();
            continue;
        case 4:
            break;
        default:
            displayInvalidChoice();
        }
        system("cls");
    } while (choice != 4);
}

void displayRiderMenu(std::string& employeeID)
{
    int choice;

    do {
        printTitle("RIDER HOMESCREEN");
        println("1. Manage Meal\n");
        println("2. Manage Customer\n");
        println("3. Manage Delivery\n");
        println("4. Back to Employee Homescreen\n");
        println("Select: ");
        std::cin >> choice;

        system("cls");
        switch (choice) {
        case 1:
            manageMeal();
            continue;
        case 2:
            manageCustomer();
            continue;
        case 3:
            manageDelivery(employeeID);
            continue;
        case 4:
            break;
        default:
            displayInvalidChoice();
        }
        system("cls");
    } while (choice != 4);
}


void manageMeal()
{
    int choiceManage, choiceSearch, choiceCategory = 0;
    bool again = false;

    do {
        printTitle("MANAGE MEAL\n");
        println("1. Add Meal\n");
        println("2. Update Meal\n");
        println("3. Delete Meal\n");
        println("4. Search Meal\n");
        println("5. View All Meals\n");
        println("6. Back to previous screen\n");
        println("Select: ");
        std::cin >> choiceManage;

        system("cls");
        switch (choiceManage) {
        case 1:
            addingMeal();
            break;
        case 2:
            updatingMeal();
            break;
        case 3:
            deletingMeal();
            break;
        case 4:
            do {
                printTitle("SEARCH MEAL\n");
                println("1. Meal ID\n");
                println("2. Category\n");
                println("3. Maximum price (Meals that have a price lower than...)\n");
                println("4. Back to Manage Meal screen\n");
                println("Search by: ");
                std::cin >> choiceSearch;
                
                system("cls");
                switch (choiceSearch) {
                case 1:
                    displayMealID();
                    break;
                case 2:
                    printTitle("SEARCH BY MEAL CATEGORY\n");
                    displayMealsCategory(choiceCategory);
                    break;
                case 3:
                    displayMealsMaxPrice();
                    break;
                case 4:
                    break;
                default:
                    displayInvalidChoice();
                }
                if (choiceSearch != 4) {
                    again = getConfirmation("\n\n\tBack to previous screen?");
                    system("cls");
                }
            } while (choiceSearch != 4 && again);
            break;
        case 5:
            printTitle("ALL MEALS\n");

            // To display Food
            choiceCategory = 1;
            displayMealsCategory(choiceCategory);

            // To display Drink
            choiceCategory = 2;
            displayMealsCategory(choiceCategory);
            break;
        case 6:
            break;
        default:
            displayInvalidChoice();
        }
        if (choiceManage != 6) {
            again = getConfirmation("\n\n\tBack to Manage Meal screen?");
            system("cls");
        }
    } while (choiceManage != 6 && again);
}

void manageCustomer()
{
    int choiceManage, choiceSearch, customerID = 0;
    bool again = false;

    do {
        printTitle("MANAGE CUSTOMER\n");
        println("1. Add Customer\n");
        println("2. Update Customer Profile\n");
        println("3. Delete Customer\n");
        println("4. Search Customer\n");
        println("5. View All Customers\n");
        println("6. Back to previous screen\n");
        println("Select: ");
        std::cin >> choiceManage;

        system("cls");
        switch (choiceManage) {
        case 1:
            addingCustomer();
            break;
        case 2:
            updatingCustomerProfile(customerID);
            break;
        case 3:
            deletingCustomer();
            break;
        case 4:
            do {
                printTitle("SEARCH CUSTOMER\n");
                println("1. Customer ID\n");
                println("2. City\n");
                println("3. Back to Manage Customer screen\n");
                println("Search by: ");
                std::cin >> choiceSearch;

                system("cls");
                switch (choiceSearch) {
                case 1:
                    printTitle("SEARCH BY CUSTOMER ID\n");
                    displayCustomerID(customerID);
                    break;
                case 2:
                    printTitle("SEARCH BY CITY\n");
                    displayCustomersCity();
                    break;
                case 3:
                    break;
                default:
                    displayInvalidChoice();
                }
                if (choiceSearch != 3) {
                    again = getConfirmation("\n\n\tBack to previous screen?");
                    system("cls");
                }
            } while (choiceSearch != 3 && again);
            break;
        case 5:
            printTitle("ALL CUSTOMERS\n"); 
            displayAllCustomers();
            break;
        case 6:
            break;
        default:
            displayInvalidChoice();
        }
        if (choiceManage != 6) {
            again = getConfirmation("\n\n\tBack to Manage Customer screen?");
            system("cls");
        }
    } while (choiceManage != 6 && again);
}

void manageEmployee()
{
    int choiceManage, choiceSearch, choicePosition = 0;
    bool again = false;
    std::string employeeID = "";

    do {
        printTitle("MANAGE EMPLOYEE");
        println("1. Add Employee\n");
        println("2. Update Employee Profile\n");
        println("3. Delete Employee\n");
        println("4. Search Employee\n");
        println("5. View All Employees\n");
        println("6. Back to previous screen\n");
        println("Select: ");
        std::cin >> choiceManage;

        system("cls");
        switch (choiceManage) {
        case 1:
            addingEmployee();
            break;
        case 2:
            updatingEmployeeProfile(employeeID);
            break;
        case 3:
            deletingEmployee();
            break;
        case 4:
            do {
                printTitle("SEARCH EMPLOYEE");
                println("1. Employee ID\n");
                println("2. Position\n");
                println("3. Maximum annual salary (Employees that have an annual salary lower than...)\n");
                println("4. Back to Manage Employee screen\n");
                println("Search by: ");
                std::cin >> choiceSearch;

                system("cls");
                switch (choiceSearch) {
                case 1:
                    employeeID = "";
                    printTitle("SEARCH BY EMPLOYEE ID\n");
                    displayEmployeeID(employeeID);
                    break;
                case 2:
                    printTitle("SEARCH BY POSITION\n");
                    displayEmployeesPosition(choicePosition);
                    break;
                case 3:
                    displayEmployeesMaxAnnualSalary();
                    break;
                case 4:
                    break;
                default:
                    displayInvalidChoice();
                }
                if (choiceSearch != 4) {
                    again = getConfirmation("\n\n\tBack to previous screen?");
                    system("cls");
                }
            } while (choiceSearch != 4 && again);
            break;
        case 5:
            printTitle("ALL EMPLOYEES\n");

            choicePosition = 1;
            displayEmployeesPosition(choicePosition);

            choicePosition = 2;
            displayEmployeesPosition(choicePosition);

            choicePosition = 3;
            displayEmployeesPosition(choicePosition);
            break;
        case 6:
            break;
        default:
            displayInvalidChoice();
        }
        if (choiceManage != 6) {
            again = getConfirmation("\n\n\tBack to Manage Employee screen?");
            system("cls");
        }
    } while (choiceManage != 6 && again);
}

void manageDelivery(std::string& employeeID)
{
    int choiceManage, choiceSearch;
    bool again = false;
    std::string deliveryStatus = "";

    do {
        printTitle("MANAGE DELIVERY");
        println("1. Update Delivery Status\n");
        println("2. Search Delivery by Order ID\n");
        println("3. View All Deliveries\n");
        println("4. Back to previous screen\n");
        println("Select: ");
        std::cin >> choiceManage;

        system("cls");
        switch (choiceManage) {
        case 1:
            do {
                printTitle("UPDATE DELIVERY STATUS");
                println("1. Assign delivery\n");
                println("2. Complete delivery\n");
                println("3. Back to Manage Delivery screen\n");
                println("Select: ");
                std::cin >> choiceSearch;

                system("cls");
                switch (choiceSearch) {
                case 1:
                    deliveryStatus = "Assigned";
                    displayDeliveryStatus(1);
                    updatingDeliveryStatus(employeeID, deliveryStatus);
                    break;
                case 2:
                    deliveryStatus = "Completed";
                    displayDeliveryStatus(2);
                    updatingDeliveryStatus(employeeID, deliveryStatus);
                    break;
                case 3:
                    break;
                default:
                    displayInvalidChoice();
                }
                if (choiceSearch != 3) {
                    again = getConfirmation("\n\n\tBack to previous screen?");
                    system("cls");
                }
            } while (choiceSearch != 3 && again);
            break;
        case 2:
            displayDeliveryByOrderID();
            break;
        case 3:
            printTitle("ALL DELIVERIES\n");

            displayDeliveryStatus(1);
            displayDeliveryStatus(2);
            displayDeliveryStatus(3);
            break;
        case 4:
            break;
        default:
            displayInvalidChoice();
        }
        if (choiceManage != 4) {
            again = getConfirmation("\n\n\tBack to Manage Delivery screen?");
            system("cls");
        }
    } while (choiceManage != 4 && again);
}

void analysisReport()
{
    int choiceManage;
    bool again = false;

    do {
        displayLogoReport();

        println("1. Yearly Sales Report\n");
        println("2. Monthly Sales Report\n");
        println("3. Highest Ordered Meal\n");
        println("4. Back to previous screen\n");
        println("Select: ");
        std::cin >> choiceManage;

        system("cls");
        switch (choiceManage) {
        case 1:
            displayReportYearly();
            break;
        case 2:
            displayReportMonthly();
            break;
        case 3:
            displayReportMealID();
            break;
        case 4:
            break;
        default:
            displayInvalidChoice();
        }
        if (choiceManage != 4) {
            again = getConfirmation("\n\n\tBack to Report Analysis screen?");
            system("cls");
        }
    } while (choiceManage != 4 && again);
}