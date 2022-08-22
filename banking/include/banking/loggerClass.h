/*
 * @brief Simple logging class
 * 
 * Logging class based on boost/log library. Thread safe.
 */

#ifndef LOGGING_INCLUDE_LOGGING_LOGGING_H_
#define LOGGING_INCLUDE_LOGGING_LOGGING_H_

#include <boost/log/core/core.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <iostream>
#include <fstream>
#include <string>

//TODO: Log is not saved in file in directory but in random file in main source folder
class loggerClass {
private:
	std::string fileName; ///< Name of the log file.

public:

	loggerClass();

	virtual ~loggerClass();

	/*!
	 * @brief Method setting up whole logging class.
	 */
	static void logInit();

	/*!
	 * @brief Simple logging method
	 * 
	 * Method to log desired info to output set by logging::logInit().
	 */
	static void logEvent(std::string input);

	/*!
	 * @brief Test method used in debugging
	 */
	static void logTest();
};

#endif /* LOGGING_INCLUDE_LOGGING_LOGGING_H_ */
