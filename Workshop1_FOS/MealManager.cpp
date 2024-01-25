#include "DatabaseConnection.h"
#include "MealManager.h"

void transform(sql::ResultSet* rs, Meal* meal)
{
    meal->mealID = rs->getInt("mealID");
    meal->meal_name = rs->getString("meal_name");
    meal->category = rs->getString("category");
    meal->meal_description = rs->getString("meal_description");
    meal->meal_price = rs->getDouble("meal_price");
}

int MealManager::addMeal(Meal* meal)
{
    DatabaseConnection dbConn;
    
    sql::PreparedStatement* ps = dbConn.prepareStatement("INSERT INTO MEAL (mealID, meal_name, category, meal_description, meal_price) VALUES (?, ?, ?, ?, ?)");
    ps->setInt(1, meal->mealID);
    ps->setString(2, meal->meal_name);
    ps->setString(3, meal->category);
    ps->setString(4, meal->meal_description);
    ps->setDouble(5, meal->meal_price);

    int numRowsAffected = ps->executeUpdate();

    ps->close();
    
    delete ps;

    return numRowsAffected;
}

int MealManager::getMaxMealID()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT MAX(mealID) AS maxID FROM MEAL");

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

int MealManager::updateMeal(Meal* meal)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("UPDATE MEAL SET meal_name = ?, meal_description = ?, meal_price = ? WHERE mealID = ?");
    ps->setString(1, meal->meal_name);
    ps->setString(2, meal->meal_description);
    ps->setDouble(3, meal->meal_price);
    ps->setInt(4, meal->mealID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();
    
    delete ps;
    
    return numRowsAffected;
}

int MealManager::deleteMeal(Meal* meal)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("DELETE FROM MEAL WHERE mealID = ?");
    ps->setInt(1, meal->mealID);

    int numRowsAffected = ps->executeUpdate();

    ps->close();

    delete ps;

    return numRowsAffected;
}


Meal* MealManager::getMeal(int mealID)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM MEAL WHERE mealID = ?");
    ps->setInt(1, mealID);

    sql::ResultSet* rs = ps->executeQuery();
    Meal* meal = nullptr;

    if (rs->next())
    {
        meal = new Meal();
        transform(rs, meal);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return meal;
}

Meal* MealManager::getMeals(const std::string& category)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM MEAL WHERE category = ?");
    ps->setString(1, category);

    sql::ResultSet* rs = ps->executeQuery();
    Meal* meal = nullptr;
    Meal* head = nullptr;

    while (rs->next())
    {
        if (meal == nullptr)
        {
            meal = new Meal();
            head = meal;
        }
        else
        {
            meal->next = new Meal();
            meal = meal->next;
        }
        transform(rs, meal);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Meal* MealManager::getMeals(double maxPrice)
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM MEAL WHERE meal_price <= ?");
    ps->setDouble(1, maxPrice);

    sql::ResultSet* rs = ps->executeQuery();
    Meal* meal = nullptr;
    Meal* head = nullptr;

    while (rs->next())
    {
        if (meal == nullptr)
        {
            meal = new Meal();
            head = meal;
        }
        else
        {
            meal->next = new Meal();
            meal = meal->next;
        }
        transform(rs, meal);
    }

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;
}

Meal* MealManager::getMeals()
{
    DatabaseConnection dbConn;

    sql::PreparedStatement* ps = dbConn.prepareStatement("SELECT * FROM MEAL");
    sql::ResultSet* rs = ps->executeQuery();
    Meal* meal = nullptr;
    Meal* head = nullptr;

    while (rs->next())
    {
        if (meal == nullptr)
        {
            meal = new Meal();
            head = meal;
        }
        else
        {
            meal->next = new Meal();
            meal = meal->next;
        }
        transform(rs, meal);
    }
   // sortMeals(meal);

    rs->close();
    ps->close();

    delete rs;
    delete ps;

    return head;    //return head pointer, not the last pointer
}