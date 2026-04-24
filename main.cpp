#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <map>

using namespace std;

// Structure to represent a variable in a scope
struct Variable {
    string name;
    string type;
    int declared_line;

    Variable(string n, string t, int line) : name(n), type(t), declared_line(line) {}
};

// Structure to represent a scope
struct Scope {
    int level;
    int start_line;
    vector<Variable> variables;

    Scope(int lvl, int line) : level(lvl), start_line(line) {}
};

class ScopeAnalyzer {
private:
    stack<Scope> scopes;
    unordered_map<string, int> variable_counts;
    vector<string> global_variables;

public:
    void enterScope(int line) {
        Scope new_scope(scopes.size() + 1, line);
        scopes.push(new_scope);
    }

    void exitScope(int line) {
        if (!scopes.empty()) {
            // Remove all variables in this scope
            for (const auto& var : scopes.top().variables) {
                auto it = variable_counts.find(var.name);
                if (it != variable_counts.end()) {
                    it->second--;
                    if (it->second == 0) {
                        variable_counts.erase(it);
                    }
                }
            }
            scopes.pop();
        }
    }

    void declareVariable(const string& name, const string& type, int line) {
        if (!scopes.empty()) {
            scopes.top().variables.emplace_back(name, type, line);
            variable_counts[name]++;
        } else {
            // Global variable
            global_variables.push_back(name);
            variable_counts[name]++;
        }
    }

    bool isVariableVisible(const string& name) {
        return variable_counts.find(name) != variable_counts.end();
    }

    string getVariableScope(const string& name) {
        if (global_variables.end() != find(global_variables.begin(), global_variables.end(), name)) {
            return "global";
        }

        stack<Scope> temp = scopes;
        while (!temp.empty()) {
            for (const auto& var : temp.top().variables) {
                if (var.name == name) {
                    return "scope_" + to_string(temp.top().level);
                }
            }
            temp.pop();
        }
        return "not_found";
    }

    int getCurrentScopeLevel() {
        return scopes.empty() ? 0 : scopes.top().level;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    vector<string> commands;

    // Read all input
    while (getline(cin, input)) {
        if (input.empty() || input == "#") break;
        commands.push_back(input);
    }

    ScopeAnalyzer analyzer;
    vector<string> outputs;

    // Process commands
    for (int i = 0; i < commands.size(); i++) {
        string cmd = commands[i];
        istringstream iss(cmd);
        string action;
        iss >> action;

        if (action == "DECLARE") {
            string type, name;
            iss >> type >> name;
            analyzer.declareVariable(name, type, i + 1);
        }
        else if (action == "CHECK") {
            string name;
            iss >> name;
            if (analyzer.isVariableVisible(name)) {
                outputs.push_back("VISIBLE " + analyzer.getVariableScope(name));
            } else {
                outputs.push_back("NOT_VISIBLE");
            }
        }
        else if (action == "ENTER") {
            analyzer.enterScope(i + 1);
        }
        else if (action == "EXIT") {
            analyzer.exitScope(i + 1);
        }
        else if (action == "LEVEL") {
            outputs.push_back(to_string(analyzer.getCurrentScopeLevel()));
        }
        else if (action == "COUNT") {
            string pattern;
            iss >> pattern;
            // For patterns, we'll handle simple cases
            if (pattern == "ALL") {
                outputs.push_back(to_string(analyzer.getCurrentScopeLevel()));
            }
        }
    }

    // Output results
    for (const auto& output : outputs) {
        cout << output << '\n';
    }

    return 0;
}