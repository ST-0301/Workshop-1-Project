#include "DatabaseConnection.h"
#include "DeliveryManager.h"

void transform(sql::ResultSet* rs, Delivery* delivery)
{
    delivery->deliveryID = rs->getInt("deliveryID");
    delivery->delivered_date_time = rs->getString("delivered_date_time");
    delivery->delivery_status = rs->getString("delivery_status");
    delivery->delivery_street = rs->getString("delivery_street");
    delivery->delivery_postcode = rs->getInt("delivery_postcode");
    delivery->delivery_city = rs->getString("delivery_city");
    delivery->delivery_state = rs->getString("delivery_state");
    delivery->orderID = rs->getInt("orderID");
    delivery->employeeID = rs->getString("employeeID");
}

int DeliveryManager::addDelivery(Delivery* delivery)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("INSERT INTO DELIVERY (deliveryID, delivery_status, delivery_street, delivery_postcode, delivery_city, delivery_state, orderID) VALUES (?, ?, ?, ?, ?, ?, ?)");
    ps->setInt(1, delivery->deliveryID);
    ps->setString(2, delivery->delivery_status);
    ps->setString(3, delivery->delivery_street);
    ps->setInt(4, delivery->delivery_postcode);
    ps->setString(5, delivery->delivery_city);
    ps->setString(6, delivery->delivery_state);
    ps->setInt(7, delivery->orderID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int DeliveryManager::getMaxDeliveryID()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT MAX(deliveryID) AS maxID FROM DELIVERY");

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

int DeliveryManager::updateDelivery(Delivery* delivery)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE DELIVERY SET delivery_street = ?, delivery_postcode = ?, delivery_city = ?, delivery_state = ? WHERE deliveryID = ?");
    ps->setString(1, delivery->delivery_street);
    ps->setInt(2, delivery->delivery_postcode);
    ps->setString(3, delivery->delivery_city);
    ps->setString(4, delivery->delivery_state);
    ps->setInt(5, delivery->deliveryID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();
    delete ps;

    return numRowsAffected;
}

int DeliveryManager::assignDelivery(Delivery* delivery)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE DELIVERY SET delivery_status = 'Assigned', employeeID = ? WHERE deliveryID = ?");
    ps->setString(1, delivery->employeeID);
    ps->setInt(2, delivery->deliveryID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();
    delete ps;

    return numRowsAffected;
}

int DeliveryManager::completeDelivery(Delivery* delivery)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE DELIVERY SET delivery_status = 'Completed', delivered_date_time = NOW(), employeeID = ? WHERE deliveryID = ?");
    ps->setString(1, delivery->employeeID);
    ps->setInt(2, delivery->deliveryID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();
    delete ps;

    return numRowsAffected;
}

int DeliveryManager::deleteDelivery(int orderID)
{
    try {
        DatabaseConnection dbConn;

        sql::PreparedStatement* ps = dbConn.prepareStatement("DELETE FROM DELIVERY WHERE orderID = ?");
        ps->setInt(1, orderID);

        int numRowsAffected = ps->executeUpdate();

        ps->close();
        delete ps;

        return numRowsAffected;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
    }
}

Delivery* DeliveryManager::getDelivery(int deliveryID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM DELIVERY WHERE deliveryID = ?");
    ps->setInt(1, deliveryID);

    sql::ResultSet* rs = ps->executeQuery();
    Delivery* delivery = nullptr;

    if (rs->next())
    {
        delivery = new Delivery();
        transform(rs, delivery);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return delivery;
}

Delivery* DeliveryManager::getDeliveryByOrderID(int orderID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM DELIVERY WHERE orderID = ?");
    ps->setInt(1, orderID);

    sql::ResultSet* rs = ps->executeQuery();
    Delivery* delivery = nullptr;

    if (rs->next())
    {
        delivery = new Delivery();
        transform(rs, delivery);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return delivery;
}

Delivery* DeliveryManager::getDeliveries(int orderID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM DELIVERY WHERE orderID = ?");
    ps->setInt(1, orderID);
    
    sql::ResultSet* rs = ps->executeQuery();
    Delivery* delivery = nullptr;
    Delivery* head = nullptr;

    while (rs->next())
    {
        if (delivery == nullptr)
        {
            delivery = new Delivery();
            head = delivery;
        }
        else
        {
            delivery->next = new Delivery();
            delivery = delivery->next;
        }
        transform(rs, delivery);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Delivery* DeliveryManager::getDeliveries(const std::string& deliveryStatus)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM DELIVERY WHERE delivery_status = ?");
    ps->setString(1, deliveryStatus);

    sql::ResultSet* rs = ps->executeQuery();
    Delivery* delivery = nullptr;
    Delivery* head = nullptr;

    while (rs->next())
    {
        if (delivery == nullptr)
        {
            delivery = new Delivery();
            head = delivery;
        }
        else
        {
            delivery->next = new Delivery();
            delivery = delivery->next;
        }
        transform(rs, delivery);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Delivery* DeliveryManager::getDeliveries()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM DELIVERY");
    sql::ResultSet* rs = ps->executeQuery();
    Delivery* delivery = nullptr;
    Delivery* head = nullptr;

    while (rs->next())
    {
        if (delivery == nullptr)
        {
            delivery = new Delivery();
            head = delivery;
        }
        else
        {
            delivery->next = new Delivery();
            delivery = delivery->next;
        }
        transform(rs, delivery);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;    //return head pointer, not the last pointer
}