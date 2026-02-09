#include <iostream>
#include <mutex>
#include <thread>
#include <windows.h>
#include <fstream>
#include <string>
using namespace std;

string textFile[5000];
int textLinesCount = 0;

string forbiddenWords[500];
int forbiddenWordsCount = 0;

int totalFound = 0;
mutex mtx;

bool isSign(char c) {
	string sign = ".,!?;:'@[]~}{#~$%^&*()-+=/`\\|\"`<>";
	for (int i = 0; i < sign.length(); i++) {
		if(c == sign[i]) {
			return true;
		}
		return false;
	}
}
string makeLowerCase(string s) {
	string result = s;
	for (int i = 0; i < result.length(); i++) {
		result[i] = tolower(result[i]);
	}
	return result;
}
void loadForbidden() {
	ifstream file("forbidden.txt");
	string line;
	forbiddenWordsCount = 0;
	while (getline(file, line)) {
		if (line.length() == 0) {
			continue;
		}
		forbiddenWords[forbiddenWordsCount] = makeLowerCase(line);
		forbiddenWordsCount++;
	}
	file.close();
}
void loadInput() {
	ifstream file("input.txt");
	string line;
	textLinesCount = 0;
	while (getline(file, line)) {
		textFile[textLinesCount] = line;
		textLinesCount++;
	}
	file.close();
}