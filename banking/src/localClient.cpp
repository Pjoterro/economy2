/*
 * localClient.cpp
 *
 *  Created on: 14 cze 2022
 *      Author: pjoter
 */

#include "banking/localClient.h"
#include "../../constants.h"

localClient::localClient(std::string nameArg, localBank* localBankPtr) :
	name(nameArg),
	masterBankPtr(localBankPtr)
{
	this->totalInstalmentsAmount = this->generateTotalInstalmentsAmount();
	this->totalLoanValue = this->generateTotalLoanValue();
	this->generateLoan();
	this->logEvent("created");
}

void localClient::paymentMethod() {
	if (this->clientLoanPtr->isReadyToBePayed()) {
		this->masterBankPtr->receivePayment(clientLoanPtr);
		this->clientLoanPtr->payAndUpdate();
	}
}

double localClient::generateTotalLoanValue() {
	std::unique_lock<std::mutex> ul(mtx);
	int roll = diceClient.roll();
	this->logEvent("generating loan total value - dice roll: " + std::to_string(roll));
	return roll * LOCAL_CLIENT::LOAN_VALUE_MULTIPLIER;
}

int localClient::generateTotalInstalmentsAmount() {
	std::unique_lock<std::mutex> ul(mtx);
	int roll = diceClient.roll();
	this->logEvent("generating loan total installments amount - dice roll: " + std::to_string(roll));
	if (roll < LOCAL_CLIENT::MINIMAL_INSTALLMENT_AMOUNT) {
		roll += LOCAL_CLIENT::MINIMAL_INSTALLMENT_AMOUNT;
	}
	return roll;
}

void localClient::generateLoan() {
	this->clientLoanPtr = new loan(
			this->totalLoanValue,
			this->totalInstalmentsAmount,
			this->masterBankPtr->getInterestRate());
}

localClient::~localClient() {
	delete this->clientLoanPtr;
	this->clientLoanPtr = nullptr;
}

