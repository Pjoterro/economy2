/*!
 * @brief Client class interface
 *
 * Client class interface with both pure virtual and defined methods.
 * Child class should be able to generate and pay loans.
 */

#ifndef LIB_CLIENT_CLIENT_H_
#define LIB_CLIENT_CLIENT_H_

#include "loan.h"
#include "dice.h"
#include "bank.h"
#include "../../constants.h"

class client {

protected:
	double totalLoanValue; ///< Amount needed for loan
	int totalInstalmentsAmount; ///< Number of installments amount 
	loan* clientLoanPtr; ///< Pointer to the client's loan
	dice diceClient; ///< Client's dice (localCLient needs it to generate both loan value and installments amount)

public:
	/*!
	 * Client constructor. The dice size is set by CLIENT::DICE_SIZE
	 */
	client() :
		diceClient(dice(CLIENT::DICE_SIZE)),
		totalLoanValue(0.0),
		totalInstalmentsAmount(0),
		clientLoanPtr(nullptr)
	{}

	virtual ~client() {
		this->clientLoanPtr = nullptr;
	};

	loan* getLoanPtr() {
		return clientLoanPtr;
	};

	/*!
	 * Payment method is responsible for letting bank instance know to receive money
	 * and to update loan
	 */
	virtual void paymentMethod() = 0;

	/*!
	 * Method responsible for generating loan amount
	 */
	virtual double generateTotalLoanValue() = 0;

	/*!
	 * @brief Generating loan based on loan value, installments amount and master bank interest rate
	 */
	virtual void generateLoan() = 0;

	/*!
	 * @brief Generating total loan installments amount
	 */
	virtual int generateTotalInstalmentsAmount() = 0;

};

#endif /* LIB_CLIENT_CLIENT_H_ */
