//============================================================================
// Name        : data_entry2.cpp
// Author      : Daniel Palloser
// Version     :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

template<typename Type>
void GetInput(Type &destination, std::string prompt);

int main() {
	int mmsi, course;
	std::string response, name;
	float lat, lng, speed;
	std::cout << "Please enter information about a ship/n";
	GetInput(mmsi, "MMSI: ");
	GetInput(name, "Ship Name: ");
	GetInput(lat, "Latitude(in the format (-)xx.xxx): ");
	GetInput(lng, "Longitude (in the format (-)xx.xxx):");
	GetInput(course, "Course made good: ");
	GetInput(speed, "Speed over Ground (in the format xx.x): ");
}

/**
 * function gets input and checks type
 */
template<typename Type>
void GetInput(Type &destination, std::string prompt) {
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

