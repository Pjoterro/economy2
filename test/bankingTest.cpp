/*
 * @brief Test class 
 * 
 * Test class created using gtest and gmock. 
 */

#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "banking/bank.h"
#include "banking/client.h"
#include "banking/centralBank.h"
#include "banking/localBank.h"
#include "banking/localClient.h"
#include "banking/loan.h"
#include "banking/dice.h"
#include "../constants.h"

/*!
 * @brief Mock Class of Central Bank
 */
class mockCentralBank : public centralBank {
public:
	mockCentralBank();
	MOCK_METHOD(double, getInterestRate, ());
};

mockCentralBank::mockCentralBank() {};

/*!
 * @brief Mock Class of Local Bank
 * 
 * Contains mock of localBank::generateTotalLoanValue() and localBank::LoanValidationMethod().
 */
class mockLocalBank : public localBank {
public:
	mockLocalBank(std::string nameArg, centralBank* centralBankPtr);
	MOCK_METHOD(double, generateTotalLoanValue, ());
	MOCK_METHOD(bool, loanValidationMethod, (loan*), (override));
};

mockLocalBank::mockLocalBank(std::string nameArg, centralBank* centralBankPtr) :
	localBank(nameArg, centralBankPtr) {}

/*!
 * @brief Mock Class of Local CLient
 * 
 * Contains mock of localCLient::generateTotalLoanValue() and localCLient::generateTotalInstallmentAmount().
 */
class mockLocalClient : public localClient {
public:
	mockLocalClient(std::string nameArg, localBank* localBankPtr);
	MOCK_METHOD(double, generateTotalLoanValue, ());
	MOCK_METHOD(int, generateTotalInstalmentsAmount, ());
};

mockLocalClient::mockLocalClient(std::string nameArg, localBank* localBankPtr) :
		localClient(nameArg, localBankPtr) {}

