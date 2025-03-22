#include <fstream>
#include <string>
#include "gtest/gtest.h"

using namespace std;

class ioTest : public ::testing::Test {
protected:
    ioTest() {
    }
    ~ioTest() override {
    }
    void SetUp() override {
    }
    void TearDown() override {
    }
};

struct PersonInfo
{
    string name;
    vector<string> phone;
};

string filePath = R"(./res/io/test.txt)";
TEST_F(ioTest, io) {
    ifstream ifs;
    ifs.open(filePath, ios::in);
    if (!ifs.is_open()){
        cerr << "Failed to open file: " << filePath << endl;
        perror("Error opening file");
        return;
    }
    vector<PersonInfo> persons;
    string line, word;
    while (getline(ifs, line)) {
        istringstream iss(line);
        PersonInfo person;

        iss >> person.name;
        while (iss >> word) {
            person.phone.push_back(word);
        }
        persons.push_back(person);
    }

    ifs.close();

    for (auto& person : persons) {
        cout << person.name << " ";
        for (auto& phone : person.phone) {
            cout << phone << " ";
        }
        cout << endl;
    }
}