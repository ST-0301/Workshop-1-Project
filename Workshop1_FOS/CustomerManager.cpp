#include "DatabaseConnection.h"
#include "CustomerManager.h"

void transform(sql::ResultSet* rs, Customer* customer)
{
    customer->customerID = rs->getInt("customerID");
    customer->f_name = rs->getString("f_name");
    customer->l_name = rs->getString("l_name");
    customer->gender = rs->getString("gender");
    customer->street = rs->getString("street");
    customer->postcode = rs->getInt("postcode");
    customer->city = rs->getString("city");
    customer->state = rs->getString("state");
    customer->tel_no = rs->getString("tel_no");
    customer->password = rs->getString("password");
}

int CustomerManager::addCustomer(Customer* customer)
{
    DatabaseConnection dbConn;
    try {
        sql::PreparedStatement* ps = dbConn.prepareStatement("INSERT INTO CUSTOMER (customerID, f_name, l_name, gender, street, postcode, city, state, tel_no, password) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        ps->setInt(1, customer->customerID);
        ps->setString(2, customer->f_name);
        ps->setString(3, customer->l_name);
        ps->setString(4, customer->gender);
        ps->setString(5, customer->street);
        ps->setInt(6, customer->postcode);
        ps->setString(7, customer->city);
        ps->setString(8, customer->state);
        ps->setString(9, customer->tel_no);
        ps->setString(10, customer->password);

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

int CustomerManager::getMaxCustomerID()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT MAX(customerID) AS maxID FROM CUSTOMER");

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

bool CustomerManager::loginCustomer(int customerID, const std::string& password)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSTOMER WHERE customerID = ? AND password = ?");
    ps->setInt(1, customerID);
    ps->setString(2, password);

    sql::ResultSet* rs = ps->executeQuery();

    bool loginSuccessful;
    if (rs->next())
        loginSuccessful = true; // Successful login
    else
        loginSuccessful = false; // Failure login
    
    rs->close();
    ps->close();

    delete rs;
    delete ps;
    
    return loginSuccessful;
}

int CustomerManager::updateCustomer(Customer* customer)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE CUSTOMER SET f_name = ?, l_name = ?, gender = ?, street = ?, postcode = ?, city = ?, state = ?, tel_no = ? WHERE customerID = ?");
    ps->setString(1, customer->f_name);
    ps->setString(2, customer->l_name);
    ps->setString(3, customer->gender);
    ps->setString(4, customer->street);
    ps->setInt(5, customer->postcode);
    ps->setString(6, customer->city);
    ps->setString(7, customer->state);
    ps->setString(8, customer->tel_no);
    ps->setInt(9, customer->customerID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int CustomerManager::updateCustomerPassword(int customerID, const std::string& password)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE CUSTOMER SET password = ? WHERE customerID = ?");
    ps->setString(1, password);
    ps->setInt(2, customerID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int CustomerManager::deleteCustomer(Customer* customer)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("DELETE FROM CUSTOMER WHERE customerID = ?");
    ps->setInt(1, customer->customerID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

Customer* CustomerManager::getCustomer(int customerID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSTOMER WHERE customerID = ?");
    ps->setInt(1, customerID);

    sql::ResultSet* rs = ps->executeQuery();
    Customer* customer = nullptr;

    if (rs->next())
    {
        customer = new Customer();
        transform(rs, customer);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return customer;
}

Customer* CustomerManager::getCustomers(const std::string& city)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSTOMER WHERE city = ?");
    ps->setString(1, city);

    sql::ResultSet* rs = ps->executeQuery();
    Customer* customer = nullptr;
    Customer* head = nullptr;

    while (rs->next())
    {
        if (customer == nullptr)
        {
            customer = new Customer();
            head = customer;
        }
        else
        {
            customer->next = new Customer();
            customer = customer->next;
        }
        transform(rs, customer);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Customer* CustomerManager::getCustomers()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM CUSTOMER");
    sql::ResultSet* rs = ps->executeQuery();
    Customer* customer = nullptr;
    Customer* head = nullptr;
    
    while (rs->next())
    {
        if (customer == nullptr)
        {
            customer = new Customer();
            head = customer;
        }
        else
        {
            customer->next = new Customer();
            customer = customer->next;
        }
        transform(rs, customer);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;    //return head pointer, not the last pointer
}