#include <banking/loggerClass.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>

#include "constants.h"

#include "banking/bank.h"
#include "banking/client.h"
#include "banking/centralBank.h"
#include "banking/localBank.h"
#include "banking/localClient.h"
#include "banking/loan.h"

using namespace std;

int currentQueuedClientsCounter {0}; ///< needed for statistics
int totalLocalClientsCounter {0}; ///< needed for statistics

mutex mtx;

/*!
 * Method responsible for single Local Client instance (creation and payment method) 
 */
void startLocalClient(localBank* localBankPtr);
/*!
 * Method managing a single Local Bank instance (creating new Local Clients
 * and paying loan to Central Bank
 */
void startLocalBank(localBank* localBankPtr);
/*!
 * Method managing a Central Bank instance.
 */
void startCentralBank(centralBank* centralBankPtr);
/*!
 * @brief method cout'ing funy messages (which shows that program is running and not freezed)
 */

int main(int argc, char **argv) {
	cout << "App start" << endl;
	loggerClass::logInit();
	loggerClass::logEvent("------ START ------");

	srand(time(NULL)); // for true RNG

	centralBank centralBankInstance;
	thread centralBankThread{startCentralBank, &centralBankInstance};
	std::vector<thread> localBankThreadVector;
	localBank localBankInstance1("Local Bank 1", &centralBankInstance);
	localBankThreadVector.push_back(thread{startLocalBank, &localBankInstance1});
	localBank localBankInstance2("Local Bank 2", &centralBankInstance);
	localBankThreadVector.push_back(thread{startLocalBank, &localBankInstance2});
	localBank localBankInstance3("Local Bank 3", &centralBankInstance);
	localBankThreadVector.push_back(thread{startLocalBank, &localBankInstance3});
	//Closing the programm
	for(auto& t: localBankThreadVector) {
		t.join();
	}
	centralBankThread.join();
	centralBankInstance.logEvent("--- thread joined ---");
	//Log info
	localBankInstance1.logEndingInfo();
	localBankInstance2.logEndingInfo();
	localBankInstance3.logEndingInfo();
	centralBankInstance.logEndingInfo();
	loggerClass::logEvent("Total local clients: " + to_string(totalLocalClientsCounter++));
	loggerClass::logEvent("------ END ------");
	cout << "App end" << endl;
	return 0;
}

void startLocalClient(localBank* localBankPtr) {
	string name{};
	{
		unique_lock<mutex> ul(mtx);
		name = localBankPtr->getName() + "-Local Client-" + to_string(currentQueuedClientsCounter);
	}
	localClient* localClientPtr = new localClient(name, localBankPtr);
	localBankPtr->loanProcessingMethod(localClientPtr->getLoanPtr());
	while (localClientPtr->getLoanPtr()->isReadyToBePayed()) {
		localClientPtr->paymentMethod();
		this_thread::sleep_for(75ms);
	}
	{
		unique_lock<mutex> ul(mtx);
		currentQueuedClientsCounter--;
	}
	loggerClass::logEvent("Current active clients: " + to_string(currentQueuedClientsCounter));
	loggerClass::logEvent("Total active clients: " + to_string(totalLocalClientsCounter));
	delete localClientPtr;
	localClientPtr = nullptr;
}

void startLocalBank(localBank* localBankPtr) {
	boost::asio::thread_pool pool(ECONOMY2::MAX_NUMBER_OF_ACTIVE_CLIENTS);
	while (totalLocalClientsCounter < ECONOMY2::MAX_NUMBER_OF_GENERATED_CLIENTS
			|| currentQueuedClientsCounter > 0) {
		if (!localBankPtr->getLoanPtr()->isReadyToBePayed() && totalLocalClientsCounter < ECONOMY2::MAX_NUMBER_OF_GENERATED_CLIENTS) {
			{
				unique_lock<mutex> ul(mtx);
				totalLocalClientsCounter++;
				currentQueuedClientsCounter++;
			}
			boost::asio::post(pool, [localBankPtr](){ startLocalClient(localBankPtr); });
		}
		localBankPtr->paymentMethod();
		this_thread::sleep_for(50ms);
	}
	pool.join();
	localBankPtr->logEvent("Local Clients threads joined ---");
	this_thread::sleep_for(100ms);
	localBankPtr->logEvent("--- thread joined ---");
}


void startCentralBank(centralBank* centralBankPtr) {
	int i {0};
	while (totalLocalClientsCounter < ECONOMY2::MAX_NUMBER_OF_GENERATED_CLIENTS || currentQueuedClientsCounter > 0) {
		this_thread::sleep_for(500ms);
		std::cout << "." << flush;
		i++;
		if (i%20 == 0) {
			std::cout << "\n" << flush;
		}
	}
	std::cout << endl;
}
