/*
 * @brief Local Client class
 * 
 * Class imitating Local CLient behavior
 */

#ifndef LIB_LOCALCLIENT_LOCALCLIENT_H_
#define LIB_LOCALCLIENT_LOCALCLIENT_H_

#include <mutex>
#include "client.h"
#include "localBank.h"

class localClient : public client {

protected:
	std::string name; ///< Name needed for logs
	localBank* masterBankPtr; ///< Pointer to Local Bank
	std::mutex mtx; ///< Mutex

public:
	/*!
	 * @Constructor
	 * 
	 * Creating new local CLient also generate and create new loan
	 * @see localClient::generateTotalInstalmentsAmount()
	 * @see localClient::generateTotalLoanValue()
	 * @see localClient::generateLoan()
	 */
	localClient(std::string nameArg, localBank* localBankPtr);

	~localClient();

	localBank* getMasterBankPtr() {
		return masterBankPtr;
	};

	void coutEvent(std::string stringArg) {
		std::cout << this->name << " event: " << stringArg << std::endl;
	};

	void logEvent(std::string stringArg) {
		loggerClass::logEvent(this->name + " event: " + stringArg);
	};

	/*!
	 * @Local Client payment method
	 * 
	 * Local Client is the point where money is simulation in produced. 
	 * Local Client is adding money to the simulation by paying loan costs
	 */
	void paymentMethod() override;

	/*!
	 * @brief Generating loan value by rolling dice
	 */
	double generateTotalLoanValue() override;

	/*!
	 * @brief Generating installments amount by rolling dice
	 */
	int generateTotalInstalmentsAmount() override;

	/*!
	 * @brief Generating new loan based on client data and Local Bank interest rate
	 */
	void generateLoan() override;
};

#endif /* LIB_LOCALCLIENT_LOCALCLIENT_H_ */
