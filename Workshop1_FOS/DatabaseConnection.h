#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <mysql/jdbc.h>

class DatabaseConnection
{
private:
    sql::Connection* connection;

public:
    DatabaseConnection();
    ~DatabaseConnection();
    sql::PreparedStatement* prepareStatement(std::string query);
};

#endif