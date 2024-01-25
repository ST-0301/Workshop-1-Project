#include <iostream>
#include <string>
#include <ctime>    // for time_t
#include <iomanip>  // for std::put_time
#include <sstream>  // for std::stringstream
#include <chrono>   // for time-related functions
#include <windows.h>   // for color setting

#include "UserInterface.h"
#include "MealManager.h"
#include "CustomerManager.h"
#include "EmployeeManager.h"
#include "OrderManager.h"
#include "OrderDetailManager.h"
#include "DeliveryManager.h"

// Function to get a line of input with a prompt
std::string getInputLine(const std::string& prompt) 
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

class DateTimeUtils 
{
public:
    static std::string getCurrentDateTime() 
    {
        time_t currentDateTime = time(0);
        std::tm localTime;

        localtime_s(&localTime, &currentDateTime);

        std::stringstream dateTimeStream;
        dateTimeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        return dateTimeStream.str();
    }
};

void displayLogo()
{
    std::string logo = R"(
                                    W   E   L   C   O   M   E     T   O
                                      ____             _                          
                                     / ___| _ __  _ __(_)_ __   __ _              
                                     \___ \| '_ \| '__| | '_ \ / _` |             
                                      ___) | |_) | |  | | | | | (_| |             
                                     |____/| .__/|_|  |_|_| |_|\__, |             
                                           |_|                 |___/              
                           ____            _                               _      
                          |  _ \  ___  ___| |_ __ _  _   _ _ __ __ _ _ __ | |_    
                          | |_) |/ _ \/ __| __/ _` | | | | '__|/ _` | '_ \| __/   
                          |  _ <   __/\__ \ || (_| | |_| | |  | (_| | | | | |_    
                          |_| |_|\___||___/\__\__,_|\__,_|_|   \__,_|_| |_|\__|   

)";
    printTitle(logo);
    std::cout << "\n";
}

void displayLogoReceipt()
{
    std::string logo = R"(
                                     ______              _       _   
                                     | ___ \            (_)     | |  
                                     | |_/ /___  ___ ___ _ _ __ | |_ 
                                     |    // _ \/ __/ _ \ | '_ \| __|
                                     | |\ \  __/ (_|  __/ | |_) | |_ 
                                     \_| \_\___|\___\___|_| .__/ \__|
                                                          | |        
                                                          |_|        
)";
    printTitle(logo);
    std::cout << "\n";
}

void displayLogoReport()
{
    std::string logo = R"(
                _____                       _                             _           _     
               |  __ \                     | |          /\               | |         (_)    
               | |__) |___ _ __   ___  _ __| |_        /  \   _ __   __ _| |_   _ ___ _ ___ 
               |  _  // _ \ '_ \ / _ \| '__| __|      / /\ \ | '_ \ / _` | | | | / __| / __|
               | | \ \  __/ |_) | (_) | |  | |_      / ____ \| | | | (_| | | |_| \__ \ \__ \
               |_|  \_\___| .__/ \___/|_|   \__|    /_/    \_\_| |_|\__,_|_|\__, |___/_|___/
                          | |                                                __/ |          
                          |_|                                               |___/           
)";
    printTitle(logo);
    std::cout << "\n";
}
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printTitle(const std::string& text) 
{
    setColor(11);  // Bright cyan text
    std::cout << std::string(2, '\n');
    std::cout << std::string(1, '\t') << std::setw(90) << std::setfill('*') << "" << std::setfill(' ') << std::endl;
    std::cout << std::string(1, '\t') << std::setw(90) << std::left << text << std::endl;
    std::cout << std::string(1, '\t') << std::setw(90) << std::setfill('*') << "" << std::setfill(' ') << std::endl;
    setColor(7);  // Reset to default color
}

void printHeader(const std::string& text)
{
    setColor(14);  // Bright yellow text
    std::cout << "\n\n" << std::string(1, '\t') << "=== " << text << " ===\n\n";
    setColor(7);  // Reset to default color
}

void println(const std::string& text)
{
    setColor(15);  // Bright white text
    std::cout << std::string(1, '\t') << text;
    setColor(7);  // Reset to default color
}

void displayInvalidChoice() 
{
    std::cout << std::setw(3) << " " << "\tInvalid choice! Please try again.\n";
}

void pressEnterToContinue()
{
    std::cout << "\n\tPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();  // Wait for the user to press Enter
}

bool getConfirmation(const std::string& message)
{
    char confirm;
    do {
        std::cout << message << " Yes(Y) or No(N): ";
        std::cin >> confirm;
    } while (confirm != 'Y' && confirm != 'N');
    return (confirm == 'Y');
}


int addingOrder(int customerID)
{
    OrderManager orderManager;
    Order* order = new Order();

    // Get auto-generated orderID
    int orderID = orderManager.getMaxOrderID() + 1;

    // Display auto-generated orderID
    std::cout << "\n\tOrder ID: " << orderID << std::endl;
    order->orderID = orderID;

    order->order_date_time = DateTimeUtils::getCurrentDateTime();
    std::cout << "\tOrder date and time: " << order->order_date_time << std::endl;

    order->order_status = "Pending";
    order->customerID = customerID;

    order->total_amount = 2; // Delivery fee RM2

    // Create the order
    int status = orderManager.addOrder(order);
    if (status != 0)
        std::cout << "\tOrder successfully created!" << std::endl;
    else
        std::cout << "\tFailed to create new order!" << std::endl;
    

    double detailsAmount = 0.0;
    bool confirm = false;
    do {
        detailsAmount += addingDetail(orderID);

        confirm = getConfirmation("\n\tAdd more meals?");
    } while (confirm);
    order->total_amount += detailsAmount;
    std::cout << "\n\tTotal amount (included delivery fee RM2): RM" << std::fixed << std::setprecision(2) << order->total_amount << std::endl;

    // Must make payment to proceed
    do {
        confirm = getConfirmation("\n\tKindly make a payment to proceed.");
    } while (!confirm);
    order->payment_status = "Paid";

    // Add the order
    int statusUpdate = orderManager.updatePayment(order);
    if (statusUpdate != 0)
        std::cout << "\tOrder successfully added!" << std::endl;
    else
        std::cout << "\tFailed to add new order!" << std::endl;

    // Display receipt
    system("cls");
    generatingReceipt(customerID, orderID);

    // Automatically add a new delivery
    pressEnterToContinue();
    system("cls");
    addingDelivery(orderID);

    delete order;
    return orderID;
}

