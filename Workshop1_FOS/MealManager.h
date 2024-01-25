#include "Meal.h"

#ifndef MEALMANAGER_H
#define MEALMANAGER_H

class MealManager
{
public:
	int addMeal(Meal* meal);
	int getMaxMealID();
	int updateMeal(Meal* meal);
	int deleteMeal(Meal* meal);

	Meal* getMeal(int mealID);
	Meal* getMeals(const std::string& category);
	Meal* getMeals(double maxPrice);

	Meal* getMeals();
};

#endif