/*
 * data_entry.hpp
 *
 *  Created on: Mar 29, 2012
 *      Author: dan
 */


#ifndef DATA_ENTRY_HPP_
#define DATA_ENTRY_HPP_

#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#include <string>
#include <fstream>
#include <vector>

#include <iostream>
using namespace std;

class data_entry {
public:
	data_entry();
	void runMenu();
private:
	int mmsi, course;
	std::string response;
	float lat, lng, speed;
	char name[256];
	void getInput(char* destination, std::string prompt);
	void writeShipFile();
	void getInformation();

	template<typename Type>
	void getInput(Type &destination, std::string prompt);

};

/*
 *
 */
struct flock* file_lock(short type, short whence) {
    static struct flock ret;
    ret.l_type = type;
    ret.l_start = 0;
    ret.l_whence = whence;
    ret.l_len = 0;
    ret.l_pid = getpid();
    return &ret;
}

#endif /* DATA_ENTRY_HPP_ */
