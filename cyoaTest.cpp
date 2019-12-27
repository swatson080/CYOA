#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::ifstream openFile(std::string fileName);

bool testIfEof(std::string line);
std::string stripScenePrefix(std::string line);	// Take off the prefix at the start of the line
std::string stripAnswerPrefix(std::string line); // Take off the prefix at the start of choice lines
std::string findNextScene(std::string line);	// Look for the $ in answer choices that indicate where to go next
std::string stripPostfix(std::string line);	// Take off the postfix at the end of a line
std::string stripEnd(std::string line);

int getIntInput(std::string message);
std::string getStringInput();

int main() {

	const int MAX_INDEX = 2;
	int index = 0, choice = 0;
	std::string nextScene = "";				// This will hold player choice for next scene (from array below)
	std::string nextSceneLocations[MAX_INDEX] = {""};	// Store the markers to jump to the next scene
	std::ifstream file = openFile("cyoaText.txt");
	if(file.is_open()) {
		while(true) {
			std::string currentLine; 
			getline(file,currentLine); 
			if(nextScene != "") {
				int pos = currentLine.find(nextScene);
				if(pos == std::string::npos) {
					continue;
				}
				else {
					nextScene = "";
				}
			}
			if(testIfEof(currentLine)) {
				currentLine = stripScenePrefix(currentLine);
				currentLine = stripEnd(currentLine);
				std::cout << currentLine << std::endl;
				std::cout << "THE END" << std::endl;
				break;
			}
			if(currentLine[0] == '$') {		// If the current line is a new scene, take off the prefix
				currentLine = stripScenePrefix(currentLine);
				// Possibly add scene header/foot e.g. "_________________________________" to break up the scenes/questions?
				std::cout << currentLine << std::endl;
			}
			if(currentLine[0] == '@') {
				currentLine = stripAnswerPrefix(currentLine);	
				nextSceneLocations[index] = findNextScene(currentLine);
				++index;
				currentLine = stripPostfix(currentLine);
				std::cout << currentLine << std::endl;
					if(index == MAX_INDEX) {
						while(true) {
							choice = getIntInput("Make a choice >");
							if(choice > 2 || choice <= 0) {
								std::cout << "Invalid selection" << std::endl;
							}
							else {
								break;
							}
						}
						nextScene = nextSceneLocations[choice-1];
						index = 0;
					}		
			}
		}
	}

	file.close();

	return 0;

}

std::ifstream openFile(std::string fileName) {
	std::ifstream file(fileName);
	if(file.is_open()) {
		return file;
	}
	else {
		std::cout << "Error opening file" << std::endl;
		return file;
	}
}

bool testIfEof(std::string line) {
	int pos = line.find("END");
	if(pos == std::string::npos) {
		return false;
	}
	else {
		return true;
	}
}

std::string stripScenePrefix(std::string line) {
	int pos = line.find(':');
	line = line.substr(pos+1,std::string::npos);
	return line;
}

std::string stripAnswerPrefix(std::string line) {
	int pos = line.find('@') + 1;
	line = line.substr(pos,std::string::npos);
	return line;
}

std::string findNextScene(std::string line) {
	std::string marker;
	int pos = line.find('$');
	marker = line.substr(pos,std::string::npos);
	return marker;
}	

std::string stripPostfix(std::string line) {
	int pos = line.find('$');
	line = line.substr(0,pos);
	return line;
}	

std::string stripEnd(std::string line) {
	int pos = line.find("END");
	line = line.substr(0,pos);
	return line;
}

// Gets integer input
int getIntInput(std::string message) {
	std::string rawInput;
	while(true) {
		std::cout << message;
		rawInput = getStringInput();
		std::stringstream stream(rawInput);

		int value;
		char temp;
		stream >> value;
		if(stream && !(stream >> temp) && value >= 0 && value <= 5) {
			return value;
		}
		else {
			std::cout << "Invalid input" << std::endl;
		}
	}
}

// Gets string input
std::string getStringInput() {
	std::string input;
	std::getline(std::cin,input);
	return input;
}