double addingDetail(int orderID)
{
    double amount;

    OrderDetailManager orderDetailManager;
    OrderDetail* orderDetail = new OrderDetail();

    Meal* meal = nullptr;
    do {
        std::cout << "\n\tMeal ID: ";
        std::cin >> orderDetail->mealID;

        // Check if it exist
        MealManager mealManager;
        meal = mealManager.getMeal(orderDetail->mealID);

        if (meal == nullptr)
            std::cout << "\tMeal ID does not exist!" << std::endl;
    } while (meal == nullptr);

    orderDetail->orderID = orderID;
    std::cout << "\tMeal price: RM" << std::fixed << std::setprecision(2) << meal->meal_price << std::endl;
    orderDetail->current_meal_price = meal->meal_price;
    std::cout << "\tQuantity: ";
    std::cin >> orderDetail->quantity;

    amount = orderDetail->current_meal_price * orderDetail->quantity;
    std::cout << "\tAmount: RM" << std::fixed << std::setprecision(2) << amount << std::endl;

    // Add the order details
    int status = orderDetailManager.addDetail(orderDetail);

    if (status != 0)
        std::cout << "\tOrder details successfully added!" << std::endl;
    else
        std::cout << "\tFailed to add order details!" << std::endl;

    return amount;
}

void generatingReceipt(int customerID, int orderID)
{
    displayLogoReceipt();

    // Check if order exist
    OrderManager orderManager;
    Order* order = orderManager.getOrder(customerID, orderID);

    // Display order's details
    std::cout << "\n\n\tOrder ID: " << order->orderID
        << "\n\tOrder Date and Time: " << order->order_date_time
        << "\n\tCustomer ID: " << order->customerID
        << "\n\tTotal Amount: RM" << std::fixed << std::setprecision(2) << order->total_amount << std::endl << std::endl;

    // Check if order details exist
    OrderDetailManager orderDetailManager;
    OrderDetail* orderDetails = orderDetailManager.getDetails(orderID);

    if (orderDetails != nullptr) {
        OrderDetail* orderDetail = orderDetails;

        // Display meal's header
        std::cout << std::string(1, '\t') << std::left << std::setw(10) << "Meal ID"
            <<  std::left << std::setw(35) << "Meal Name"
            <<  std::left << std::setw(10) << "Category"
            <<  std::left << std::setw(11) << "Price(RM)"
            <<  std::right << std::setw(4) << "Quantity" << std::endl;

        do {
            MealManager mealManager;
            Meal* meal = mealManager.getMeal(orderDetail->mealID);

            //Display meal's data
            std::cout << std::string(1, '\t') << std::left << std::setw(10) << orderDetail->mealID
                <<  std::left << std::setw(35) << meal->meal_name
                <<  std::left << std::setw(10) << meal->category
                <<  std::left << std::setw(11) << std::fixed << std::setprecision(2) << orderDetail->current_meal_price
                <<  std::right << std::setw(4) << orderDetail->quantity << std::endl;

            orderDetail = orderDetail->next;
        } while (orderDetail != nullptr);
    }
    std::cout << "\n\n\tPlease take a screenshot of this receipt and show it to our rider when they arrive.\n\tThank you!\n";
}

void modifyingOrder(int orderID, int customerID)
{
    printTitle("MODIFY ORDER");

    int mealID = 0;
    int existingQuantity;
    double existingPrice, existingAmount, newAmount, diffAmount, befModify;
    bool confirm = false;

    OrderManager orderManager;
    Order* order = nullptr;

    do {
        //displayOrdersHistory(customerID);
        
        // Check if order exist
        order = orderManager.getOrder(orderID);

        if (order == nullptr) {
            std::cout << "\tOrder ID to be modified: ";
            std::cin >> orderID;
        }
        else {
            // Only order with status "Pending" can modify
            if (order->order_status == "Pending") {
                displayOrderDetailsHistory(orderID);

                OrderDetailManager orderDetailManager;
                OrderDetail* existingDetail;
                OrderDetail* newDetail = new OrderDetail;

                do {
                    std::cout << "\n\n\tMeal ID to be modified: ";
                    std::cin >> mealID;

                    // Check if order details exist
                    existingDetail = orderDetailManager.getDetail(orderID, mealID);

                    if (existingDetail == nullptr)
                        std::cout << "\tOrder details does not exist!\n";
                } while (existingDetail == nullptr);

                std::cout << "\tOrder details exist! Proceed with order modification...\n\n";

                existingPrice = existingDetail->current_meal_price;
                existingQuantity = existingDetail->quantity;
                existingAmount = existingPrice * existingQuantity;

                Meal* meal;
                do {
                    std::cout << "\n\tNew meal ID: ";
                    std::cin >> newDetail->mealID;

                    // Check if meal exist
                    MealManager mealManager;
                    meal = mealManager.getMeal(newDetail->mealID);

                    if (meal == nullptr)
                        std::cout << "\tMeal ID does not exist!\n";
                    else
                    {
                        newDetail->orderID = orderID;
                        std::cout << "\tMeal price: RM" << std::fixed << std::setprecision(2) << meal->meal_price << std::endl;
                        newDetail->current_meal_price = meal->meal_price;
                        std::cout << "\tQuantity: ";
                        std::cin >> newDetail->quantity;

                        newAmount = newDetail->current_meal_price * newDetail->quantity;
                        if (newAmount < existingAmount) {
                            std::cout << "\tTotal price must be equal to or greater than the original.\n\tPlease choose another meal!\n";
                        }
                    }
                } while (meal == nullptr || (newAmount < existingPrice));

                std::cout << "\tAmount: RM" << std::fixed << std::setprecision(2) << newAmount << std::endl;

                // Update the order details
                int statusDetail = orderDetailManager.updateDetail(newDetail, mealID);
                if (statusDetail != 0)
                    std::cout << "\tOrder details successfully updated!" << std::endl;
                else
                    std::cout << "\tFailed to update order details!" << std::endl;


                befModify = order->total_amount;
                std::cout << "\n\tTotal amount (before order modification): RM" << std::fixed << std::setprecision(2) << befModify;

                // Update total amount for cusOrder
                order->total_amount = order->total_amount - existingAmount + newAmount;
                std::cout << "\n\tNew total amount: RM" << std::fixed << std::setprecision(2) << order->total_amount << std::endl;

                // Calculate amount difference
                diffAmount = order->total_amount - befModify;
                if (diffAmount > 0) {
                    std::cout << "\n\tPlease note that the selected meal has a difference amount of: RM" << std::fixed << std::setprecision(2) << diffAmount;

                    // Must make payment to proceed
                    do {
                        confirm = getConfirmation("\n\tKindly make payment for amount difference to proceed with this modification.");
                    } while (!confirm);
                    order->payment_status = "Paid";

                    // Update the order
                    int statusOrder = orderManager.updatePayment(order);
                    if (statusOrder != 0)
                        std::cout << "\tOrder successfully updated!" << std::endl;
                    else
                        std::cout << "\tFailed to update an order!" << std::endl;
                }
                else
                    std::cout << "\tNo difference in amount. No payment needed.\n";
                delete newDetail;
            }
            else
                std::cout << "\tOrder modification not allowed! Status: " << order->order_status << std::endl;
        }
    } while (order == nullptr);
}

