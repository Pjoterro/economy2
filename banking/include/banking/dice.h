/*
 * @brief Class to imitate dice and dice roll
 *
 * Class which aims to imitate **n**-sided dice and method for rolling it
 */

#ifndef LIB_DICE_DICE_H_
#define LIB_DICE_DICE_H_

class dice {

private:
	int size; ///< number if sides of the dice

public:

	dice();

	/*!
	 * @brief Creating dice with number of sides parsed as a parameter
	 * @param sizenumber of sides of the dice
	 */
	dice(int size);

	/*!
	 * Method returns integer number between 1 and **dice.size** based on **rand()** 
	 */
	int roll();

	virtual ~dice();
};

#endif /* LIB_DICE_DICE_H_ */
