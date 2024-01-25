#include "DatabaseConnection.h"
#include "OrderManager.h"

void transform(sql::ResultSet* rs, Order* order)
{
    order->orderID = rs->getInt("orderID");
    order->order_date_time = rs->getString("order_date_time");
    order->order_status = rs->getString("order_status");
    order->customerID = rs->getInt("customerID");
    order->total_amount = rs->getDouble("total_amount");
    order->payment_status = rs->getString("payment_status");
}

int OrderManager::addOrder(Order* order)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("INSERT INTO CUSORDER (orderID, order_date_time, order_status, customerID, total_amount, payment_status) VALUES (?, ?, ?, ?, ?, ?)");
    ps->setInt(1, order->orderID);
    ps->setString(2, order->order_date_time);
    ps->setString(3, order->order_status);
    ps->setInt(4, order->customerID);
    ps->setDouble(5, order->total_amount);
    ps->setString(6, order->payment_status);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int OrderManager::getMaxOrderID()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT MAX(orderID) AS maxID FROM CUSORDER");
    sql::ResultSet* rs = ps->executeQuery();

    int maxID = 0;
    if (rs->next())
    {
        maxID = rs->getInt("maxID");
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return maxID;
}

int OrderManager::updateOrderStatus(Order* order)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE CUSORDER SET order_status = ? WHERE orderID = ?");
    ps->setString(1, order->order_status);
    ps->setInt(2, order->orderID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int OrderManager::updatePayment(Order* order)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE CUSORDER SET total_amount = ?, payment_status = ? WHERE orderID = ?");
    ps->setDouble(1, order->total_amount);
    ps->setString(2, order->payment_status);
    ps->setInt(3, order->orderID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}


int OrderManager::deleteOrder(int orderID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("DELETE FROM CUSORDER WHERE orderID = ?");
    ps->setInt(1, orderID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

Order* OrderManager::getOrder(int orderID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER WHERE orderID = ?");
    ps->setInt(1, orderID);

    sql::ResultSet* rs = ps->executeQuery();
    Order* order = nullptr;

    if (rs->next())
    {
        order = new Order();
        transform(rs, order);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return order;
}

Order* OrderManager::getOrder(int customerID, int orderID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER WHERE customerID = ? AND orderID = ?");
    ps->setInt(1, customerID);
    ps->setInt(2, orderID);

    sql::ResultSet* rs = ps->executeQuery();
    Order* order = nullptr;

    if (rs->next())
    {
        order = new Order();
        transform(rs, order);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return order;
}

Order* OrderManager::getOrdersHistory(int customerID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER WHERE customerID = ?");
    ps->setInt(1, customerID);

    sql::ResultSet* rs = ps->executeQuery();
    Order* order = nullptr;
    Order* head = nullptr;

    while (rs->next())
    {
        if (order == nullptr)
        {
            order = new Order();
            head = order;
        }
        else
        {
            order->next = new Order();
            order = order->next;
        }
        transform(rs, order);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

bool OrderManager::orderExist(int year)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER WHERE YEAR(order_date_time) = ?");
    ps->setInt(1, year);

    sql::ResultSet* rs = ps->executeQuery();

    bool orderExist;

    if (rs->next())
        orderExist = true;
    else
        orderExist = false;

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return orderExist;
}

std::pair<std::unique_ptr<MonthlySales[]>, int> OrderManager::getReportYearly(int year)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement(
        "SELECT MONTH(order_date_time) AS month, SUM(total_amount) AS total_amount "
        "FROM CUSORDER "
        "WHERE YEAR(order_date_time) = ? "
        "GROUP BY MONTH(order_date_time)");
    ps->setInt(1, year);

    sql::ResultSet* rs = ps->executeQuery();

    int rowCount = rs->rowsCount();
    std::unique_ptr<MonthlySales[]> monthlySales = std::make_unique<MonthlySales[]>(rs->rowsCount());
    
    int i = 0;
    while (rs->next()) 
    {
        monthlySales[i].month = rs->getInt("month");
        monthlySales[i].month_sales = rs->getDouble("total_amount");
        i++;
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return { std::move(monthlySales), rowCount };
}

Order* OrderManager::getReportMonthly(int year, int month)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER WHERE YEAR(order_date_time) = ? AND MONTH(order_date_time) = ?");
    ps->setInt(1, year);
    ps->setInt(2, month);

    sql::ResultSet* rs = ps->executeQuery();
    Order* order = nullptr;
    Order* head = nullptr;

    while (rs->next())
    {
        if (order == nullptr)
        {
            order = new Order();
            head = order;
        }
        else
        {
            order->next = new Order();
            order = order->next;
        }
        transform(rs, order);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Order* OrderManager::getOrders()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER");
    sql::ResultSet* rs = ps->executeQuery();
    Order* order = nullptr;
    Order* head = nullptr;

    while (rs->next())
    {
        if (order == nullptr)
        {
            order = new Order();
            head = order;
        }
        else
        {
            order->next = new Order();
            order = order->next;
        }
        transform(rs, order);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;    //return head pointer, not the last pointer
}