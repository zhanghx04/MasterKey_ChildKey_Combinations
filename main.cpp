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

/**
 *  @brief isValidSeondaryChildKey will check whether the child key can be the secondary child key of the seondary
 *  master key.
 *  @param secondary is the list of pins of the secondary master key
 *  @param child is the list of pins of the secondary child key candidate
 *  @param pinSets is the pin set of the child key
 *  @return Only when the child key and the secondary master key meet the requirement
 */
bool isValidSecondaryChildKey(const vector<int>& secondary, const vector<int>& child, const vector<pair<int, int>>&
pinSets) {
    size_t len = secondary.size();
    for (int i = 0; i < len; ++i) {
        // For the valid secondary child keys
        // either the secondary master key's pin[i] is equal to the child key's bot-pin[i]
        // or the secondary master key's pin[i] is equal to the sum of  child key's bot-pin[i] and mid-pin[i]
        if (secondary[i] != pinSets[i].first && secondary[i] != pinSets[i].first + pinSets[i].second) {
            return false;
        }
    }
    return true;
}

/**
 *  @brief getAssembly function computes each pair of bot-pin and mid-pin' lengths based on the master key and
 *  current child key.
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

/**
 *  @brief currlineResult collects all of the pin set of the child key.
 *  @param key is the child key
 *  @param allMap is the map contains all of the possible child key that could be the child key of the master key
 *  @return return the child key information as string
 */
string currLineResult(const vector<int>& key, map<vector<int>,vector<pair<int, int>>>& allMap) {
    string result = "{";
    // Child key
    for (int c : key) {
        result += to_string(c) + " ";
    }
    result += "}  |  ";

    // Pin sets
    for (auto& p : allMap[key]) {
        result += "(" + to_string(p.first) + " + " + to_string(p.second) + ")";
    }
    result += "\n";

    return result;
}

/**
 *  @brief writeAllMasterAndChild writes the whole key information into a local file
 *  @param master is the master key
 *  @param secondaryResults contains the secondary master key and all of its secondary child key
 *  @param allMap contains all of the information of possible child keys
 */
void writeAllMasterAndChild(const vector<int>& master,
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
    for (auto& [secondaryMasterKey, secondaryChildKeyList] : secondaryResults) {
        output_file << separateLineSMaster;
        // Secondary
        string secondary = "{ ";
        for (int c : secondaryMasterKey) {
            secondary += to_string(c) + " ";
        }
        secondary += "}";
        secondary += " - Total Child Key: " + to_string(secondaryChildKeyList.size()) + "\n";
        output_file << secondary;
        output_file << separateLineSMasterAndChild;
        // Child keys
        for (auto& child : secondaryChildKeyList) {
            string childInfo = currLineResult(child, allMap);
            output_file << childInfo;
        }
    }

    // Close the file
    output_file.close();
}

/**
 *  @brief writeAllMasterAndChild writes the whole key information into a local file
 *  @param master is the master key
 *  @param secondaryResults contains the secondary master key and all of its secondary child key
 *  @param allMap contains all of the information of possible child keys
 */
void writeOneLevelMasterAndChild(const vector<int>& master,
                            vector<vector<int>>& childResults,
                            map<vector<int>,vector<pair<int, int>>>& allMap) {
    string separateLineSMaster = "\n========================================================================\n\n";
    string separateLineSMasterAndChild = "------------------------------------------------------------------------\n";

    // Open the file that will write data into
    ofstream output_file("1levelManageMap.txt");

    // Master key
    string record = "{ ";
    for (int c : master) {
        record += to_string(c) + " ";
    }
    record += "}";
    record += " - Total Child Key: " + to_string(childResults.size()) + "\n";
    output_file << record;
    output_file << separateLineSMaster;

    cout << childResults.size() << endl;

    // Get child keys
    for (auto& childKeyList : childResults) {
        string childInfo = currLineResult(childKeyList, allMap);
        output_file << childInfo;
    }

    // Close the file
    output_file.close();
}

/**
 *  @brief twoLevelManagement manage the the keys that has master, secondary master, and secondary child
 */
void twoLevelManagement(const vector<int>& master, map<vector<int>,vector<pair<int, int>>>& allPossibleResults,
                        map<vector<int>,vector<pair<int, int>>>& allPossibleResultsMap) {
    // Secondary master keys
    int numSecMaster = 100;
    int numAllPossible = (int)allPossibleResults.size();
    int n = numAllPossible / (numSecMaster + 1);
    set<vector<int>> secondaryMasters;

    int count = 0;
    for (auto [childKey, assembly] : allPossibleResults) {
        // Here, get every n_th childkey as secondary master key
        if (count % n == 0) {
            count = 0;
            secondaryMasters.insert(childKey);
        }

        // Only get numSecMaster number of childkeys
        if (secondaryMasters.size() == numSecMaster) break;
        count++;
    }

    cout << "SecondaryMaster count: " << secondaryMasters.size() << endl;

    // Remove the secondary master from allPossibleResults
    for (auto& secondaryKey : secondaryMasters) {
        allPossibleResults.erase(secondaryKey);
    }

    cout << "ALl possible child key count without secondary master keys: " << allPossibleResults.size() << endl;

    map<vector<int>, vector<vector<int>>> secondaryResults;

    // Go through all the possible child keys and assign to the secondary master keys
    for (auto& [childKey, childKeyAssembly] : allPossibleResults) {
        for (auto& secondaryKey : secondaryMasters) {
            if (isValidSecondaryChildKey(secondaryKey, childKey, childKeyAssembly)) {
                secondaryResults[secondaryKey].push_back(childKey);
                break;
            }
        }
    }

    writeAllMasterAndChild(master, secondaryResults, allPossibleResultsMap);
}

/**
 *  @brief oneLevelManagement manage the the keys that has master and child
 */
void oneLevelManagement(const vector<int>& master, map<vector<int>,vector<pair<int, int>>>& allPossibleResults,
                        map<vector<int>,vector<pair<int, int>>>& allPossibleResultsMap) {
    vector<vector<int>> childKeyResults;

    // Go through all the possible child keys and assign to the secondary master keys
    for (auto& [childKey, childKeyAssembly] : allPossibleResults) {
            if (isValidSecondaryChildKey(master, childKey, childKeyAssembly)) {
                childKeyResults.push_back(childKey);
            }
    }

    writeOneLevelMasterAndChild(master, childKeyResults, allPossibleResultsMap);
}



int main() {
    const vector<int> master = {1, 2, 3, 4, 5, 6}; // 2, 3, 2, 1, 5, 4
    int level = 1;

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

    if (level % 2) {
        cout << "Start One Level Master/Child key Combination..." << endl;
        oneLevelManagement(master, allPossibleResults, allPossibleResults);
    } else {
        cout << "Start Two Level Master/Child key Combination..." << endl;
        twoLevelManagement(master, allPossibleResults, allPossibleResultsMap);
    }

    // printWithChild({1, 1, 1, 1, 1, 1}, allPossibleResults);
    // printWithChild({2, 5, 1, 2, 1, 1}, allPossibleResults);
    // printWithChild({2, 1, 1, 2, 1, 6}, allPossibleResults);
    // printWithChild({2, 1, 1, 2, 2, 5}, allPossibleResults);

    // printAll(allPossibleResults);

    return 0;
}
