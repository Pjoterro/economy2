/*
 * @brief Central Bank class
 * 
 * Class representing Central Bank model
 */

#ifndef LIB_CENTRALBANK_CENTRALBANK_H_
#define LIB_CENTRALBANK_CENTRALBANK_H_

#include "bank.h"

class centralBank : public bank {
protected:

public:
	/*!
	 * @brief central bank class constructor
	 * 
	 * This constructor does not accept any parameters. To change either name, starting trasury or
	 * interest rate check constants.h
	 */
	centralBank();

	~centralBank() {};

	/*!
	 * @brief Loan processing method 
	 * 
	 * Implementation od bank::loanProcessingMethod for Central Bank class. If Central Bank has enough
	 * treasury the all loans are granted. Loans are not granted only where there is not enough money in treasury 
	 */
	void loanProcessingMethod(loan *loanPtr) override;

	/*!
	 * @brief Method to adjust interest rate based on treasury to total treasury ratio.
	 * 
	 * Method adjust interest rate based on (current treasury / total treasury) ratio.
	 * See CENTRAL_BANK::INTEREST_TO_TREASURY_RATE
	 */
	void adjustInterestRate() override;

};

#endif /* LIB_CENTRALBANK_CENTRALBANK_H_ */