void displayOrderDetailsHistory(int orderID)
{
    // Check if order detail exist
    OrderDetailManager orderDetailManager;
    OrderDetail* orderDetails = orderDetailManager.getDetails(orderID);

    if (orderDetails != nullptr) {
        OrderDetail* orderDetail = orderDetails;

        // Display meal's header
        std::cout << std::string(1, '\t') << std::left << std::setw(10) << "Meal ID"
            << std::left << std::setw(35) << "Meal Name"
            << std::left << std::setw(10) << "Category"
            << std::left << std::setw(11) << "Price(RM)"
            << std::left << std::setw(4) << "Quantity" << std::endl;

        do {
            MealManager mealManager;
            Meal* meal = mealManager.getMeal(orderDetail->mealID);

            //Display meal's data
            std::cout << std::string(1, '\t') << std::left << std::setw(10) << orderDetail->mealID
                << std::left << std::setw(35) << meal->meal_name
                << std::left << std::setw(10) << meal->category
                << std::left << std::setw(11) << std::fixed << std::setprecision(2) << orderDetail->current_meal_price
                << std::right << std::setw(4) << orderDetail->quantity << std::endl;

            orderDetail = orderDetail->next;
        } while (orderDetail != nullptr);
    }
}

void cancellingOrder(int orderID)
{
    printTitle("CANCEL ORDER");

    bool confirm = false;

    OrderManager orderManager;
    Order* order;
    do {
        // Check if it exist
        order = orderManager.getOrder(orderID);

        if (order == nullptr) {
            std::cout << "\tOrder ID to be cancelled: ";
            std::cin >> orderID;
        }
    } while (order == nullptr);

    confirm = getConfirmation("\tCancel order?");

    if (confirm) {
        // Delete the order details
        OrderDetailManager orderDetailManager;
        int statusDetail = orderDetailManager.deleteDetail(orderID);
        if (statusDetail != 0)
            std::cout << "\tOrder details successfully deleted!" << std::endl;
        else
            std::cout << "\tFailed to delete order details!" << std::endl;

        // Delete the delivery
        DeliveryManager deliveryManager;
        int statusDelivery = deliveryManager.deleteDelivery(orderID);
        if (statusDelivery != 0)
            std::cout << "\tDelivery successfully deleted!" << std::endl;
        else
            std::cout << "\tFailed to delete delivery!" << std::endl;

        // Delete the order
        int statusOrder = orderManager.deleteOrder(orderID);
        if (statusOrder != 0)
            std::cout << "\tOrder successfully cancelled!" << std::endl;
        else
            std::cout << "\tFailed to cancel order!" << std::endl;
    }
}

void displayOrdersHistory(int customerID)
{
    // Check if order exist
    OrderManager orderManager;
    Order* orders = orderManager.getOrdersHistory(customerID);

    if (orders != nullptr) {
        Order* order = orders;
        do {
            // Check if delivery exist
            DeliveryManager deliveryManager;
            Delivery* deliveries = deliveryManager.getDeliveries(order->orderID);

            if (deliveries != nullptr) {
                Delivery* delivery = deliveries;
                do {
                    // Display order's details
                    std::cout << "\n\n\tOrder ID: " << order->orderID
                        << "\n\tOrder Date and Time: " << order->order_date_time
                        << "\n\tTotal Amount: RM" << std::fixed << std::setprecision(2) << order->total_amount
                        << "\n\tOrder Status: " << order->order_status
                        << "\n\tDelivery Status: " << delivery->delivery_status << std::endl << std::endl;

                    delivery = delivery->next;
                } while (delivery != nullptr);
            }

            // Check if order details exist
            OrderDetailManager orderDetailManager;
            OrderDetail* orderDetails = orderDetailManager.getDetails(order->orderID);

            if (orderDetails != nullptr) {
                OrderDetail* orderDetail = orderDetails;

                // Display meal's header
                std::cout << std::string(1, '\t') << std::left << std::setw(10) << "Meal ID"
                     << std::left << std::setw(35) << "Meal Name"
                     << std::left << std::setw(10) << "Category"
                     << std::left << std::setw(11) << "Price(RM)"
                     << std::left << std::setw(4) << "Quantity" << std::endl;

                do {
                    MealManager mealManager;
                    Meal* meal = mealManager.getMeal(orderDetail->mealID);

                    //Display meal's data
                    std::cout << std::string(1, '\t') << std::left << std::setw(10) << orderDetail->mealID
                         << std::left << std::setw(35) << meal->meal_name
                         << std::left << std::setw(10) << meal->category
                         << std::left << std::setw(11) << std::fixed << std::setprecision(2) << orderDetail->current_meal_price
                         << std::right << std::setw(4) << orderDetail->quantity << std::endl;

                    orderDetail = orderDetail->next;
                } while (orderDetail != nullptr);
            }
            order = order->next;
        } while (order != nullptr);
    }
    else
        std::cout << "\n\tNo order history found for customer with ID " << customerID << "!\n";
}




int addingDelivery(int orderID)
{
    printTitle("ADD NEW DELIVERY");

    DeliveryManager deliveryManager;
    Delivery* delivery = new Delivery();

    // Get auto-generated deliveryID
    int deliveryID = deliveryManager.getMaxDeliveryID() + 1;

    // Display auto-generated deliveryID
    std::cout << "\tDelivery ID: " << deliveryID << std::endl;
    delivery->deliveryID = deliveryID;

    delivery->orderID = orderID;
    delivery->delivery_status = "Pending";

    std::cout << "\n\tDelivery Address:\n";
    delivery->delivery_street = getInputLine("\t\tStreet: ");

    std::cout << "\t\tPostcode: ";
    std::cin >> delivery->delivery_postcode;

    std::cin.ignore();
    delivery->delivery_city = getInputLine("\t\tCity: ");
    delivery->delivery_state = getInputLine("\t\tState: ");

    // Add the delivery
    int status = deliveryManager.addDelivery(delivery);
    if (status != 0)
        std::cout << "\tDelivery successfully added!" << std::endl;
    else
        std::cout << "\tFailed to add new delivery!" << std::endl;

    delete delivery;
    return deliveryID;
}

void updatingDeliveryAddress(int orderID)
{
    printTitle("DELIVERY ADDRESS UPDATE\n");

    DeliveryManager deliveryManager;
    Delivery* delivery;

    do {
        // Check if it exist
        delivery = deliveryManager.getDeliveryByOrderID(orderID);

        if (delivery == nullptr) {
            std::cout << "\tOrder ID: ";
            std::cin >> orderID;
        }
        else {
            if (delivery->delivery_status == "Pending") {
                std::cout << "\n\t*Address:\n";
                std::cin.ignore();
                delivery->delivery_street = getInputLine("\t\t*Street: ");

                std::cout << "\t\t*Postcode: ";
                std::cin >> delivery->delivery_postcode;

                std::cin.ignore();
                delivery->delivery_city = getInputLine("\t\t*City: ");
                delivery->delivery_state = getInputLine("\t\t*State: ");


                // Update the delivery
                int status = deliveryManager.updateDelivery(delivery);
                if (status != 0)
                    std::cout << "\tDelivery successfully updated!" << std::endl;
                else
                    std::cout << "\tFailed to update delivery!" << std::endl;
            }
            else {
                std::cout << "\tNot allowed to change delivery address! Status: " << delivery->delivery_status << std::endl;
                break;
            }
        }
    } while (delivery == nullptr);
}

