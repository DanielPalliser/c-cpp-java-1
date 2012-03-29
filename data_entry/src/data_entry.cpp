//============================================================================
// Name        : data_entry2.cpp
// Author      : Daniel Palloser
// Version     :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "data_entry.hpp"
using namespace std;

/**
 * this class allows a user to input information about a ship and save that information to a file.
 * The program also outputs to a log file
 */
class data_entry {
public:
	data_entry();
	void runMenu();
private:
	int mmsi, course;
	std::string response, name;
	float lat, lng, speed;
	template<typename Type>
	void getInput(Type &destination, std::string prompt);
	void writeShipFile();
	void getInformation();
};

data_entry::data_entry() {

}

/**
 * gets information from a user about a ship.
 */
void data_entry::getInformation() {
	std::cout << "Please enter information about a ship\n";
	getInput(mmsi, "MMSI: ");
	getInput(name, "Ship Name: ");
	getInput(lat, "Latitude(in the format (-)xx.xxx): ");
	getInput(lng, "Longitude (in the format (-)xx.xxx):");
	getInput(course, "Course made good: ");
	getInput(speed, "Speed over Ground (in the format xx.x): ");
}

/**
 * function gets input and checks type
 */
template<typename Type>
void data_entry::getInput(Type &destination, std::string prompt) {
	while (1) {

		std::cout << prompt;
		std::string temporaryHolder;
		std::stringstream stringStream;

		std::getline(std::cin, temporaryHolder);
		stringStream << temporaryHolder;

		if (stringStream >> destination) {
			break;
		}
	}
}
/**
 * writes a ship file from the data input by the user
 */
void data_entry::writeShipFile() {
	int fd = open("test.txt", O_RDWR);
	if (fd == -1) {
		cout << "File descriptor error";
		exit(1);
	}
	struct flock* fl = file_lock(F_WRLCK, SEEK_SET);

	if (fcntl(fd, F_SETLK, fl) == -1) {
		if (errno == EACCES || errno == EAGAIN) {
			cout << "Already locked by another process... bad luck\n";
			exit(1);
		} else {
			/* Handle unexpected error */;
		}
	} else {
		std::stringstream tmpstr;
		tmpstr << mmsi << ".shp";
		string filename = tmpstr.str();
		ofstream file(filename.c_str());
		file << mmsi << endl << name << endl << lat << endl << lng << endl
				<< course << endl << speed;
		file.close();
		FILE* text = fdopen(fd, "a");
		fcntl(fd, F_SETLKW, file_lock(F_UNLCK, SEEK_SET));
		fclose(text);
	}
}

/**
 * runs a menu which allows a user to enter information about a ship and save that information
 */
void data_entry::runMenu() {
	char choice = '\0';
	while (choice != 'q' && choice != 'Q') {
		std::cout
				<< "\nChoose an option: \n E: enter ship information \n S: save information to a file \n Q: quit";
		switch (choice) {
		case 'e':
		case 'E':
			getInformation();
			break;
		case 's':
		case 'S':
			writeShipFile();
			break;
		}
	}
}

int main() {
	data_entry dataentry;
	dataentry.runMenu();
}

