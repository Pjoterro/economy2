/*
 * @brief class simulating simple loan
 * 
 * Class responsibility is to simulate simple loan which does change its costs during its life cycle. 
 */

#ifndef LIB_LIGHTLOAN_LIGHTLOAN_H_
#define LIB_LIGHTLOAN_LIGHTLOAN_H_

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "loggerClass.h"

class loan {
protected:
	double startingValue {0}; ///< Value of the loan needed by the client (does not include costs)
	double valueLeft {0}; ///< Total value of loan left to be payed
	double cost {0}; ///< Costs of the loan based on starting value and interest rate
	double singleInstalmentValue {0}; ///< Value of single loan installment
	int startingInstalmentAmount {0}; ///< Amount of installments
	int instalmentAmountLeft {0}; ///< Amount of installments left to be payed
	/*!
	 * @brief Loan validation status
	 * 
	 * Loan validation status. Remains false if loan is not granted or is set 
	 * to true by bank.loanProcessingMethod when granting the loan. 
	 * If remains false then loan should be deleted.
	 * If set to true then loan should be check if it is ready for payment
	 */
	bool validated {false};
	/*!
	 * @brief Loan payment readiness status
	 * 
	 * After loan is validated, it is waiting to be ready to be payed. 
	 * This behavior is required for example when localBank accepted loan application 
	 * from localClient but does not have enough money in treasury to grant the loan.
	 */
	bool paymentReadiness {false};

public:
	/*!
	 * @brief Loan class constructor
	 * 
	 * @param loanValueArg should be supplied by **client**
	 * @param instalmentAmountArg should be supplied by **client**
	 * @param interestRateArg should be supplied by **bank**
	 */
	loan(double loanValueArg, int instalmentsAmountArg, double interestRateArg);

	virtual ~loan();

	/*!
	 * The value is equal to (startingValue + cost) / startingInstalmentAmount.
	 */
	void setSingleInstalmentValue();

	/*!
	 * @brief Method responsible for paying single loan installment.
	 * 
	 * This method updates loan variables when client (localBank or localClient)
	 *  is paying single installment.
	 *  @attention if last installment amount is payed then loan.validated and loan.paymentReadiness are set to false
	 */
	bool payAndUpdate();

	void validateLoan() {
		this->validated = true;
	};

	bool isLoanValid() {
		return this->validated;
	};

	double getStartingValue() {
		return this->startingValue;
	};

	double getValueLeft() {
		return this->valueLeft;
	};

	int getStartingInstalmentsAmount() {
		return this->startingInstalmentAmount;
	};

	int getInstalentsAmountLeft() {
		return this->instalmentAmountLeft;
	};

	double getSingleInstallmentValue() {
		return this->singleInstalmentValue;
	};

	double getCost() {
		return this->cost;
	};

	bool isReadyToBePayed() {
		return this->paymentReadiness;
	};

	void setAsReadyForPayment() {
		this->paymentReadiness = true;
	};

//	void coutEvent(std::string stringArg) {
//		std::cout << "Loan event: " << stringArg << std::endl;
//	};

	/*!
	 * @brief Simple logging method
	 * 
	 * Simple logging method. See logging:logEvent for more info
	 */
	void logEvent(std::string stringArg) {
		loggerClass::logEvent("Loan event: " + stringArg);
	};

};

#endif /* LIB_LIGHTLOAN_LIGHTLOAN_H_ */