void updatingDeliveryStatus(const std::string& employeeID, const std::string& deliveryStatus)
{
    printTitle("UPDATE DELIVERY AND ORDER STATUS");

    int deliveryID, status = 0;
    std::string deliveryMessage = "";

    // Determine delivery message
    if (deliveryStatus == "Assigned")
        deliveryMessage = "assign";
    else
        deliveryMessage = "complete";

    DeliveryManager deliveryManager;
    Delivery* delivery = new Delivery();
    do {
        std::cout << "\tInsert Delivery ID to " << deliveryMessage << " a delivery: ";
        std::cin >> deliveryID;

        // Check if it exist
        delivery = deliveryManager.getDelivery(deliveryID);

        if (delivery == nullptr)
            std::cout << "\tDelivery ID does not exist!" << std::endl;
    } while (delivery == nullptr);

    // Update employeeID that delivered
    delivery->employeeID = employeeID;
    
    OrderManager orderManager;
    Order* order = orderManager.getOrder(delivery->orderID);

    // Update the delivery status
    if (deliveryStatus == "Assigned") 
    {
        // Check if order being prepared before assigned
        if (order->order_status == "Prepared")
            status = deliveryManager.assignDelivery(delivery);
        else
            std::cout << "\tDelivery already " << delivery->delivery_status << " !\n";
    }
    else {
        if (delivery->delivery_status == "Assigned")
            status = deliveryManager.completeDelivery(delivery);
        else
            std::cout << "\tDelivery " << delivery->delivery_status << " !\n";
    }

    if (status != 0) {
        std::cout << "\tDelivery status successfully updated!" << std::endl;

        // Update corresponding order status
        updatingOrderStatus(delivery->orderID, deliveryStatus);
    }
    else
        std::cout << "\tFailed to update delivery status!" << std::endl;

    delete delivery;
}

void updatingOrderStatus(int orderID, const std::string& orderStatus)
{
    OrderManager orderManager;
    Order* order;
    do {
        // Check if it exist
        order = orderManager.getOrder(orderID);

        if (order == nullptr) {
            std::cout << "\n\tOrder ID: ";
            std::cin >> orderID;
        }
    } while (order == nullptr);

    order->order_status = orderStatus;

    // Update the order
    int status = orderManager.updateOrderStatus(order);
    if (status != 0)
        std::cout << "\tOrder successfully updated!" << std::endl;
    else
        std::cout << "\tFailed to update order!" << std::endl;
}

void displayDeliveryHeader()
{
    std::cout << "\n";
    std::cout << std::string(1, '\t') << std::left << std::setw(13) << "Delivery ID"
         << std::left << std::setw(25) << "Delivered Date and Time"
         << std::left << std::setw(10) << "Order ID"
         << std::left << std::setw(13) << "Employee ID"
         << std::left << std::setw(17) << "Delivery Status"
         << "Address" << std::endl;
}

void displayDeliveryData(Delivery* delivery)
{
    std::cout << std::string(1, '\t') << std::left << std::setw(13) << delivery->deliveryID
         << std::left << std::setw(25) << delivery->delivered_date_time
         << std::left << std::setw(10) << delivery->orderID
         << std::left << std::setw(13) << delivery->employeeID
         << std::left << std::setw(17) << delivery->delivery_status
         << delivery->delivery_street << ", " << delivery->delivery_postcode << ", " << delivery->delivery_city << ", " << delivery->delivery_state << std::endl;
}

void displayDeliveryByOrderID()
{
    printTitle("SEARCH BY ORDER ID\n");

    int orderID;
    Delivery* delivery = nullptr;

    do {
        std::cout << "\tOrder ID: ";
        std::cin >> orderID;

        // Check if it exist
        DeliveryManager deliveryManager;
        delivery = deliveryManager.getDeliveryByOrderID(orderID);

        if (delivery != nullptr) {
            // Display meal's details
            displayDeliveryHeader();
            displayDeliveryData(delivery);
        }
        else
            std::cout << "\tNo delivery found!\n";
    } while (delivery == nullptr);
}

void displayDeliveryStatus(int choiceStatus)
{
    std::string deliveryStatus;

    if (choiceStatus < 1 || choiceStatus > 3) {
        std::cout << "\n\tChoose Status:\n";
        std::cout << "\t1-Preparing\t2-Assigned\t3-Completed\n\tSelect: ";
        std::cin >> choiceStatus;
    }
    while (choiceStatus < 1 || choiceStatus > 3) {
        std::cout << "\tInvalid choice. Please enter a valid status: ";
        std::cin >> choiceStatus;
    }

    if (choiceStatus == 1)
        deliveryStatus = "Preparing";
    else if (choiceStatus == 2)
        deliveryStatus = "Assigned";
    else
        deliveryStatus = "Completed";

    DeliveryManager deliveryManager;
    Delivery* deliveries = deliveryManager.getDeliveries(deliveryStatus);

    if (deliveries != nullptr) {
        Delivery* delivery = deliveries;

        printHeader(deliveryStatus + " Delivery List");

        // Display delivery's details
        displayDeliveryHeader();
        do {
            displayDeliveryData(delivery);
            delivery = delivery->next;
        } while (delivery != nullptr);
    }
    else
        std::cout << "\tNo deliveries found!" << std::endl;
}




void displayOrderHeader()
{
    std::cout << "\n";
    std::cout << std::string(1, '\t') << std::left << std::setw(11) << "Order ID"
        << std::left << std::setw(22) << "Order Date & Time"
        << std::left << std::setw(15) << "Order Status"
        << std::left << std::setw(14) << "Customer ID"
        << std::left << std::setw(13) << "Amount(RM)"
        << std::left << std::setw(15) << "Payment Status" << std::endl;
    std::cout << std::string(1, '\t') << std::string(90, '-') << std::endl << std::endl;
}

void displayOrderData(Order* order)
{
    std::cout << std::string(1, '\t') << std::left << std::setw(11) << order->orderID
        << std::left << std::setw(22) << order->order_date_time
        << std::left << std::setw(15) << order->order_status
        << std::left << std::setw(14) << order->customerID
        << std::left << std::setw(13) << std::fixed << std::setprecision(2) << order->total_amount
        << std::left << std::setw(15) << order->payment_status << std::endl;
}

