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
using namespace std;

class data_entry{
public:
	data_entry();
	template<typename Type>
	void GetInput(Type &destination, std::string prompt);
	void WriteShipFile();
	void getInformation();
protected:
	int mmsi, course;
	std::string response, name;
	float lat, lng, speed;
};

data_entry::data_entry(){

}

/**
 * function gets input and checks type
 */
template<typename Type>
void data_entry::GetInput(Type &destination, std::string prompt) {
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
void data_entry::WriteShipFile(){
	std::stringstream tmpstr;
	tmpstr << mmsi;
	string filename = tmpstr.str();
	ofstream file (filename.c_str());
	file << mmsi << endl << name << endl << lat << endl << lng << endl << course << endl << speed;
	file.close();
}

void data_entry::getInformation(){
	std::cout << "Please enter information about a ship\n";
	GetInput(mmsi, "MMSI: ");
	GetInput(name, "Ship Name: ");
	GetInput(lat, "Latitude(in the format (-)xx.xxx): ");
	GetInput(lng, "Longitude (in the format (-)xx.xxx):");
	GetInput(course, "Course made good: ");
	GetInput(speed, "Speed over Ground (in the format xx.x): ");
}

int main() {
	data_entry dataentry;
	dataentry.getInformation();
	dataentry.WriteShipFile();
}

