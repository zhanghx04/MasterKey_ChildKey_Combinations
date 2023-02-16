#include <iostream>
#include <fstream>

#include <set>
#include <map>
#include <vector>
#include <string>

using namespace std;

/**
    Mid and Bottom parts combination for the master key and child keys.

    The child key can open only the specific door. The master key can open all the doors.

    In the normal lock, there top part and bottom part. Each part has different length.
    When the key is inserted into the lock, both top and bottom part will be pushed up.
    If the key is right, the top of all the bottom parts will on the same level, which the key can be rotated.

    If the manager want to use Master key. Then, the bottom part need to be separated to mid part and bottom part.
    Therefore, when the child key is inserted, the top of the mid parts can be on the same level. When the master
    key is inserted, the top of the bottom parts can be on the same level.

    e.g.
               top ||   bot               key
        @@@@@@@@[   4   ][5     ]       [  2]
        @@@@@@@@@@@[   4   ][2  ]    [     5]
        @@@@@@@[   4   ][6      ]        [ 1]
        @@@@@@@@[   4   ][5     ]       [  2]
        @@@@@@@[   4   ][6      ]        [ 1]
        @@@@@@@[   4   ][6      ]        [ 1]

        -----------------Insert Key-----------------

               top ||   bot   key
        @@@[   4   ][5     ][  2]
        @@@[   4   ][2  ][     5]
        @@@[   4   ][6      ][ 1]
        @@@[   4   ][5     ][  2]
        @@@[   4   ][6      ][ 1]
        @@@[   4   ][6      ][ 1]


    rule:
        bot is min(master[i], child[i])
        mid is abs(master[i] - child[i])
*/
bool isValidSecondaryChildKey(vector<int> secondary, vector<int> child, vector<pair<int, int>>& pinSets) {
    size_t len = secondary.size();
    for (int i = 0; i < len; ++i) {
        int bot = min(secondary[i], child[i]);
        int mid = abs(secondary[i] - child[i]);
        if (secondary[i] != pinSets[i].first && secondary[i] != pinSets[i].first + pinSets[i].second) {
            return false;
        }
    }
    return true;
}

/**
 *  @brief getAssembly function computes each pair of bot-pin and mid-pin' lengths based on the master key and
 *  current child key
 *  @param master is the master key, the element is the length of each pin
 *  @param child is the child key, the element is the length of each pin
 *  @param allPossibleResults is the map that the key is the child key, the value is the assembly of the child key
 */
void getAssembly(const vector<int>& master, const vector<int>& child, map<vector<int>,vector<pair<int, int>>>&
        allPossibleResults) {
    size_t len = master.size();
    for (int i = 0; i < len; ++i) {
        int bot = min(master[i], child[i]);
        int mid = abs(master[i] - child[i]);
        allPossibleResults[child].emplace_back(bot, mid);
    }
}

void printWithChild(const vector<int>& child, map<vector<int>,vector<pair<int, int>>>& allPossibleResults) {
    cout << "{ ";
    for (int i : child) {
        cout << i << " ";
    }
    cout << "}    ";

    for (auto& p : allPossibleResults[child]) {
        cout << "(" << p.first << "+" << p.second << ")";
    }
    cout << endl;
}

void writeAllChild(map<vector<int>,vector<pair<int, int>>>& allPossibleResults) {
    // cout << "Print all data: "<< endl;
    // cout << "   child key     |  (bottom + middle)" << endl;

    // Open the file that will write data into
    ofstream output_file("data.txt");

    for (auto& [child, pairs] : allPossibleResults) {
        string record = "{ ";
        // cout << "{ ";
        for (int c : child) {
            // cout << c << " ";
            record += to_string(c) + " ";
        }
        // cout << "}  |  ";
        record += "}  |  ";

        for (auto& p : pairs) {
            // cout << "(" << p.first << " + " << p.second << ")";
            record += "(" + to_string(p.first) + " + " + to_string(p.second) + ")";
        }
        // cout << endl;
        record += "\n";

        // Write data into file
        output_file << record;
    }
    output_file.close();
}