void displayReportYearly()
{
    int year;
    bool orderExist = false;
    bool confirm = false;

    OrderManager orderManager;

    do {
        std::cout << "\n\tYear (YYYY): ";
        std::cin >> year;

        bool orderExist = orderManager.orderExist(year);

        if (orderExist) {

            std::pair<std::unique_ptr<MonthlySales[]>, int> salesData = orderManager.getReportYearly(year);
            std::unique_ptr<MonthlySales[]> monthlySales = std::move(salesData.first);

            int rowCount = salesData.second;

            // Calculate total yearly sales
            double totalYearlySales = 0.0;
            for (int i = 0; i < rowCount; ++i) {
                totalYearlySales += monthlySales[i].month_sales;
            }

            // Calculate monthly sales as a percentage of yearly sales
            double percentageSales[12] = {};
            for (int i = 0; i < rowCount; ++i) {
                percentageSales[i] = (monthlySales[i].month_sales / totalYearlySales) * 100.0;
            }

            system("cls");
            printTitle("Yearly Sales Report for " + std::to_string(year) + "\n");

            setColor(15); // Set bright white text
            std::cout << "\n\tTotal Sales in " << year << " = RM" << std::fixed << std::setprecision(2) << totalYearlySales << std::string(3, '\n');

            std::cout << std::string(1, '\t') << "  Month"
                << std::string(2, '\t') << "Monthly Sales(RM)"
                << std::string(2, '\t') << "Percentage (based on Yearly Sales)" << std::endl;
            std::cout << std::string(1, '\t') << std::string(90, '-') << std::endl << std::endl;
            setColor(7); // Set to default

            for (int i = 0; i < rowCount; ++i)
            {
                std::cout << std::right << std::setw(12) << monthlySales[i].month
                    << std::right << std::setw(24) << std::fixed << std::setprecision(2) << monthlySales[i].month_sales
                    << std::right << std::setw(29) << std::fixed << std::setprecision(2) << percentageSales[i] << "%\n";
            }
            std::cout << std::string(1, '\n');

            // Chart representation
            printTitle("Chart Representation\n");
            std::cout << std::string(1, '\n');
            println("  Month\t");
            println(" Monthly Sales(RM)\n");

            for (int i = 0; i < rowCount; ++i)
            {
                std::cout << std::right << std::setw(12) << monthlySales[i].month << std::string(2, '\t');
                for (int j = 0; j < percentageSales[i]; j += 5)
                {
                    setColor(j % 2 + 6);
                    std::cout << " #";
                    setColor(7);
                }
                std::cout << "\n";
            }
        }
        else
            confirm = getConfirmation("\tNo orders in this year!\n\tView another year?");
    } while (confirm);
}

void displayReportMonthly()
{
    int year, month;
    bool confirm = false;
    double totalSales = 0;

    OrderManager orderManager;
    Order* orders;
    do {
        println("\n\tYear(YYYY): ");
        std::cin >> year;

        do {
            println("Month(MM) : ");
            std::cin >> month;
        } while (month < 1 || month > 12);

        // Check if order exist
        orders = orderManager.getReportMonthly(year, month);
        if (orders == nullptr) {
            confirm = getConfirmation("\tNo orders in this month!\n\tView another month?");
        }
        else {
            Order* order = orders;

            system("cls");
            printTitle("Monthly Sales Report for " + std::to_string(year) + "\n");
            printHeader("Order List");

            // Display order's details
            displayOrderHeader();
            do {
                if (order != nullptr) {
                    displayOrderData(order);
                    totalSales += order->total_amount;

                    order = order->next;
                }
            } while (order != nullptr);
            std::cout << "\n\n\tTotal Sales in " << month << "/" << year << " = RM" << std::fixed << std::setprecision(2) << totalSales;
        }
    } while (confirm);
}

void displayReportMealID()
{
    OrderDetailManager orderDetailManager;
    std::pair<int, int> mealData = orderDetailManager.getReportMealID();

    int mostOrderedMealID = mealData.first;
    int mealIDCount = mealData.second;

    // Check if mealID was found
    if (mostOrderedMealID > 0) 
    {
        MealManager mealManager;
        Meal* mostOrderedMeal = mealManager.getMeal(mostOrderedMealID);

        printTitle("Highest Ordered Meal\n");
        std::cout << "\n\tWith " << mealIDCount << " orders, "
            << mostOrderedMeal->meal_name << " (Meal ID " << mostOrderedMealID << ") is the clear winner!" << std::endl;
    }
    else
        std::cout << "No meal data found." << std::endl;
}




void addingMeal()
{
    printTitle("ADD NEW MEAL\n");

    MealManager mealManager;
    Meal* meal = new Meal();

    // Set category
    int choiceCategory;
    do {
        std::cout << "\n\t1-Food\t2-Drink\n\tCategory: ";
        std::cin >> choiceCategory;
    } while (choiceCategory != 1 && choiceCategory != 2);

    if (choiceCategory == 1)
        meal->category = "Food";
    else
        meal->category = "Drink";

    // Get meal's details
    std::cin.ignore();
    meal->meal_name = getInputLine("\n\tMeal name: ");
    meal->meal_description = getInputLine("\tMeal description: ");

    std::cout << "\tMeal price: ";
    std::cin >> meal->meal_price;

    // Get auto-generated mealID
    int mealID = mealManager.getMaxMealID() + 1;

    // Display auto-generated mealID
    std::cout << "\tMeal ID: " << mealID << std::endl;

    // Add the meal
    int status = mealManager.addMeal(meal);

    if (status != 0)
        std::cout << "\tSuccessfully added a new meal!" << std::endl;
    else
        std::cout << "\tFailed to add a new meal!" << std::endl;
    
    delete meal;
}

void updatingMeal()
{
    printTitle("UPDATE MEAL\n");
    
    int mealID;

    MealManager mealManager; 
    Meal* meal = new Meal();

    do {
        std::cout << "\tMeal ID to be updated: ";
        std::cin >> mealID;

        // Check if it exist
        meal = mealManager.getMeal(mealID);

        if (meal == nullptr)
            std::cout << "\tMeal ID does not exist!" << std::endl;
        else {
            std::cin.ignore();
            meal->meal_name = getInputLine("\tMeal name: ");

            std::cin.ignore();
            meal->meal_description = getInputLine("\tMeal description: ");

            std::cout << "\tMeal price: ";
            std::cin >> meal->meal_price;

            // Update the meal
            int status = mealManager.updateMeal(meal);
            if (status != 0)
                std::cout << "\tSuccessfully updated an existing meal!" << std::endl;
            else
                std::cout << "\tFailed to update an existing meal!" << std::endl;
        }
    } while (meal == nullptr);
}

void deletingMeal()
{
    printTitle("DELETE MEAL\n");

    int mealID;

    MealManager mealManager;
    Meal* meal = new Meal();

    do {
        std::cout << "\tMeal ID: ";
        std::cin >> mealID;

        // Check if it exist
        meal = mealManager.getMeal(mealID);

        if (meal == nullptr) 
            std::cout << "\tMeal ID does not exist!" << std::endl;
        else {
            // Delete the meal
            int status = mealManager.deleteMeal(meal);
            if (status != 0)
                std::cout << "\tSuccessfully deleted an existing meal!" << std::endl;
            else
                std::cout << "\tFailed to delete an existing meal!" << std::endl;
        }
    } while (meal == nullptr);
    delete meal;
}

