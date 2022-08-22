/*!
 * @brief Bank class interface
 *
 * Bank class interface with both pure virtual and defined methods.
 * Child class should be able to process loans for clients.
 */

#ifndef LIB_BANK_BANK_H_
#define LIB_BANK_BANK_H_

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>

#include "dice.h"
#include "client.h"
#include "loan.h"
#include "../../constants.h"
#include "loggerClass.h"

class bank {

protected:
	std::string name; ///< Name of the bank
	double totalTreasury; ///< The total value of treasury, also the starting value
	double currentTreasury; ///< The current value of treasury
	double interestRate; ///< Starting value of interest rate
	dice diceBank; ///< Dice object which is in <loanProcessingMethod>()
	std::mutex bankMTX; ///< Mutex used in implementation of these methods:
	double totalLoans; ///< Variable needed for statistic
	double totalValidLoans; ///< Variable needed for statistic

public:
	/*!
	 * Bank constructor. diceBank value is set by BANK::DICE_SIZE
	 * @param nameArg is used as Bank instance name (for example in logging::logEvent() method)
	 * @param totalTreasuryArg used to set both **totalTreasury** and **currentTreasury**
	 * @param interestRateArg is used to set bank.interestRate
	 */
	bank(std::string nameArg, double totalTreasuryArg, double interestRateArg) :
		name(nameArg),
		totalTreasury(totalTreasuryArg),
		currentTreasury(totalTreasuryArg),
		interestRate(interestRateArg),
		diceBank(dice(BANK::DICE_SIZE)),
		totalLoans(0),
		totalValidLoans(0)
	{};

	virtual ~bank() {};

	/*!
	 * @brief Method responsible for processing loan applications.
	 * 
	 * This method is responsible for accepting or rejecting loan application 
	 * (which is parsed in argument).
	 * If Loan is accepted then loan::validateLoan() is called on itself. 
	 */
	virtual void loanProcessingMethod(loan *loanPtr) = 0;

	/*!
	 * @brief Method responsible for adjusting interest rate.
	 * 
	 * This method can either have some logic like in centralBank::adjustInterestRate
	 * or just return flat value like in localBank::adjustInterestRate
	 */
	virtual void adjustInterestRate() = 0;

	/*!
	 * @brief Method which receives single installment payment
	 * 
	 * Method responsible for receiving and processing single installment 
	 * payment of a valid loan.
	 */
	void receivePayment(loan* loanPtr) {
		std::lock_guard<std::mutex> lock_guard1(this->bankMTX);
		this->currentTreasury = this->currentTreasury + loanPtr->getSingleInstallmentValue();
		this->adjustInterestRate();
		this->logCurrentTreasuryRate();
		this->logCurrentInterestRate();
	};

	/*!
	 * @brief Method which allows to reduce current treasury 
	 * 
	 * This method reduces bank::currentTreasury 
	 * and updates interest rate by calling bank::adjustInterestRate().
	 * Needed for testing purpose, do not remove
	 */
	void withdraw(double amount) {
		if (this->currentTreasury >= amount) {
			this->currentTreasury = this->currentTreasury - amount; 
			this->adjustInterestRate();
		};
	};

	std::string getName() {
		return this->name;
	};

	double getCurrentTreasury() {
		return this->currentTreasury;
	};
	double getTotalTreasury() {
		return this->totalTreasury;
	};

	virtual double getInterestRate() {
		return this->interestRate;
	};

//	void coutEvent(std::string stringArg) {
//		std::cout << this->name << " event: " << stringArg << std::endl;
//	};

	/*!
	 * @brief Simple logging method
	 * 
	 * Simple logging method. See logging:logEvent for more info
	 */
	void logEvent(std::string stringArg) {
		loggerClass::logEvent(this->name + " event: " + stringArg);
	};

	/*!
	 * @brief Method to log info about treasury 
	 */
	void logCurrentTreasuryRate() {
		this->logEvent("treasury rate: "
				+ std::to_string(this->currentTreasury / this->totalTreasury * 100)
				+ "% (Total treasury: "
				+ std::to_string(this->totalTreasury)
				+ ")");
	};

	/*!
	 * @brief Method to log info about interest rate 
	 */
	void logCurrentInterestRate() { this->logEvent("interest rate: "
			+ std::to_string(this->getInterestRate() * 100)
			+ "%");
	};

	/*!
	 * @brief Method to log info about number of validated loans 
	 */
	void logLoansValidationRate() { this->logEvent("loans validation rate: "
			+ std::to_string((this->totalValidLoans / this->totalLoans) * 100)
			+ "%");
	};
	
	/*!
	 * @brief Method to log info at the end of the run
	 */
	void logEndingInfo() {
		this->logCurrentTreasuryRate();
		this->logCurrentInterestRate();
		this->logLoansValidationRate();
	};
};

#endif /* LIB_BANK_BANK_H_ */
