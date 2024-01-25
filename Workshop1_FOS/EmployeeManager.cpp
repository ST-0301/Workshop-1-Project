#include "DatabaseConnection.h"
#include "EmployeeManager.h"

void transform(sql::ResultSet* rs, Employee* employee)
{
    employee->employeeID = rs->getString("employeeID");
    employee->f_name = rs->getString("f_name");
    employee->l_name = rs->getString("l_name");
    employee->tel_no = rs->getString("tel_no");
    employee->street = rs->getString("street");
    employee->postcode = rs->getInt("postcode");
    employee->city = rs->getString("city");
    employee->state = rs->getString("state");
    employee->gender = rs->getString("gender");
    employee->position = rs->getString("position");
    employee->monthly_salary = rs->getDouble("monthly_salary");
    employee->password = rs->getString("password");
}

int EmployeeManager::addEmployee(Employee* employee)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("INSERT INTO EMPLOYEE (employeeID, f_name, l_name, tel_no, street, postcode, city, state, gender, position, monthly_salary, password) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    ps->setString(1, employee->employeeID);
    ps->setString(2, employee->f_name);
    ps->setString(3, employee->l_name);
    ps->setString(4, employee->tel_no);
    ps->setString(5, employee->street);
    ps->setInt(6, employee->postcode);
    ps->setString(7, employee->city);
    ps->setString(8, employee->state);
    ps->setString(9, employee->gender);
    ps->setString(10, employee->position);
    ps->setDouble(11, employee->monthly_salary);
    ps->setString(12, employee->password);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int EmployeeManager::getMaxEmployeeID(const std::string& position)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT MAX(CAST(SUBSTRING(employeeID, LENGTH(?)+2) AS UNSIGNED)) FROM EMPLOYEE WHERE position = ?");
    ps->setString(1, position.substr(0, 1));
    ps->setString(2, position);

    sql::ResultSet* rs = ps->executeQuery();

    int maxID = 0;
    if (rs->next())
    {
        maxID = rs->getInt(1);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return maxID;
}

bool EmployeeManager::loginEmployee(const std::string& employeeID, const std::string& password)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM EMPLOYEE WHERE employeeID = ? AND password = ?");
    ps->setString(1, employeeID);
    ps->setString(2, password);

    sql::ResultSet* rs = ps->executeQuery();

    bool loginStatus = rs->next();

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return loginStatus;
}

int EmployeeManager::updateEmployee(Employee* employee)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE EMPLOYEE SET f_name = ?, l_name = ?, tel_no = ?, street = ?, postcode = ?, city = ?, state = ?, gender = ?, monthly_salary = ? WHERE employeeID = ?");
    ps->setString(1, employee->f_name);
    ps->setString(2, employee->l_name);
    ps->setString(3, employee->tel_no);
    ps->setString(4, employee->street);
    ps->setInt(5, employee->postcode);
    ps->setString(6, employee->city);
    ps->setString(7, employee->state);
    ps->setString(8, employee->gender);
    ps->setDouble(9, employee->monthly_salary);
    ps->setString(10, employee->employeeID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int EmployeeManager::updateEmployeePassword(const std::string& employeeID, const std::string& password)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE EMPLOYEE SET password = ? WHERE employeeID = ?");
    ps->setString(1, password);
    ps->setString(2, employeeID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

int EmployeeManager::deleteEmployee(Employee* employee)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("DELETE FROM EMPLOYEE WHERE employeeID = ?");
    ps->setString(1, employee->employeeID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}

Employee* EmployeeManager::getEmployee(const std::string& employeeID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM EMPLOYEE WHERE employeeID = ?");
    ps->setString(1, employeeID);

    sql::ResultSet* rs = ps->executeQuery();
    Employee* employee = nullptr;

    if (rs->next())
    {
        employee = new Employee();
        transform(rs, employee);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return employee;
}

Employee* EmployeeManager::getEmployees(const std::string& position)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM EMPLOYEE WHERE position = ?");
    ps->setString(1, position);

    sql::ResultSet* rs = ps->executeQuery();
    Employee* employee = nullptr;
    Employee* head = nullptr;

    while (rs->next())
    {
        if (employee == nullptr)
        {
            employee = new Employee();
            head = employee;
        }
        else
        {
            employee->next = new Employee();
            employee = employee->next;
        }
        transform(rs, employee);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Employee* EmployeeManager::getEmployees(double maxAnnualSalary)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM EMPLOYEE WHERE (monthly_salary * 12) <= ?");
    ps->setDouble(1, maxAnnualSalary);

    sql::ResultSet* rs = ps->executeQuery();
    Employee* employee = nullptr;
    Employee* head = nullptr;

    while (rs->next())
    {
        if (employee == nullptr)
        {
            employee = new Employee();
            head = employee;
        }
        else
        {
            employee->next = new Employee();
            employee = employee->next;
        }
        transform(rs, employee);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Employee* EmployeeManager::getEmployees()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM EMPLOYEE");
    sql::ResultSet* rs = ps->executeQuery();
    Employee* employee = nullptr;
    Employee* head = nullptr;

    while (rs->next())
    {
        if (employee == nullptr)
        {
            employee = new Employee();
            head = employee;
        }
        else
        {
            employee->next = new Employee();
            employee = employee->next;
        }
        transform(rs, employee);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;    //return head pointer, not the last pointer
}