void displayMealHeader()
{
    std::cout << "\n";
    std::cout << std::string(1, '\t') << std::left << std::setw(10) << "Meal ID"
        << std::left << std::setw(35) << "Meal Name"
        << std::left << std::setw(10) << "Category"
        << std::left << std::setw(11) << "Price(RM)"
        << "Description" << std::endl;
}

void displayMealData(Meal* meal)
{
    std::cout << std::string(1, '\t') << std::left << std::setw(10) << meal->mealID
        << std::left << std::setw(35) << meal->meal_name
        << std::left << std::setw(10) << meal->category
        << std::left << std::setw(11) << std::fixed << std::setprecision(2) << meal->meal_price
        << meal->meal_description << std::endl;
}

void displayMealID()
{
    printTitle("SEARCH BY MEAL ID\n");

    int mealID;
    Meal* meal = nullptr;

    do {
        println("Meal ID: ");
        std::cin >> mealID;
        
        // Check if it exist
        MealManager mealManager;
        meal = mealManager.getMeal(mealID);
    } while (meal == nullptr);

    // Display meal's details
    displayMealHeader();
    displayMealData(meal);
}

void displayMealsCategory(int choiceCategory)
{
    std::string category;
    
    if (choiceCategory < 1 || choiceCategory > 2) {
        println("Category:");
        println("1-Food\t");
        println("2-Drink\n");
        println("Select: ");
        std::cin >> choiceCategory;
    }
    while (choiceCategory < 1 || choiceCategory > 2) {
        std::cout << "\tInvalid choice. Please enter a valid category: ";
        std::cin >> choiceCategory;
    }

    if (choiceCategory == 1)
        category = "Food";
    else
       category = "Drink";

    MealManager mealManager;
    Meal* meals = mealManager.getMeals(category);

    if (meals != nullptr) 
    {
        Meal* meal = meals;
        
        printHeader(category + " Menu");

        // Display meal's details
        displayMealHeader();
        
        do {
            displayMealData(meal);
            meal = meal->next;
        } while (meal != nullptr);
    }
    else
        std::cout << "\tNo meals found!" << std::endl;
}

void displayMealsMaxPrice()
{
    printTitle("SEARCH BY MAXIMUM PRICE\n");

    double maxPrice;

    do {
        println("Maximum price: ");
        std::cin >> maxPrice;
    } while (maxPrice <= 0);

    // Check if it exist
    MealManager mealManager;
    Meal* meals = mealManager.getMeals(maxPrice);

    if (meals != nullptr) 
    {
        Meal* meal = meals;
        
        // Display meal's details
        displayMealHeader();
        
        do {
            displayMealData(meal);
            meal = meal->next;
        } while (meal != nullptr);
    }
    else
        std::cout << "\tNo meals found!" << std::endl;
}



int addingCustomer()
{
    printTitle("NEW CUSTOMER REGISTER\n");
    
    CustomerManager customerManager;
    Customer* customer = new Customer();

    // Get customer's details
    std::cin.ignore();
    customer->f_name = getInputLine("\t*First name: ");
    customer->l_name = getInputLine("\t*Last name: ");

    std::cout << "\t*Address:\n";
    std::cin.ignore();
    customer->street = getInputLine("\t\t*Street: ");

    std::cout << "\t\t*Postcode: ";
    std::cin >> customer->postcode;

    std::cin.ignore();
    customer->city = getInputLine("\t\t*City: ");
    customer->state = getInputLine("\t\t*State: ");
    customer->tel_no = getInputLine("\t*Tel no: ");

    // Set gender
    int choiceGender;
    do {
        std::cout << "\n\t1-Female\t2-Male\n\t*Gender: ";
        std::cin >> choiceGender;
    } while (choiceGender != 1 && choiceGender != 2);

    if (choiceGender == 1)
        customer->gender = "Female";
    else
        customer->gender = "Male";

    // Get auto-generated customerID
    customer->customerID = customerManager.getMaxCustomerID() + 1;

    // Display auto-generated customerID
    std::cout << "\n\tCustomer ID: " << customer->customerID << std::endl;

    // Get password
    std::cin.ignore();
    customer->password = getInputLine("\t*Password: ");

    // Add the customer
    int status = customerManager.addCustomer(customer);

    if (status != 0)
        std::cout << "\tSuccessfully added a new customer!" << std::endl;
    else
        std::cout << "\tFailed to add a new customer!" << std::endl;

    return customer->customerID;
    delete customer;
}

int loginCustomer()
{
    printTitle("CUSTOMER LOGIN\n");

    int customerID;
    std::string password;

    for (int i = 0; i < 3; i++)
    {
        std::cout << "\n\tCustomer ID: ";
        std::cin >> customerID;

        std::cout << "\tPassword: ";
        std::cin >> password;

        CustomerManager customerManager;
        bool loginStatus = customerManager.loginCustomer(customerID, password);

        if (loginStatus) {
            std::cout << "\n\tLogin successful!" << std::endl;
            return customerID;
        }
        else {
            std::cout << "\n\tInvalid username or password! Attempts left: " << 2 - i << "\n";
        }
    }
    return 0;
}

void updatingCustomerProfile(int customerID)
{
    printTitle("CUSTOMER PROFILE UPDATE\n");

    CustomerManager customerManager;
    Customer* customer = new Customer();

    do {
        // Check if it exist
        customer = customerManager.getCustomer(customerID);

        if (customer == nullptr) {
            std::cout << "\tCustomer ID to be updated: ";
            std::cin >> customerID;
        }
        else {
            std::cin.ignore();
            customer->f_name = getInputLine("\t*First name: ");
            customer->l_name = getInputLine("\t*Last name: ");

            std::cout << "\n\t*Address:\n";
            customer->street = getInputLine("\t\t*Street: ");

            std::cout << "\t\t*Postcode: ";
            std::cin >> customer->postcode;

            std::cin.ignore();
            customer->city = getInputLine("\t\t*City: ");
            customer->state = getInputLine("\t\t*State: ");
            customer->tel_no = getInputLine("\t*Tel no: ");

            // Set gender
            int choiceGender;
            do {
                std::cout << "\n\t1-Female\t2-Male\n\t*Gender: ";
                std::cin >> choiceGender;
            } while (choiceGender != 1 && choiceGender != 2);

            if (choiceGender == 1)
                customer->gender = "Female";
            else
                customer->gender = "Male";

            // Update the customer
            int status = customerManager.updateCustomer(customer);
            if (status != 0)
                std::cout << "\tSuccessfully updated an existing customer!" << std::endl;
            else
                std::cout << "\tFailed to update an existing customer!" << std::endl;
        }
    } while (customer == nullptr);
    delete customer;
}

