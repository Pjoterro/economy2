/*
 * localBank.cpp
 *
 *  Created on: 14 cze 2022
 *      Author: pjoter
 */

#include <iostream>
#include <mutex>
#include <vector>
#include <iostream>
#include "banking/localBank.h"
#include "../../constants.h"

localBank::localBank(std::string nameArg, centralBank* centralBankPtr) :
		bank(nameArg, LOCAL_BANK::STARTING_TREASURY, LOCAL_BANK::INTEREST_RATE),
		client(),
		masterBankPtr(centralBankPtr),
		neededAmountThreshold(LOCAL_BANK::THRESHOLD_FOR_LOAN),
		amountNeededForLoans(0)
{
	this->clientLoanPtr = new loan(0.0, 0, 0.0);
	this->logEvent("created");
}

localBank::~localBank() {
	delete this->clientLoanPtr;
	this->clientLoanPtr = nullptr;
}

void localBank::loanProcessingMethod(loan *loanPtr) {
	std::lock_guard<std::mutex> lock_guard2(this->bankMTX);
	if (this->loanValidationMethod(loanPtr)) {
		if (this->currentTreasury > loanPtr->getStartingValue()) {
			loanPtr->setAsReadyForPayment();
			this->currentTreasury = this->currentTreasury - loanPtr->getStartingValue();
			this->totalTreasury = this->totalTreasury + loanPtr->getCost();
			this->totalLoans++;
			this->totalValidLoans++;
			this->logEvent("loan granted from treasury");
		} else {
			this->waitingLoans.push_back(loanPtr);
			this->amountNeededForLoans = this->amountNeededForLoans + loanPtr->getStartingValue();
			this->totalLoans++;
			this->totalValidLoans++;
			this->logEvent("not enough money in treasury, adding loan to waiting vector");
			if (this->amountNeededForLoans >= LOCAL_BANK::THRESHOLD_FOR_LOAN) {
				this->applyForLoan();
			}
		}
	} else {
		this->totalLoans++;
		this->logEvent("loan not granted");
	}
}

bool localBank::loanValidationMethod(loan *loanPtr) {
	return (this->diceBank.roll() + static_cast<int>(loanPtr->getStartingValue()) % 3)  >= 6 && !this->clientLoanPtr->isLoanValid();
}

void localBank::paymentMethod() {
	if (this->clientLoanPtr->isReadyToBePayed()) {
		this->logEvent("Central Bank loan installment payment");
		this->logCurrentTreasuryRate();
		this->currentTreasury = this->currentTreasury - this->clientLoanPtr->getSingleInstallmentValue();
		this->masterBankPtr->receivePayment(clientLoanPtr);
		this->clientLoanPtr->payAndUpdate();
	}
}

double localBank::generateTotalLoanValue() {
	std::lock_guard<std::mutex> lock_guard1(this->bankMTX);
	return this->amountNeededForLoans;
}

void localBank::generateLoan() {
	delete this->clientLoanPtr;
	this->clientLoanPtr = new loan(
		this->amountNeededForLoans,
		this->generateTotalInstalmentsAmount(),
		this->masterBankPtr->getInterestRate());
}

void localBank::applyForLoan() {
	this->generateLoan();
	this->logEvent("Applying for Central Bank loan");
	this->masterBankPtr->loanProcessingMethod(this->clientLoanPtr);
	if (this->clientLoanPtr->isLoanValid()) {
		this->logEvent("Central Bank loan granted");
		this->logCurrentTreasuryRate();
		this->currentTreasury = this->currentTreasury + this->amountNeededForLoans;
		this->totalTreasury = this->totalTreasury + (this->amountNeededForLoans * this->interestRate);
		for (auto loan: waitingLoans) {
			this->totalLoans++;
			this->totalValidLoans++;
			loan->setAsReadyForPayment();
			this->currentTreasury = this->currentTreasury - loan->getStartingValue();
		}
		this->waitingLoans.clear();
		this->amountNeededForLoans = 0;
	} else {
		this->logEvent("Central Bank did not grant loan");
	}
}
