#ifndef MEAL_H
#define MEAL_H

class Meal
{
public:
	int mealID;
	std::string meal_name, category, meal_description;
	double meal_price;
	
	Meal* next;
};

#endif