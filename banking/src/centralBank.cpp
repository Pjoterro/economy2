/*
 * centralBank.cpp
 *
 *  Created on: 15 cze 2022
 *      Author: pjoter
 */

#include <iostream>
#include <vector>
#include "banking/centralBank.h"
#include "../../constants.h"

centralBank::centralBank() :
		bank(CENTRAL_BANK::NAME, CENTRAL_BANK::STARTING_TREASURY, CENTRAL_BANK::INTEREST_TO_TREASURY_RATE[1][9])
{
	this->logEvent("created");
	this->adjustInterestRate();
}

void centralBank::loanProcessingMethod(loan *loanPtr) {
	std::lock_guard<std::mutex> lock_guard2(this->bankMTX);
	if (this->currentTreasury > loanPtr->getStartingValue()) {
		loanPtr->validateLoan();
		loanPtr->setAsReadyForPayment();
		this->totalLoans++;
		this->totalValidLoans++;
		this->logEvent("Central Bank loan granted");
		this->currentTreasury = this->currentTreasury - loanPtr->getStartingValue();
		this->totalTreasury = this->totalTreasury + loanPtr->getCost();
		this->adjustInterestRate();
	} else {
		this->totalLoans++;
		this->logEvent("Central Bank loan not granted");
	}
}

void centralBank::adjustInterestRate() {
	for (int i = 0; i < 10; i++){
		if( this->currentTreasury / this->totalTreasury <= CENTRAL_BANK::INTEREST_TO_TREASURY_RATE[0][i]) {
			this->interestRate = CENTRAL_BANK::INTEREST_TO_TREASURY_RATE[1][i];
			break;
		}
	}
	this->logEvent("interest rate updated");
	this->logCurrentInterestRate();
}

