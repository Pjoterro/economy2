/*
 * @brief Local Bank class
 * 
 * Class representing Local Bank model. 
 */

#ifndef LIB_LOCALBANK_LOCALBANK_H_
#define LIB_LOCALBANK_LOCALBANK_H_
#include <vector>
#include "bank.h"
#include "client.h"
#include "centralBank.h"

class localBank : public bank, public client {

protected:
	
	double neededAmountThreshold; ///< After reaching this amount Local Bank asks Central Bank for loan 
	/*!
	 * @brief sum of all validated loans
	 * 
	 * Sum of all validated loans for which bank does not have enough treasury. 
	 * @attention this value later becomes loan.startingValue of Local Bank loan
	 */
	double amountNeededForLoans;
	centralBank* masterBankPtr; ///< Pointer to Central Bank
	std::vector<loan*> waitingLoans; ///< Vector of validated loans 

public:
	localBank();
	localBank(std::string nameArg, centralBank* masterBankPtr);

	~localBank();

	/*!
	 * @brief Loan processing method is responsible for accepting or declining loan applications
	 * 
	 * Loan processing method at first randomly decide if loan should be validated or not see 
	 * localBank::loanValidationMethod() ). 
	 * 
	 * If loan is validated and Local Bank has enough treasury then loan is ready to be payed
	 * and Local Client can start paying installments.
	 * 
	 * If loan was not granted (dice roll was too low) then loan is not granted and Local Client do nothing.
	 */
	void loanProcessingMethod(loan *loanPtr) override;

	/*!
	 * @brief Applying for new loan
	 * 
	 * Generating and applying for new loan. At first localBank::generateLoan() is called, and later
	 * Local Bank apply to Central Bank with new loan. If granted then localBank.currentTreasury is increased
	 * and all loans from localBank.waitingLoans are granted. 
	 * @attention If loan is not granted then the vector is **NOT** cleared and Local Bank will ask about
	 * loan later when next Local Client will show up
	 */
	void applyForLoan();

	/*!
	 * @brief Method deciding whether Local Client loan should be validated or not
	 * 
	 * This method is using dice::roll() method to determine the outcome
	 */
	virtual bool loanValidationMethod(loan *loanPtr);

	/*!
	 * @brief Local Bank payment method
	 * 
	 * Payment method reduces current treasure by one Local Bank loan 
	 * installment amount and updates the loan data
	 */
	void paymentMethod() override;
	
	/*!
	 * @brief Returns localBank.amountNeededForLoans
	 */
	double generateTotalLoanValue() override;

	/*!
	 * @brief Generating new loan
	 * 
	 * Local Bank generate new loan pointer based on needed loana mount, 
	 * instalments amount and Central Bank interest rate. 
	 */
	void generateLoan() override;

	/*!
	 * @brief Generating loan installments amount
	 * 
	 * Local Bank has fixed amount of installment amount set to 10
	 */
	int generateTotalInstalmentsAmount() override {
		return 10;
	};

	/*!
	 * @brief Method to adjust interest rate. 
	 * 
	 * This method updates interest rate based on treasury rate. For Local Bank it always returns fixed
	 * which is LOCAL_BANK::INTEREST_RATE
	 */
	void adjustInterestRate() override {
		this->interestRate = LOCAL_BANK::INTEREST_RATE;
	};

	double getInterestRate() override {
		return (this->interestRate + this->masterBankPtr->getInterestRate());
	};

	/*!
	 * @brief Method which determines if Local Bank should ask for loan
	 * 
	 * This method checks if value of all loans which were validated but not granted 
	 * is bigger than fixed value LOCAL_BANK::THRESHOLD_FOR_LOAN
	 */
	bool shouldApplyForLoan() {
		std::lock_guard<std::mutex> lock_guard2(this->bankMTX);
		return (this->amountNeededForLoans >= this->neededAmountThreshold);
	};

	/*!
	 * @brief Method increasing treasury when Central Bank grants loan to Local Bank
	 */
	void increaseTreasury() {
		std::lock_guard<std::mutex> lock_guard2(this->bankMTX);
		this->currentTreasury = this->currentTreasury + this->amountNeededForLoans;
		this->totalTreasury = this->totalTreasury + this->amountNeededForLoans;
		this->amountNeededForLoans = 0;
	};

	centralBank* getMasterBankPtr() {
		return masterBankPtr;
	};

	std::string getName() {
		return this->name;
	};

	double getCurrentTreasury() {
		return this->currentTreasury;
	};

	/*!
	 * @brief setter needed for tests
	 * @warning **DO NOT REMOVE**
	 */
	void setAmountNeededForLoans(double newAmount) {
		this->amountNeededForLoans = newAmount;
	};
};

#endif /* LIB_LOCALBANK_LOCALBANK_H_ */
