#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Custom Exception Class for invalid marks
class InvalidMarksException : public runtime_error {
public:
    explicit InvalidMarksException(const string& msg) : runtime_error(msg) {}
};

// Base Class: Student
class Student {
protected:
    int rollNo;
    string name;

public:
    Student(int r = 0, const string& n = "Unknown") : rollNo(r), name(n) {}

    virtual ~Student() {}

    int getRollNo() const { return rollNo; }
    string getName() const { return name; }

    // Virtual function for runtime polymorphism
    virtual void displayRecord() const {
        cout << left << setw(10) << rollNo << setw(20) << name;
    }
};

// Derived Class: Result (inherits from Student)
class Result : public Student {
private:
    float marks[3];
    float total;
    float percentage;
    string grade;

    // Validate marks (must be between 0 and 100)
    void validateMarks(float m1, float m2, float m3) const {
        if (m1 < 0.0f || m1 > 100.0f ||
            m2 < 0.0f || m2 > 100.0f ||
            m3 < 0.0f || m3 > 100.0f) {
            throw InvalidMarksException("Marks must be within the valid range of 0 to 100!");
        }
    }

    void computeResult() {
        total = marks[0] + marks[1] + marks[2];
        percentage = total / 3.0f;

        if (percentage >= 90.0f) grade = "O";
        else if (percentage >= 80.0f) grade = "A+";
        else if (percentage >= 70.0f) grade = "A";
        else if (percentage >= 60.0f) grade = "B+";
        else if (percentage >= 50.0f) grade = "B";
        else grade = "F";
    }

public:
    Result(int r = 0, const string& n = "Unknown", float m1 = 0.0f, float m2 = 0.0f, float m3 = 0.0f)
        : Student(r, n) {
        validateMarks(m1, m2, m3);
        marks[0] = m1;
        marks[1] = m2;
        marks[2] = m3;
        computeResult();
    }

    float getPercentage() const { return percentage; }
    float getTotal() const { return total; }
    string getGrade() const { return grade; }

    // Overridden virtual function
    void displayRecord() const override {
        Student::displayRecord();
        cout << left << setw(8) << fixed << setprecision(1) << marks[0]
             << setw(8) << marks[1]
             << setw(8) << marks[2]
             << setw(9) << total
             << setw(10) << setprecision(2) << percentage
             << grade << endl;
    }

    // Serialize record to CSV format for file storage
    string toFileString() const {
        ostringstream oss;
        oss << rollNo << "," << name << ","
            << marks[0] << "," << marks[1] << "," << marks[2];
        return oss.str();
    }

    // Deserialize record from CSV string
    static Result fromFileString(const string& line) {
        stringstream ss(line);
        string item;
        vector<string> tokens;
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }
        if (tokens.size() != 5) {
            throw runtime_error("Corrupted record format in file!");
        }
        int r = stoi(tokens[0]);
        string n = tokens[1];
        float m1 = stof(tokens[2]);
        float m2 = stof(tokens[3]);
        float m3 = stof(tokens[4]);
        return Result(r, n, m1, m2, m3);
    }
};

// Save all Result records to file
void saveRecordsToFile(const vector<Result>& records, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        throw runtime_error("Unable to open file for writing: " + filename);
    }
    for (const auto& rec : records) {
        outFile << rec.toFileString() << "\n";
    }
    outFile.close();
}

// Load Result records from file
vector<Result> loadRecordsFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Unable to open file for reading: " + filename);
    }
    vector<Result> loaded;
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            loaded.push_back(Result::fromFileString(line));
        }
    }
    inFile.close();
    return loaded;
}

void printTableHeader() {
    cout << left << setw(10) << "Roll No"
         << setw(20) << "Student Name"
         << setw(8) << "Sub 1"
         << setw(8) << "Sub 2"
         << setw(8) << "Sub 3"
         << setw(9) << "Total"
         << setw(10) << "Percent"
         << "Grade" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}

int main() {
    const string filename = "student_results.txt";
    vector<Result> records;

    cout << "================================================================================" << endl;
    cout << "              STUDENT RESULT MANAGEMENT SYSTEM (OOP C++ - TASK 2)               " << endl;
    cout << "================================================================================" << endl;

    // 1. Adding valid records and demonstrating Exception Handling for invalid marks
    cout << "\n--- Step 1: Adding Student Records & Validating Marks ---" << endl;
    try {
        records.emplace_back(101, "Varshini Noora", 92.0f, 89.5f, 94.0f);
        records.emplace_back(102, "Ananya Sharma", 84.0f, 88.0f, 86.5f);
        records.emplace_back(103, "Rohan Verma", 72.5f, 76.0f, 74.0f);
        records.emplace_back(104, "Karthik Rao", 95.0f, 91.5f, 93.0f);
        records.emplace_back(105, "Meera Nair", 64.0f, 68.5f, 66.0f);
        cout << "Successfully added " << records.size() << " valid student records." << endl;

        cout << "Attempting to add record with invalid marks (108.0)..." << endl;
        records.emplace_back(106, "Test Student", 108.0f, 85.0f, 90.0f);
    } catch (const InvalidMarksException& e) {
        cout << "[Exception Caught] " << e.what() << endl;
    }

    // 2. File I/O: Save records to file and reload from file
    cout << "\n--- Step 2: Saving Records to File & Loading Back ---" << endl;
    try {
        saveRecordsToFile(records, filename);
        cout << "Records saved to '" << filename << "'." << endl;

        vector<Result> loadedRecords = loadRecordsFromFile(filename);
        cout << "Loaded " << loadedRecords.size() << " records from '" << filename << "':\n" << endl;

        // Polymorphic display via base class pointer
        printTableHeader();
        for (const auto& rec : loadedRecords) {
            const Student* basePtr = &rec;
            basePtr->displayRecord();
        }

        // 3. STL Algorithm + Lambda Expression: Sort descending by percentage
        cout << "\n--- Step 3: Sorting Records by Percentage (STL sort + Lambda) ---" << endl;
        sort(loadedRecords.begin(), loadedRecords.end(), [](const Result& a, const Result& b) {
            return a.getPercentage() > b.getPercentage();
        });
        printTableHeader();
        for (const auto& rec : loadedRecords) {
            rec.displayRecord();
        }

        // 4. STL Algorithm + Lambda Expression: Filter distinction students (>= 85%)
        float threshold = 85.0f;
        cout << "\n--- Step 4: Filtering Distinction Students (Percentage >= 85%) ---" << endl;
        int distCount = count_if(loadedRecords.begin(), loadedRecords.end(), [threshold](const Result& r) {
            return r.getPercentage() >= threshold;
        });
        cout << "Total Distinction Students Found: " << distCount << "\n" << endl;

        printTableHeader();
        for_each(loadedRecords.begin(), loadedRecords.end(), [threshold](const Result& r) {
            if (r.getPercentage() >= threshold) {
                r.displayRecord();
            }
        });

    } catch (const exception& e) {
        cout << "[File/Runtime Error] " << e.what() << endl;
    }

    cout << "\n================================================================================" << endl;
    return 0;
}