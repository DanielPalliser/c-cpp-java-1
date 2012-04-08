//============================================================================
// Name        : data_entry2.cpp
// Author      : Daniel Palloser
// Version     :
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "data_entry.h"

#define WEST_BRDR -6.5
#define	EAST_BRDR -4.75
#define NORTH_BRDR 52.75
#define SOUTH_BRDR 51.5

using namespace std;

/**
 * this class allows a user to input information about a ship and save that information to a file.
 * The program also outputs to a log file
 */
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
		char temporaryHolder[256];
		std::stringstream stringStream;
		cin.getline(temporaryHolder, 256, '\n');
		stringStream << temporaryHolder;
		if (stringStream >> destination) {
			break;
		}
	}
}

void data_entry::getInput(char* destination, std::string prompt) {
	cout << prompt;
	cin.getline(destination, 256);
}

/**
 * writes a ship file from the data input by the user
 */
void data_entry::writeShipFile() {
	int fd = open("log.txt", (O_RDWR | O_CREAT | O_APPEND), 0664);
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
	} else if (lng < WEST_BRDR || EAST_BRDR < lng || lat < SOUTH_BRDR
			|| NORTH_BRDR < lat) {
		{

			stringstream logstrm;

			cout << "ship with mmsi: " << mmsi
					<< " left the area. Deleting file if it exists" << endl;
			time_t t = time(NULL); //current time
			struct tm* now = gmtime(&t);
			logstrm << asctime(now) << "ship with mmsi: " << mmsi
					<< " left the area. Deleting file if it exists" << endl;
			string logstr = logstrm.str(); // generate string to be written to file
			FILE* log = fdopen(fd, "a");
			fprintf(log, "%s", logstr.c_str()); // write to log file
			fcntl(fd, F_SETLKW, file_lock(F_UNLCK, SEEK_SET));
			fclose(log);
			std::stringstream tmpstr;
			tmpstr << mmsi << ".shp";
			string filename = tmpstr.str();
			remove(filename.c_str());
		}
	} else {
		std::stringstream tmpstr;
		tmpstr << mmsi << ".shp";
		string filename = tmpstr.str();
		ofstream file(filename.c_str());
		file << mmsi << endl << name << endl << lat << endl << lng << endl
				<< course << endl << speed;
		file.close();
		FILE* log = fdopen(fd, "a");
		time_t t = time(NULL); //current time
		struct tm* now = gmtime(&t);
		stringstream logstrm;
		logstrm << asctime(now) << " Ship with mmsi " << mmsi << " was updated"
				<< endl;
		string logstr = logstrm.str();
		fprintf(log, "%s", logstr.c_str());
		fcntl(fd, F_SETLKW, file_lock(F_UNLCK, SEEK_SET));
		fclose(log);
		cout << "ship information saved to file";
	}
}

/**
 * runs a menu which allows a user to enter information about a ship and save that information
 */
void data_entry::runMenu() {
	char choice = '\0';
	while (choice != 'q' && choice != 'Q') {

		std::cout
				<< "\nChoose an option: \n E: enter ship information \n S: save information to a file \n Q: quit \n";
		cin >> choice;
		switch (choice) {
		case 'e':
		case 'E':
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

