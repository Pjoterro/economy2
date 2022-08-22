/*
 * @brief Class with parameters for program running
 * 
 * This class contains all simulation parameters as constants. 
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
#include <vector>

namespace BANK {
const int DICE_SIZE {20}; ///< Size of Bank class dice
}

namespace CLIENT {
const int DICE_SIZE {20}; ///< Size of Client class dice
}

namespace CENTRAL_BANK {
const std::string NAME {"Central Bank"}; ///< Name of the only Central Bank instance
const double STARTING_TREASURY {20'000}; ///< Central Bank starting treasury
/*!
 * Dependency between treasury (upper row) and interest rate (bottom row) for Central Bank instance
 */
const double INTEREST_TO_TREASURY_RATE[2][10] {
	{0.1,  0.2, 0.3,  0.4, 0.5,  0.6, 0.7,   0.8,  0.9,   1.0},
	{0.75, 0.5, 0.25, 0.2, 0.15, 0.1, 0.075, 0.05, 0.025, 0.01}
};
}

namespace LOCAL_BANK {
const double STARTING_TREASURY {10'000}; ///< LocalBank starting treasury 
/*!
 * @brief Local Bank interest rate
 * 
 * @attention this does not change like Central Bank interest rate
 */
const double INTEREST_RATE {0.05};
const double THRESHOLD_FOR_LOAN {STARTING_TREASURY * 0.01}; ///< Threshold after which Local Bank ask for loan
}

namespace LOCAL_CLIENT {
const double LOAN_VALUE_MULTIPLIER {200}; ///< Multiplier for CLIENT::DICE_SIZE
const int MINIMAL_INSTALLMENT_AMOUNT {10}; ///< Minimal amount of installments for Local Client
}

//TODO: poprawic
namespace ECONOMY2 {
const int MAX_NUMBER_OF_ACTIVE_CLIENTS {10}; ///< Size of thread pool for single Local Bank instance
const int MAX_NUMBER_OF_GENERATED_CLIENTS {100}; ///< Total number of clients throughout the whole simulation
/*!
 * @brief Just some funny loading message
 */
const std::vector<std::string> LOADING_MESSAGE_TEXT {
    "Spinning up the hamster...",
    "Shovelling coal into the server...",
    "Programming the flux capacitor",
    "Checking the gravitational constant in your locale",
    "Moving the satellite into position",
    "Must go faster, must go faster",
    "Searching for Answer to Life, the Universe, and Everything",
    "Waiting for the system admin to hit enter...",
    "Warming up the processors...",
    "RE-calibrating the internet...",
    "Doing something useful...",
    "Prepare for awesomeness!",
    "Testing ozone.",
    "Deterministically simulating the future.",
    "Testing for perfection.",
    "Your time is important to us. Please hold.",
    "Loading new loading screen.",
    "Recalculating PI....",
    "Entertaining you while you wait...",
    "Just stalling to simulate activity...",
    "Creating Time-Loop Inversion Field",
    "The internet is full... Please wait...",
    "I know this is painful to watch, but I have to load this.",
    "Buy more RAM...",
    "Starting in 0...1...2...n...",
    "Increasing speed to 88.8mph;",
    "Communing with nature",
    "Spinning the wheel of fortune",
    "Computing chance of success",
    "Preparing for hyperspace jump",
    "Centralizing the processing units...",
    "Randomizing memory access...",
    "I'm sorry Dave, I can't do that",
    "Calculating the odds...",
    "Waiting for something in the server.",
    "Calculating meaning of life...."
};
}

#endif /* CONSTANTS_H_ */