string currLineResult(vector<int> key, map<vector<int>,vector<pair<int, int>>>& allMap) {
    string result = "{";
    for (int c : key) {
        result += to_string(c) + " ";
    }
    result += "}  |  ";
    for (auto& p : allMap[key]) {
        result += "(" + to_string(p.first) + " + " + to_string(p.second) + ")";
    }
    result += "\n";

    return result;
}
void writeAllMasterAndChild(vector<int>& master,
                            map<vector<int>, vector<vector<int>>>& secondaryResults,
                            map<vector<int>,vector<pair<int, int>>>& allMap) {
    string separateLineSMaster = "\n========================================================================\n\n";
    string separateLineSMasterAndChild = "------------------------------------------------------------------------\n";

    // Open the file that will write data into
    ofstream output_file("2levelManageMap.txt");

    // Master key
    string record = "{ ";
    for (int c : master) {
        record += to_string(c) + " ";
    }
    record += "}";
    output_file << record;

    cout << secondaryResults.size() << endl;

    // Secondary Master and their child keys
    for (auto it = secondaryResults.begin(); it != secondaryResults.end(); ++it) {
        output_file << separateLineSMaster;
        // Secondary
        string secondary = "{ ";
        for (int c : it->first) {
            secondary += to_string(c) + " ";
        }
        secondary += "}\n";
        output_file << secondary;
        output_file << separateLineSMasterAndChild;
        // Child keys
        for (auto& child : it->second) {
            string childInfo = currLineResult(child, allMap);
            output_file << childInfo;
        }
    }
    output_file.close();
}

int main() {
    vector<int> master = {1, 2, 3, 4, 5, 6}; // 2, 3, 2, 1, 5, 4
    map<vector<int>,vector<pair<int, int>>> allPossibleResults, allPossibleResultsMap;

    // Traversal all possible child key combinations and compute the assembly
    for (int i1 = 1; i1 <= 7; ++i1) {
        for (int i2 = 1; i2 <= 7; ++i2) {
            for (int i3 = 1; i3 <= 7; ++i3) {
                for (int i4 = 1; i4 <= 7; ++i4) {
                    for (int i5 = 1; i5 <= 7; ++i5) {
                        for (int i6 = 1; i6 <= 7; ++i6) {
                            getAssembly(master, {i1, i2, i3, i4, i5, i6}, allPossibleResults);
                        }
                    }
                }
            }
        }
    }
    allPossibleResultsMap = allPossibleResults;

    // remove the master key from allPossibleResults
    allPossibleResults.erase(master);
    cout << "ALl possible child key count: " << allPossibleResults.size() << endl;

    // Secondary master keys
    int numSecMaster = 100;
    int numAllPossible = allPossibleResults.size();
    int n = numAllPossible / (numSecMaster + 1);
    set<vector<int>> secondaryMasters;

    int count = 0;
    for (auto it = allPossibleResults.begin(); it != allPossibleResults.end(); ++it) {
        if (count % n == 0) {
            count = 0;
            secondaryMasters.insert(it->first);
        }
        if (secondaryMasters.size() == numSecMaster) break;
        count++;
    }

    cout << "SecondaryMaster count: " << secondaryMasters.size() << endl;

    // Remove the secondary master from allPossibleResults
    for (auto it = secondaryMasters.begin(); it != secondaryMasters.end(); ++it) {
        allPossibleResults.erase(*it);
    }

    cout << "ALl possible child key count without secondary master keys: " << allPossibleResults.size() << endl;

    map<vector<int>, vector<vector<int>>> secondaryResults;

    // Go through all the possible child keys and assign to the secondary master keys
    for (auto it = allPossibleResults.begin(); it != allPossibleResults.end(); ++it) {
        for (auto itSecond = secondaryMasters.begin(); itSecond != secondaryMasters.end(); ++itSecond) {
            if (isValidSecondaryChildKey(*itSecond, it->first, it->second)) {
                secondaryResults[*itSecond].push_back(it->first);
                break;
            }
        }
    }

    writeAllMasterAndChild(master, secondaryResults, allPossibleResultsMap);

    // printWithChild({1, 1, 1, 1, 1, 1}, allPossibleResults);
    // printWithChild({2, 5, 1, 2, 1, 1}, allPossibleResults);
    // printWithChild({2, 1, 1, 2, 1, 6}, allPossibleResults);
    // printWithChild({2, 1, 1, 2, 2, 5}, allPossibleResults);

    // printAll(allPossibleResults);

    return 0;
}
