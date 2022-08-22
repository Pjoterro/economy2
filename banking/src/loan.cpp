/*
 * lightLoan.cpp
 *
 *  Created on: 6 lip 2022
 *      Author: pjoter
 */

#include "banking/loan.h"
#include <iostream>

loan::loan(double loanValueArg, int instalmentsAmountArg, double interestRateArg) :
	startingValue(loanValueArg),
	valueLeft(loanValueArg*(1.0 + interestRateArg)),
	cost(loanValueArg*interestRateArg),
	startingInstalmentAmount(instalmentsAmountArg),
	instalmentAmountLeft(instalmentsAmountArg),
	validated (false),
	paymentReadiness (false)
{
	this->setSingleInstalmentValue();
}

void loan::setSingleInstalmentValue() {
	this->singleInstalmentValue = this->valueLeft / this->startingInstalmentAmount;
}

bool loan::payAndUpdate() {

	this->valueLeft = this->valueLeft - this->singleInstalmentValue;
	this->instalmentAmountLeft--;
	if (this->instalmentAmountLeft == 0) {
		this->validated = false;
		this->paymentReadiness = false;
	}
	return this->validated;
}

loan::~loan() {
	// TODO Auto-generated destructor stub
}

