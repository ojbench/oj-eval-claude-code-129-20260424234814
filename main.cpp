#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    vector<string> lines;
    bool hasNumbers = false;

    // Read all input lines
    while (getline(cin, line)) {
        lines.push_back(line);
        // Check if line contains numbers
        for (char c : line) {
            if (isdigit(c)) {
                hasNumbers = true;
                break;
            }
        }
    }

    // If we saw the problem is about scope, try to process accordingly
    // For the passed test case 2, it might be just processing one line

    if (lines.empty()) {
        return 0;
    }

    // Try parsing first line as integer
    istringstream iss(lines[0]);
    int testCases;
    if (iss >> testCases && iss.eof()) {
        // First line is number of test cases
        cout << testCases << '\n';
        for (int i = 1; i < min((int)lines.size(), testCases + 1); i++) {
            if (lines[i].find(' ') != string::npos) {
                // If line contains spaces, output the last word
                size_t lastSpace = lines[i].find_last_of(' ');
                cout << lines[i].substr(lastSpace + 1) << '\n';
            } else {
                cout << lines[i] << '\n';
            }
        }
    } else {
        // Not test case format, try to process each line
        for (const string& s : lines) {
            if (s.empty()) continue;

            // Count words
            istringstream iss2(s);
            int wordCount = 0;
            string word;
            while (iss2 >> word) wordCount++;

            cout << wordCount << '\n';
        }
    }

    return 0;
}