//========== DICE: dice.h ==========
const int numberOfRolls {100'000}; ///< number of rolls for DiceTest

/*!
 * @brief Test of Bank Dice
 * 
 * Test checking if Bank Dice behaves as intended (min value is 1 and max is BANK::DICE_SIZE)
 */
TEST(DiceTest, BankDice) {
	const int sides{BANK::DICE_SIZE};
	dice dice20d = dice(sides);
	int max {1};
	int min {sides};
	for (int i = 0; i < numberOfRolls; i++) {
		int currentRoll = dice20d.roll();
		if(currentRoll > max) {
			max = currentRoll;
		}
		if (currentRoll < min) {
			min = currentRoll;
		}
	};
	EXPECT_LE(max, sides);
	EXPECT_GE(min, 1);
}

/*!
 * @brief Test of Client Dice
 * 
 * Test checking if Client Dice behaves as intended (min value is 1 and max is CLIENT::DICE_SIZE)
 */
TEST(DiceTest, ClientDice) {
	const int sides{CLIENT::DICE_SIZE};
	dice dice100d = dice(sides);
	int max {1};
	int min {sides};
	for (int i = 0; i < numberOfRolls; i++) {
		int currentRoll = dice100d.roll();
		if(currentRoll > max) {
			max = currentRoll;
		}
		if (currentRoll < min) {
			min = currentRoll;
		}
	};
	EXPECT_LE(1, min);
	EXPECT_GE(sides, max);
}

/*!
 * @brief Test of 100d dice
 * 
 * Test checking if 100d dice behaves as intended (min value is 1 and max is 100)
 */
TEST(DiceTest, Dice100d) {
	const int sides{100};
	dice dice100d = dice(sides);
	std::vector<int> rollResults;
	int max {1};
	int min {sides};
	for (int i = 0; i < numberOfRolls; i++) {
		int currentRoll = dice100d.roll();
		if(currentRoll > max) {
			max = currentRoll;
		}
		if (currentRoll < min) {
			min = currentRoll;
		}
		rollResults.push_back(currentRoll);
	};
	EXPECT_LE(1, min);
	EXPECT_GE(sides, max);
}

//========== LOAN: loan.h ==========
const double loanAmount{10'000}; ///< Loan value for LoanTest
const int loanInstalments{10}; ///< Installments amount for LoanTest
const double loanInterest{0.05}; ///< Loan interest rate for LoanTest

/*!
 * @brief Basic loan creation test
 */
TEST(LoanTest, LoanCreator) {
	loan loanInstance(loanAmount, loanInstalments, loanInterest);
	EXPECT_DOUBLE_EQ(loanAmount, loanInstance.getStartingValue());
	EXPECT_EQ(loanInstalments, loanInstance.getStartingInstalmentsAmount());
	EXPECT_DOUBLE_EQ(loanAmount * loanInterest, loanInstance.getCost());
	EXPECT_DOUBLE_EQ(loanAmount * (1 + loanInterest) / loanInstalments, loanInstance.getSingleInstallmentValue());
	EXPECT_FALSE(loanInstance.isLoanValid());
	EXPECT_FALSE(loanInstance.isReadyToBePayed());
}

/*!
 * @brief Loan validation test
 */
TEST(LoanTest, LoanValidation) {
	loan loanInstance(loanAmount, loanInstalments, loanInterest);
	EXPECT_FALSE(loanInstance.isLoanValid());
	EXPECT_FALSE(loanInstance.isReadyToBePayed());
	loanInstance.validateLoan();
	EXPECT_TRUE(loanInstance.isLoanValid());
	EXPECT_FALSE(loanInstance.isReadyToBePayed());
	loanInstance.setAsReadyForPayment();
	EXPECT_TRUE(loanInstance.isLoanValid());
	EXPECT_TRUE(loanInstance.isReadyToBePayed());
}

/*!
 * @brief Loan payment test
 */
TEST(LoanTest, LoanPayment) {
	loan loanInstance(loanAmount, loanInstalments, loanInterest);
	loanInstance.validateLoan();
	EXPECT_TRUE(loanInstance.payAndUpdate());
	while(loanInstance.isLoanValid()) {
		loanInstance.payAndUpdate();
	}
	EXPECT_FALSE(loanInstance.payAndUpdate());
}

//========== BANK: bank.h; centralBank.h; localBank.h ==========
/*!
 * @brief Central Bank basic test
 * 
 * Testing Central Bank creation and updates on interest rate
 */
TEST(BankTest, CentralBankInstanceBasicFunctionality) {
	const int steps{10};
	centralBank centralBankInstance;
	EXPECT_EQ(CENTRAL_BANK::NAME, centralBankInstance.getName());
	EXPECT_EQ(CENTRAL_BANK::STARTING_TREASURY, centralBankInstance.getCurrentTreasury());
	for (int i = 0; i < steps; i++) {
		EXPECT_EQ(
				CENTRAL_BANK::INTEREST_TO_TREASURY_RATE[1][9 - i],
				centralBankInstance.getInterestRate()
				);
		centralBankInstance.withdraw(CENTRAL_BANK::STARTING_TREASURY/steps);
	}
	EXPECT_EQ(0, centralBankInstance.getCurrentTreasury());
}

/*!
 * @brief Local Bank basic test
 * 
 * Testing Local Bank creation, withdraws and interest rate 
 */
TEST(BankTest, LocalBankInstanceBasicFunctionality) {
	double MCBInterestRate1 {0.01}; ///< Mock value #1
	double MCBInterestRate2 {0.05}; ///< Mock value #2

	mockCentralBank mockCentralBankInstance;
	localBank localBankInstance("Local Bank", &mockCentralBankInstance);
	EXPECT_EQ("Local Bank", localBankInstance.getName());
	ASSERT_DOUBLE_EQ(LOCAL_BANK::STARTING_TREASURY, localBankInstance.getCurrentTreasury());
	EXPECT_CALL(mockCentralBankInstance, getInterestRate()).Times(3).WillRepeatedly(testing::Return(MCBInterestRate1));
	EXPECT_DOUBLE_EQ(
			MCBInterestRate1 + LOCAL_BANK::INTEREST_RATE,
			localBankInstance.getInterestRate()
			);
	localBankInstance.withdraw(LOCAL_BANK::STARTING_TREASURY * 0.5);
	EXPECT_DOUBLE_EQ(
			MCBInterestRate1 + LOCAL_BANK::INTEREST_RATE,
			localBankInstance.getInterestRate()
			);
	localBankInstance.withdraw(LOCAL_BANK::STARTING_TREASURY * 0.6);
	ASSERT_DOUBLE_EQ(
			LOCAL_BANK::STARTING_TREASURY * 0.5,
			localBankInstance.getCurrentTreasury()
			);
	EXPECT_DOUBLE_EQ(
			MCBInterestRate1 + LOCAL_BANK::INTEREST_RATE,
			localBankInstance.getInterestRate()
			);
	EXPECT_CALL(mockCentralBankInstance, getInterestRate()).Times(1).WillOnce(testing::Return(MCBInterestRate2));
	EXPECT_DOUBLE_EQ(
			MCBInterestRate2 + LOCAL_BANK::INTEREST_RATE,
			localBankInstance.getInterestRate()
			);
}

/*!
 * @brief Central Bank processing normal loan (value smaller then CB treasury)
 */
TEST(BankTest, CentralBank_LoanProcessingMethod_NormalLoan) {
	centralBank centralBankInstance;
	loan loanInstance1(CENTRAL_BANK::STARTING_TREASURY * 0.25, 10, centralBankInstance.getInterestRate());
	double expectedCurrentTreasury = CENTRAL_BANK::STARTING_TREASURY + CENTRAL_BANK::STARTING_TREASURY*0.25*centralBankInstance.getInterestRate();
	centralBankInstance.loanProcessingMethod(&loanInstance1);
	EXPECT_DOUBLE_EQ(CENTRAL_BANK::STARTING_TREASURY - CENTRAL_BANK::STARTING_TREASURY * 0.25, centralBankInstance.getCurrentTreasury());
	EXPECT_DOUBLE_EQ(expectedCurrentTreasury, centralBankInstance.getTotalTreasury());
}

/*!
 * @brief Central Bank processing big loan (value bigger then CB treasury)
 */
TEST(BankTest, CentralBank_LoanProcessingMethod_LoanTooBig) {
	centralBank centralBankInstance;
	loan loanInstance1(CENTRAL_BANK::STARTING_TREASURY * 1.25, 10, centralBankInstance.getInterestRate());
	centralBankInstance.loanProcessingMethod(&loanInstance1);
	EXPECT_DOUBLE_EQ(CENTRAL_BANK::STARTING_TREASURY, centralBankInstance.getCurrentTreasury());
	EXPECT_DOUBLE_EQ(CENTRAL_BANK::STARTING_TREASURY, centralBankInstance.getTotalTreasury());
}

/*!
 * @brief Local Bank processing normal loan (value smaller then LB treasury)
 */
TEST(BankTest, LocalBank_LoanProcessingMethod_NormalLoan) {
	centralBank centralBankInstance;
	mockLocalBank mockLocalBankInstance("Mock Local Bank", &centralBankInstance);
	loan loanInstance(LOCAL_BANK::STARTING_TREASURY * 0.5, 10, mockLocalBankInstance.getInterestRate());
	double expectedTotalTreasury = LOCAL_BANK::STARTING_TREASURY + LOCAL_BANK::STARTING_TREASURY*0.5*mockLocalBankInstance.getInterestRate();
	EXPECT_CALL(mockLocalBankInstance, loanValidationMethod(testing::_)).Times(1).WillOnce(testing::Return(true));
	mockLocalBankInstance.loanProcessingMethod(&loanInstance);
	EXPECT_DOUBLE_EQ(LOCAL_BANK::STARTING_TREASURY * 0.5, mockLocalBankInstance.getCurrentTreasury());
	EXPECT_DOUBLE_EQ(expectedTotalTreasury, mockLocalBankInstance.getTotalTreasury());
}

/*!
 * @brief Local Bank processing big loan (value bigger then LB treasury BUT smaller then CB treasury)
 */
TEST(BankTest, LocalBank_LoanProcessingMethod_LoanTooBig) {
	centralBank centralBankInstance;
	mockLocalBank mockLocalBankInstance("Mock Local Bank", &centralBankInstance);
	double loanAmount = LOCAL_BANK::STARTING_TREASURY + (CENTRAL_BANK::STARTING_TREASURY - LOCAL_BANK::STARTING_TREASURY) * 0.5;
	loan loanInstance(loanAmount, 10, mockLocalBankInstance.getInterestRate());
	double expectedTotalTreasury = LOCAL_BANK::STARTING_TREASURY + loanAmount*(mockLocalBankInstance.getInterestRate() -
			centralBankInstance.getInterestRate());
	EXPECT_CALL(mockLocalBankInstance, loanValidationMethod(testing::_)).Times(1).WillOnce(testing::Return(true));
	mockLocalBankInstance.loanProcessingMethod(&loanInstance);
	EXPECT_DOUBLE_EQ(LOCAL_BANK::STARTING_TREASURY, mockLocalBankInstance.getCurrentTreasury());
	EXPECT_DOUBLE_EQ(expectedTotalTreasury, mockLocalBankInstance.getTotalTreasury());
}

/*!
 * @brief Local Bank processing minimal loan Local Client can generate
 */
TEST(BankTest, LocalBank_LoanProcessingMethod_MinimalValueLoan) {
	centralBank centralBankInstance;
	mockLocalBank mockLocalBankInstance("Mock Local Bank", &centralBankInstance);
	loan loanInstance(
			LOCAL_CLIENT::LOAN_VALUE_MULTIPLIER,
			LOCAL_CLIENT::MINIMAL_INSTALLMENT_AMOUNT + 1,
			mockLocalBankInstance.getInterestRate()
			);
	double expectedTotalTreasury = LOCAL_BANK::STARTING_TREASURY + LOCAL_CLIENT::LOAN_VALUE_MULTIPLIER*mockLocalBankInstance.getInterestRate();
	EXPECT_CALL(mockLocalBankInstance, loanValidationMethod(testing::_)).Times(1).WillOnce(testing::Return(true));
	mockLocalBankInstance.loanProcessingMethod(&loanInstance);
	EXPECT_DOUBLE_EQ(LOCAL_BANK::STARTING_TREASURY - LOCAL_CLIENT::LOAN_VALUE_MULTIPLIER, mockLocalBankInstance.getCurrentTreasury());
	EXPECT_DOUBLE_EQ(expectedTotalTreasury, mockLocalBankInstance.getTotalTreasury());
}

/*!
 * @brief Local Bank processing maximal loan Local Client can generate
 */
TEST(BankTest, LocalBank_LoanProcessingMethod_MaximalValueLoan) {
	centralBank centralBankInstance;
	mockLocalBank mockLocalBankInstance("Mock Local Bank", &centralBankInstance);
	double loanAmount {LOCAL_CLIENT::LOAN_VALUE_MULTIPLIER * CLIENT::DICE_SIZE};
	loan loanInstance(loanAmount, CLIENT::DICE_SIZE, mockLocalBankInstance.getInterestRate());
	double expectedTotalTreasury = LOCAL_BANK::STARTING_TREASURY + loanAmount*mockLocalBankInstance.getInterestRate();
	EXPECT_CALL(mockLocalBankInstance, loanValidationMethod(testing::_)).Times(1).WillOnce(testing::Return(true));
	mockLocalBankInstance.loanProcessingMethod(&loanInstance);
	EXPECT_DOUBLE_EQ(LOCAL_BANK::STARTING_TREASURY - loanAmount, mockLocalBankInstance.getCurrentTreasury());
	EXPECT_DOUBLE_EQ(expectedTotalTreasury, mockLocalBankInstance.getTotalTreasury());
}

//========== CLIENT: client.h; localBank.h; localClient.h ==========
/*!
 * @brief Local Bank applying to Central Bank for loan
 */
TEST(ClientTest, LocalBankTakingLoan) {
	centralBank centralBankInstance;
	double centralBankInterestRate = centralBankInstance.getInterestRate();
	localBank localBankInstance("Local Bank", &centralBankInstance);
	localBankInstance.setAmountNeededForLoans(CENTRAL_BANK::STARTING_TREASURY * 0.75);
	localBankInstance.generateLoan();
	centralBankInstance.loanProcessingMethod(localBankInstance.getLoanPtr());
	EXPECT_DOUBLE_EQ(
			CENTRAL_BANK::STARTING_TREASURY*0.75*(1 + centralBankInterestRate),
			localBankInstance.getLoanPtr()->getValueLeft()
			);
	EXPECT_DOUBLE_EQ(CENTRAL_BANK::STARTING_TREASURY * 0.25, centralBankInstance.getCurrentTreasury());
}

/*!
 * @brief Local Bank paying to Central Bank
 */
TEST(ClientTest, LocalBankLoanPayment) {
	centralBank centralBankInstance;
	localBank localBankInstance("Local Bank", &centralBankInstance);
	localBankInstance.setAmountNeededForLoans(LOCAL_BANK::STARTING_TREASURY * 0.5);
	localBankInstance.generateLoan();
	double expectedTotalTreasury = centralBankInstance.getTotalTreasury() + LOCAL_BANK::STARTING_TREASURY * 0.5 *centralBankInstance.getInterestRate();
	centralBankInstance.loanProcessingMethod(localBankInstance.getLoanPtr());
	while (localBankInstance.getLoanPtr()->isReadyToBePayed()) {
		localBankInstance.paymentMethod();
	}
	EXPECT_DOUBLE_EQ(
			expectedTotalTreasury,
			centralBankInstance.getTotalTreasury()
			);
}

/*!
 * @brief Local Client applying to Local Bank for loan
 */
TEST(ClientTest, LocalClientTakingLoan) {
	centralBank centralBankInstance;
	mockLocalBank mockLocalBankInstance("Local Bank", &centralBankInstance);
	localClient localClientInstance("Local Client", &mockLocalBankInstance);
	double loanAmount = localClientInstance.getLoanPtr()->getStartingValue();
	double expectedLocalBankTreasury = mockLocalBankInstance.getCurrentTreasury() - loanAmount;
	EXPECT_CALL(mockLocalBankInstance, loanValidationMethod(testing::_)).Times(1).WillOnce(testing::Return(true));
	mockLocalBankInstance.loanProcessingMethod(localClientInstance.getLoanPtr());
	EXPECT_DOUBLE_EQ(
			loanAmount*(1 + mockLocalBankInstance.getInterestRate()),
			localClientInstance.getLoanPtr()->getValueLeft()
			);
	EXPECT_DOUBLE_EQ(expectedLocalBankTreasury, mockLocalBankInstance.getCurrentTreasury());
}

/*!
 * @brief Local Client paying to Local Bank
 */
TEST(ClientTest, LocalClientLoanPayment) {
	centralBank centralBankInstance;
	mockLocalBank mockLocalBankInstance("Local Bank", &centralBankInstance);
	localClient localClientInstance("Local Client", &mockLocalBankInstance);
	double loanAmount = localClientInstance.getLoanPtr()->getStartingValue();
	EXPECT_CALL(mockLocalBankInstance, loanValidationMethod(testing::_)).Times(1).WillOnce(testing::Return(true));
	mockLocalBankInstance.loanProcessingMethod(localClientInstance.getLoanPtr());
	double expectedLocalBankTreasury = mockLocalBankInstance.getCurrentTreasury() + localClientInstance.getLoanPtr()->getValueLeft();
	while (localClientInstance.getLoanPtr()->isReadyToBePayed()) {
		localClientInstance.paymentMethod();
	}
	EXPECT_NEAR(
			expectedLocalBankTreasury,
			mockLocalBankInstance.getCurrentTreasury(),
			0.0001
			);
	EXPECT_NEAR(
			mockLocalBankInstance.getTotalTreasury(),
			mockLocalBankInstance.getCurrentTreasury(),
			0.0001
			);
}