void updatingCustomerPassword(int customerID)
{
    printTitle("CHANGE PASSWORD\n");

    std::string password;
    char temp;

    for (int i = 0; i < 3; i++) 
    {
        std::cout << "\n\tCurrent password: ";
        std::cin >> password;

        // Check if the current password is correct
        CustomerManager customerManager;
        bool passwordStatus = customerManager.loginCustomer(customerID, password);

        if (passwordStatus) 
        {
            std::cout << "\tNew password: ";
            std::cin >> password;

            // Update the customer
            int status = customerManager.updateCustomerPassword(customerID, password);

            if (status != 0)
                std::cout << "\n\tSuccessfully updated password!\n" << std::endl;
            else
                std::cout << "\n\tFailed to update password!" << std::endl;
            break;
        }
        else
            std::cout << "\n\tInvalid current password! Attempts left: " << 2 - i << "\n";
    }
}

void deletingCustomer()
{
    printTitle("DELETE CUSTOMER\n");

    int customerID;

    CustomerManager customerManager;
    Customer* customer = new Customer();

    do {
        std::cout << "\tCustomer ID to be deleted: ";
        std::cin >> customerID;

        // Check if it exist
        customer = customerManager.getCustomer(customerID);

        if (customer == nullptr)
            std::cout << "\tCustomer ID does not exist!" << std::endl;
        else {
            // Delete the customer
            int status = customerManager.deleteCustomer(customer);
            if (status != 0)
                std::cout << "\tSuccessfully deleted an existing customer!" << std::endl;
            else
                std::cout << "\tFailed to delete an existing customer!" << std::endl;
        }
    } while (customer == nullptr);
    delete customer;
}

void displayCustomerHeader()
{
    std::cout << "\n";
    std::cout << std::string(1, '\t') << std::left << std::setw(14) << "Customer ID"
        << std::left << std::setw(20) << "Name"
        << std::left << std::setw(15) << "Contact"
        << std::left << std::setw(9) << "Gender"
        << "Address" << std::endl;
}

void displayCustomerData(Customer* customer)
{
    std::cout << std::string(1, '\t') << std::left << std::setw(14) << customer->customerID
        << std::left << std::setw(20) << customer->f_name + " " + customer->l_name
        << std::left << std::setw(15) << customer->tel_no
        << std::left << std::setw(9) << customer->gender
        << customer->street << ", " << customer->postcode << ", " << customer->city << ", " << customer->state << std::endl;
}

void displayCustomerID(int customerID)
{
    Customer* customer = nullptr;

    do {
        // Check if it exist
        CustomerManager customerManager;
        customer = customerManager.getCustomer(customerID);

        if (customer == nullptr)
        {
            std::cout << "\n\tCustomer ID: ";
            std::cin >> customerID;
        }
    } while (customer == nullptr);

    // Display customer's details
    std::cout << "\n\tCustomer ID: " << customer->customerID
        << "\n\tName: " << customer->f_name + " " + customer->l_name
        << "\n\tContact: " << customer->tel_no
        << "\n\tGender: " << customer->gender
        << "\n\tAddress: " << customer->street << ", " << customer->postcode << ", " << customer->city << ", " << customer->state << std::endl << std::endl;
}

void displayCustomersCity()
{
    std::string city;
    Customer* customers = nullptr;
    do {
        std::cout << "\n\tCity: ";
        std::cin >> city;

        // Check if it exist
        CustomerManager customerManager;
        customers = customerManager.getCustomers(city);
    } while (customers == nullptr);

    Customer* customer = customers;

    // Display customer's details
    displayCustomerHeader();
    do {
        displayCustomerData(customer);
        customer = customer->next;
    } while (customer != nullptr);
}

void displayAllCustomers()
{
    printHeader("Customer List");

    CustomerManager customerManager;
    Customer* customers = customerManager.getCustomers();
    if (customers != nullptr)
    {
        Customer* customer = customers;

        // Display customer's details
        displayCustomerHeader();
        do {
            displayCustomerData(customer);
            customer = customer->next;
        } while (customer != nullptr);
    }
    else
        std::cout << "\tNo customers found!" << std::endl;
}


void addingEmployee()
{
    printTitle("NEW EMPLOYEE REGISTER\n");

    EmployeeManager employeeManager;
    Employee* employee = new Employee();

    // Get employee's details
    std::cin.ignore();
    employee->f_name = getInputLine("\t*First name: ");
    employee->l_name = getInputLine("\t*Last name: ");
    employee->tel_no = getInputLine("\t*Tel no: ");

    std::cout << "\n\t*Address:\n";
    std::cin.ignore();
    employee->street = getInputLine("\t\t*Street: ");

    std::cout << "\t\t*Postcode: ";
    std::cin >> employee->postcode;

    std::cin.ignore();
    employee->city = getInputLine("\t\t*City: ");
    employee->state = getInputLine("\t\t*State: ");

    // Set gender
    int choiceGender;
    do {
        std::cout << "\n\t1-Female\t2-Male\n\t*Gender: ";
        std::cin >> choiceGender;
    } while (choiceGender != 1 && choiceGender != 2);

    if (choiceGender == 1)
        employee->gender = "Female";
    else
        employee->gender = "Male";

    // Set position
    int choicePosition;
    do {
        std::cout << "\n1-manager\t2-staff\t3-rider\n\t*Position: ";
        std::cin >> choicePosition;
    } while (choicePosition < 1 || choicePosition > 3);

    if (choicePosition == 1)
        employee->position = "manager";
    else if (choicePosition == 2)
        employee->position = "staff";
    else
        employee->position = "rider";

    std::cout << "\n\t*Monthly Salary: ";
    std::cin >> employee->monthly_salary;

    // Get auto-generated employeeID based on position
    int employeeID = employeeManager.getMaxEmployeeID(employee->position) + 1;

    // Display auto-generated employeeID
    employee->employeeID = employee->position.substr(0, 1) + "_" + std::to_string(employeeID);
    std::cout << "\tEmployee ID: " << employee->employeeID << std::endl;

    // Get password
    std::cin.ignore();
    employee->password = getInputLine("\t*Password: ");

    // Add the employee
    int status = employeeManager.addEmployee(employee);

    if (status != 0)
        std::cout << "\tSuccessfully added a new employee!" << std::endl;
    else
        std::cout << "\tFailed to add a new employee!" << std::endl;

    delete employee;
}

std::string loginEmployee()
{
    printTitle("EMPLOYEE LOGIN\n");

    std::string employeeID;
    std::string password;
 
    for (int i = 0; i < 3; i++)
    {
        std::cout << "\n\tEmployee ID: ";
        std::cin >> employeeID;

        std::cout << "\tPassword: ";
        std::cin >> password;

        EmployeeManager employeeManager;
        bool loginStatus = employeeManager.loginEmployee(employeeID, password);

        if (loginStatus) {
            std::cout << "\n\tLogin successful!\n";
            return employeeID;
        }
        else
            std::cout << "\n\tInvalid username or password! Attempts left: " << 2 - i << "\n";
    }
    return "";
}

