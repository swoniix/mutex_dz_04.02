#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <string>

using namespace std;

string textFile[5000];
int textLinesCount = 0;

string forbiddenWords[500];
int forbiddenWordsCount = 0;

int totalFound = 0;
mutex mtx;

bool isDelimiter(char c) {
    string del = " \t.,!?;:\"()[]{}<>-";
    for (int i = 0; i < del.length(); i++) {
        if (c == del[i]) return true;
    }
    return false;
}

char toLowerAscii(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
    return c;
}

bool isWholeWord(string line, int pos, int len) {
    if (pos > 0 && !isDelimiter(line[pos - 1])) return false;
    if (pos + len < line.length() && !isDelimiter(line[pos + len])) return false;
    return true;
}

void loadForbidden() {
    ifstream file("forbidden.txt");
    string line;
    forbiddenWordsCount = 0;
    while (getline(file, line)) {
        if (line.length() == 0) continue;
        forbiddenWords[forbiddenWordsCount++] = line;
    }
    file.close();
}

void loadInput() {
    ifstream file("input.txt");
    string line;
    textLinesCount = 0;
    while (getline(file, line)) {
        textFile[textLinesCount++] = line;
    }
    file.close();
}

void searchRange(int start, int end) {
    ofstream report("report.txt", ios::app);
    for (int i = start; i < end; i++) {
        string line = textFile[i];
        for (int j = 0; j < forbiddenWordsCount; j++) {
            string word = forbiddenWords[j];
            int len = word.length();
            int pos = line.find(word);
            while (pos != -1) {
                if (isWholeWord(line, pos, len)) {
                    mtx.lock();
                    totalFound++;
                    report << "Line: " << (i + 1)
                        << ", Position: " << (pos + 1)
                        << ", Word: " << word << endl;
                    report << "Context: " << line << endl << endl;
                    mtx.unlock();
                }
                pos = line.find(word, pos + 1);
            }
        }
    }
    report.close();
}

int main() {
    loadForbidden();
    loadInput();

    ofstream clear("report.txt");
    clear.close();

    int threadsCount = 4;
    if (threadsCount > textLinesCount) threadsCount = textLinesCount;

    thread threads[4];
    int linesPerThread = textLinesCount / threadsCount;
    int start = 0;

    for (int i = 0; i < threadsCount; i++) {
        int end = (i == threadsCount - 1)
            ? textLinesCount
            : start + linesPerThread;
        threads[i] = thread(searchRange, start, end);
        start = end;
    }

    for (int i = 0; i < threadsCount; i++) {
        threads[i].join();
    }

    cout << "TOTAL FOUND: " << totalFound << endl;
}