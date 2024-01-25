#include "DatabaseConnection.h"
#include "OrderDetailManager.h"

void transform(sql::ResultSet* rs, OrderDetail* orderDetail)
{
    orderDetail->orderID = rs->getInt("orderID");
    orderDetail->mealID = rs->getInt("mealID");
    orderDetail->current_meal_price = rs->getDouble("current_meal_price");
    orderDetail->quantity = rs->getInt("quantity");
}

int OrderDetailManager::addDetail(OrderDetail* orderDetail)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("INSERT INTO CUSORDER_DETAIL (orderID, mealID, current_meal_price, quantity) VALUES (?, ?, ?, ?)");
    ps->setInt(1, orderDetail->orderID);
    ps->setInt(2, orderDetail->mealID);
    ps->setDouble(3, orderDetail->current_meal_price);
    ps->setInt(4, orderDetail->quantity);

    int numRowsAffected = ps->executeUpdate();

    ps->close();
    delete ps;

    return numRowsAffected;
}

int OrderDetailManager::updateDetail(OrderDetail* orderDetail, int mealID)
{
    DatabaseConnection dbConn;
    
    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE CUSORDER_DETAIL SET mealID = ?, current_meal_price = ?, quantity = ? WHERE orderID = ? AND mealID = ?");
    ps->setInt(1, orderDetail->mealID);
    ps->setDouble(2, orderDetail->current_meal_price);
    ps->setInt(3, orderDetail->quantity);
    ps->setInt(4, orderDetail->orderID);
    ps->setInt(5, mealID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int OrderDetailManager::deleteDetail(int orderID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("DELETE FROM CUSORDER_DETAIL WHERE orderID = ?");
    ps->setInt(1, orderID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

std::pair<int, int> OrderDetailManager::getReportMealID()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement(
        "SELECT mealID, COUNT(mealID) AS mealID_count "
        "FROM CUSORDER_DETAIL "
        "GROUP BY mealID "
        "ORDER BY mealID_count DESC "
        "LIMIT 1"); // Retrieve only the top result

    sql::ResultSet* rs = ps->executeQuery();

    if (rs->next()) {
        int maxID = rs->getInt("mealID");
        int count = rs->getInt("mealID_count");
        return std::make_pair(maxID, count);
    }
    else {
        // Handle case where no results are found
        return std::make_pair(0, 0);  // Or throw an exception
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;
}

OrderDetail* OrderDetailManager::getDetail(int orderID, int mealID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER_DETAIL WHERE orderID = ? AND mealID = ?");
    ps->setInt(1, orderID);
    ps->setInt(2, mealID);

    sql::ResultSet* rs = ps->executeQuery();
    OrderDetail* orderDetail = nullptr;

    if (rs->next())
    {
        orderDetail = new OrderDetail();
        transform(rs, orderDetail);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return orderDetail;
}

OrderDetail* OrderDetailManager::getDetails(int orderID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSORDER_DETAIL WHERE orderID = ?");
    ps->setInt(1, orderID);

    sql::ResultSet* rs = ps->executeQuery();
    OrderDetail* orderDetail = nullptr;
    OrderDetail* head = nullptr;

    while (rs->next())
    {
        if (orderDetail == nullptr)
        {
            orderDetail = new OrderDetail();
            head = orderDetail;
        }
        else
        {
            orderDetail->next = new OrderDetail();
            orderDetail = orderDetail->next;
        }
        transform(rs, orderDetail);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;    //return head pointer, not the last pointer
}