void updatingEmployeeProfile(std::string& employeeID)
{
    printTitle("EMPLOYEE PROFILE UPDATE\n");

    EmployeeManager employeeManager;
    Employee* employee = new Employee();

    do {
        // Check if it exist
        employee = employeeManager.getEmployee(employeeID);
        
        if (employee == nullptr) {
            std::cout << "\tEmployee ID to be updated: ";
            std::cin >> employeeID;
        }
        else {
            std::cin.ignore();
            employee->f_name = getInputLine("\t*First name: ");
            employee->l_name = getInputLine("\t*Last name: ");
            employee->tel_no = getInputLine("\t*Tel no: ");

            std::cout << "\n\t*Address:\n";
            employee->street = getInputLine("\t\t*Street: ");

            std::cout << "\t\t*Postcode: ";
            std::cin >> employee->postcode;

            std::cin.ignore();
            employee->city = getInputLine("\t\t*City: ");
            employee->state = getInputLine("\t\t*State: ");

            // Set gender
            int choiceGender;
            do {
                std::cout << "\n\t1-Female\t2-Male\n\t*Gender: ";
                std::cin >> choiceGender;
            } while (choiceGender != 1 && choiceGender != 2);

            if (choiceGender == 1)
                employee->gender = "Female";
            else
                employee->gender = "Male";

            std::cout << "\n\t*Monthly Salary: ";
            std::cin >> employee->monthly_salary;

            // Update the employee
            int status = employeeManager.updateEmployee(employee);
            if (status != 0)
                std::cout << "\tSuccessfully updated an existing employee!" << std::endl;
            else
                std::cout << "\tFailed to update an existing employee!" << std::endl;
        }
    } while (employee == nullptr);
    delete employee;
}

void updatingEmployeePassword(std::string& employeeID)
{
    printTitle("CHANGE PASSWORD\n");

    std::string password, newPassword;

    for (int i = 0; i < 3; i++)
    {
        std::cout << "\n\tCurrent password: ";
        std::cin >> password;

        // Check if the current password is correct
        EmployeeManager employeeManager;
        bool passwordStatus = employeeManager.loginEmployee(employeeID, password);

        if (passwordStatus)
        {
            std::cout << "\tNew password: ";
            std::cin >> newPassword;

            // Update the customer
            int status = employeeManager.updateEmployeePassword(employeeID, newPassword);

            if (status != 0)
                std::cout << "\n\tSuccessfully updated password!\n";
            else
                std::cout << "\n\tFailed to update password!\n";
            break;
        }
        else
            std::cout << "\n\tInvalid current password! Attempts left: " << 2 - i << "\n";
    }
}

void deletingEmployee()
{
    printTitle("DELETE EMPLOYEE\n");

    std::string employeeID;

    EmployeeManager employeeManager;
    Employee* employee = new Employee();

    do {
        std::cout << "\tEmployee ID to be deleted: ";
        std::cin >> employeeID;

        // Check if it exist
        employee = employeeManager.getEmployee(employeeID);

        if (employee == nullptr)
            std::cout << "\tEmployee ID does not exist!" << std::endl;
        else {
            // Delete the employee
            int status = employeeManager.deleteEmployee(employee);
            if (status != 0)
                std::cout << "\tSuccessfully deleted an existing employee!" << std::endl;
            else
                std::cout << "\tFailed to delete an existing employee!" << std::endl;
        }
    } while (employee == nullptr);
    delete employee;
}

void displayEmployeeHeader()
{
    std::cout << "\n";
    std::cout << std::string(1, '\t') << std::left << std::setw(14) << "Employee ID"
        <<  std::left << std::setw(20) << "Name"
        <<  std::left << std::setw(15) << "Contact"
        <<  std::left << std::setw(9) << "Gender"
        <<  std::left << std::setw(11) << "Position"
        <<  std::left << std::setw(21) << "Monthly Salary(RM)"
        <<  "Address" << std::endl;
}

void displayEmployeeData(Employee* employee)
{
    std::cout << std::string(1, '\t') << std::left << std::setw(14) << employee->employeeID
        <<  std::left << std::setw(20) << employee->f_name + " " + employee->l_name
        <<  std::left << std::setw(15) << employee->tel_no
        <<  std::left << std::setw(9) << employee->gender
        <<  std::left << std::setw(11) << employee->position
        <<  std::left << std::setw(21) << std::fixed << std::setprecision(2) << employee->monthly_salary
        <<  employee->street << ", " << employee->postcode << ", " << employee->city << ", " << employee->state << std::endl;
}

void displayEmployeeID(std::string& employeeID)
{
    Employee* employee = nullptr;
    
    do {
        // Check if it exist
        EmployeeManager employeeManager;
        employee = employeeManager.getEmployee(employeeID);

        if (employee == nullptr)
        {
            std::cout << "\n\tEmployee ID: ";
            std::cin >> employeeID;
        }
    } while (employee == nullptr);

    // Display employee's details
    std::cout << "\n\tEmployee ID: " << employee->employeeID
        << "\n\tName: " << employee->f_name + " " + employee->l_name
        << "\n\tContact: " << employee->tel_no
        << "\n\tGender: " << employee->gender
        << "\n\tPosition: " << employee->position
        << "\n\tMonthly Salary(RM): " << std::fixed << std::setprecision(2) << employee->monthly_salary
        << "\n\tAddress: " << employee->street << ", " << employee->postcode << ", " << employee->city << ", " << employee->state << std::endl << std::endl;
}

void displayEmployeesPosition(int choicePosition)
{
    std::string position;

    if (choicePosition < 1 || choicePosition > 3) {
        std::cout << "\n\tChoose Position:\n";
        std::cout << "\t1-manager\t2-staff\t3-rider\n\tSelect: ";
        std::cin >> choicePosition;
    }
    while (choicePosition < 1 || choicePosition > 3) {
        std::cout << "\tInvalid choice. Please enter a valid position: ";
        std::cin >> choicePosition;
    }

    if (choicePosition == 1)
        position = "manager";
    else if (choicePosition == 2)
        position = "staff";
    else
        position = "rider";

    EmployeeManager employeeManager;
    Employee* employees = employeeManager.getEmployees(position);

    if (employees != nullptr)
    {
        Employee* employee = employees;
        
        printHeader(position + " List");

        // Display employee's details
        displayEmployeeHeader();

        do {
            displayEmployeeData(employee);
            employee = employee->next;
        } while (employee != nullptr);
    }
    else
        std::cout << "\tNo employees found!" << std::endl;
}

void displayEmployeesMaxAnnualSalary()
{
    printTitle("SEARCH BY MAXIMUM ANNUAL SALARY\n");
    
    double maxAnnualSalary;
    do {
        std::cout << "\n\tMaximum Annual Salary: ";
        std::cin >> maxAnnualSalary;
    } while (maxAnnualSalary <= 0);

    // Check if it exist
    EmployeeManager employeeManager;
    Employee* employees = employeeManager.getEmployees(maxAnnualSalary);

    if (employees != nullptr)
    {
        Employee* employee = employees;

        // Display employee's details
        displayEmployeeHeader();

        do {
            displayEmployeeData(employee);
            employee = employee->next;
        } while (employee != nullptr);
    }
    else
        std::cout << "\tNo employees found!" << std::endl;
}