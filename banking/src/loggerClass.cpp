/*
 * logging.cpp
 *
 *  Created on: 1 sie 2022
 *      Author: pjoter
 */
#include <banking/loggerClass.h>

loggerClass::loggerClass() {

}

void loggerClass::logInit() {
	boost::log::add_file_log("test1.log");
//	boost::log::add_file_log(
//    		boost::log::keywords::file_name = "sample.log"
//    				);
    boost::log::core::get()->set_filter
    (
    		boost::log::trivial::severity >= boost::log::trivial::info
    );    
}

void loggerClass::logEvent(std::string input) {
	BOOST_LOG_TRIVIAL(info) << input;
}

void loggerClass::logTest() {
	BOOST_LOG_TRIVIAL(info) << "This is test logging function";
}

loggerClass::~loggerClass() {}
