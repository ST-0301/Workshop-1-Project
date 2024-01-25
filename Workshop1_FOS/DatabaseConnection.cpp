#include "DatabaseConnection.h"

//DatabaseConnection::DatabaseConnection()
//{
//    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
//    connection = driver->connect("tcp://127.0.0.1:3306", "root", "");
//
//    connection->setSchema("dbFOS_test");
//}

DatabaseConnection::DatabaseConnection()
{
    try 
    {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        connection = driver->connect("tcp://127.0.0.1:3306", "root", "");

        if (connection == nullptr) 
        {
            // Handle connection failure
            throw std::runtime_error("Failed to connect to the database.");
        }

        connection->setSchema("dbFOS_workshop1");
    }
    catch (const std::exception& e) 
    {
        // Handle exceptions (e.g., print error message or log)
        std::cerr << "Error during database connection: " << e.what() << std::endl;
    }
}

DatabaseConnection::~DatabaseConnection()
{
	connection->close();

	delete connection;
}

sql::PreparedStatement* DatabaseConnection::prepareStatement(std::string query)
{
    if (!connection || connection->isClosed()) 
    {
        // Handle the case where the connection is not valid
        throw std::logic_error("Invalid database connection");
    }

    return connection->prepareStatement(query);
}