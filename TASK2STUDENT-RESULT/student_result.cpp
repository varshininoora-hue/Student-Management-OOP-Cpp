#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

// ============================================================================
// Function Template: Displays any STL container of records cleanly
// ============================================================================
template <typename Container>
void printSectionBanner(const string& title, const Container& items) {
    cout << "\n==================== " << title << " (Count: " << items.size() << ") ====================\n";
}

// ============================================================================
// Base Class: Student (Inheritance & Runtime Polymorphism)
// ============================================================================
class Student {
protected:
    string usn;
    string name;
    string branch;

public:
    Student(const string& u = "", const string& n = "", const string& b = "ECE")
        : usn(u), name(n), branch(b) {}

    virtual ~Student() {}

    // Virtual function overridden by derived class Result
    virtual void display() const {
        cout << left << setw(13) << usn
             << setw(20) << name
             << setw(8) << branch;
    }

    string getUSN() const { return usn; }
    string getName() const { return name; }
    string getBranch() const { return branch; }
};

// ============================================================================
// Derived Class: Result (Single Inheritance, Polymorphism & Exception Handling)
// ============================================================================
class Result : public Student {
private:
    double m1, m2, m3; // Marks in 3 subjects (out of 100 each)

    void validateMark(double mark, const string& subject) const {
        if (mark < 0.0 || mark > 100.0) {
            throw out_of_range("Invalid mark (" + to_string(mark) + ") in " + subject + ". Must be between 0 and 100.");
        }
    }

public:
    Result(const string& u, const string& n, const string& b, double s1, double s2, double s3)
        : Student(u, n, b), m1(s1), m2(s2), m3(s3) {
        validateMark(m1, "Subject 1 (OOPS)");
        validateMark(m2, "Subject 2 (Signals)");
        validateMark(m3, "Subject 3 (Digital Logic)");
    }

    double getTotal() const { return m1 + m2 + m3; }
    double getPercentage() const { return getTotal() / 3.0; }

    string getGradeStatus() const {
        if (m1 < 35.0 || m2 < 35.0 || m3 < 35.0) return "FAIL";
        double pct = getPercentage();
        if (pct >= 75.0) return "DISTINCTION";
        if (pct >= 60.0) return "FIRST CLASS";
        return "PASS";
    }

    // Overridden virtual function to display full student details, marks & result
    void display() const override {
        Student::display();
        cout << right << fixed << setprecision(1)
             << setw(7) << m1
             << setw(7) << m2
             << setw(7) << m3
             << setw(9) << getTotal()
             << setw(8) << getPercentage() << "%   "
             << left << getGradeStatus() << "\n";
    }

    // Serialize record to CSV format for file storage
    string toCSV() const {
        ostringstream oss;
        oss << usn << "," << name << "," << branch << "," << m1 << "," << m2 << "," << m3;
        return oss.str();
    }

    // Deserialize record from CSV line
    static Result fromCSV(const string& line) {
        stringstream ss(line);
        string u, n, b, s1Str, s2Str, s3Str;
        getline(ss, u, ',');
        getline(ss, n, ',');
        getline(ss, b, ',');
        getline(ss, s1Str, ',');
        getline(ss, s2Str, ',');
        getline(ss, s3Str, ',');
        return Result(u, n, b, stod(s1Str), stod(s2Str), stod(s3Str));
    }
};

// ============================================================================
// File Handling: Save & Retrieve Student Result Records
// ============================================================================
void saveRecordsToFile(const vector<Result>& records, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        throw runtime_error("Unable to open file for writing: " + filename);
    }
    for (const auto& rec : records) {
        outFile << rec.toCSV() << "\n";
    }
    outFile.close();
    cout << "[File Saved] " << records.size() << " student records written to '" << filename << "'.\n";
}

vector<Result> loadRecordsFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Unable to open file for reading: " + filename);
    }
    vector<Result> loaded;
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            loaded.push_back(Result::fromCSV(line));
        }
    }
    inFile.close();
    cout << "[File Loaded] " << loaded.size() << " student records retrieved from '" << filename << "'.\n";
    return loaded;
}

void printTableHeader() {
    cout << left << setw(13) << "USN"
         << setw(20) << "Student Name"
         << setw(8) << "Branch"
         << right << setw(7) << "Sub1"
         << setw(7) << "Sub2"
         << setw(7) << "Sub3"
         << setw(9) << "Total"
         << setw(9) << "Percent"
         << "   " << left << "Result\n";
    cout << string(92, '-') << "\n";
}

int main() {
    cout << "===== STUDENT RESULT MANAGEMENT SYSTEM (OOPS C++ TASK 2) =====\n";

    vector<Result> records;
    const string filename = "student_results.txt";

    // 1. Add valid student records (demonstrating parameterized constructors & STL vector)
    try {
        records.emplace_back("NB25ECE196", "Varshini Noora", "ECE", 92.0, 88.5, 95.0);
        records.emplace_back("NB25ECE194", "Vaishnavi B", "ECE", 85.0, 81.0, 89.5);
        records.emplace_back("NB25ECE195", "Varshitha V", "ECE", 78.0, 74.5, 80.0);
        records.emplace_back("NB25ECE180", "Rohan Kulkarni", "ECE", 64.0, 59.0, 68.5);
        records.emplace_back("NB25ECE188", "Ananya Rao", "ECE", 94.5, 91.0, 96.0);
    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << "\n";
    }

    // 2. Demonstrate Exception Handling for Invalid Marks (> 100 or < 0)
    cout << "\n--- Testing Exception Handling for Invalid Marks ---\n";
    try {
        cout << "Attempting to add record with mark = 108.0...\n";
        Result invalidStudent("NB25ECE199", "Test Invalid", "ECE", 108.0, 75.0, 80.0);
        records.push_back(invalidStudent);
    } catch (const out_of_range& ex) {
        cout << "[Exception Caught] " << ex.what() << "\n";
    }

    // Optional interactive entry from user
    char choice;
    cout << "\nDo you want to add another student record manually? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        try {
            string u, n, b;
            double s1, s2, s3;
            cout << "Enter USN: ";
            cin >> u;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, n);
            cout << "Enter Branch: ";
            cin >> b;
            cout << "Enter 3 Subject Marks (0-100): ";
            cin >> s1 >> s2 >> s3;
            records.emplace_back(u, n, b, s1, s2, s3);
            cout << "[Success] Student record added!\n";
        } catch (const exception& ex) {
            cout << "[Exception Caught] " << ex.what() << "\n";
        }
    }

    // 3. File Handling: Store records to file and retrieve them back
    cout << "\n--- File Handling: Saving & Retrieving Records ---\n";
    saveRecordsToFile(records, filename);
    vector<Result> retrievedRecords = loadRecordsFromFile(filename);

    // 4. Runtime Polymorphism: Display all retrieved records via Base Class pointers (Student*)
    printSectionBanner("ALL STUDENT RESULTS (VIA POLYMORPHIC BASE POINTERS)", retrievedRecords);
    printTableHeader();
    vector<Student*> polyPtrs;
    for (auto& rec : retrievedRecords) {
        polyPtrs.push_back(&rec);
    }
    for (const Student* ptr : polyPtrs) {
        ptr->display(); // Calls Result::display() dynamically at runtime
    }

    // 5. STL Algorithm (std::count_if & std::copy_if) + Lambda Expression to filter high scorers
    double threshold = 250.0;
    cout << "\nEnter minimum total marks (out of 300) to filter top scorers (e.g. 250): ";
    cin >> threshold;

    // Lambda expression capturing threshold by value
    auto isAboveThreshold = [threshold](const Result& r) {
        return r.getTotal() >= threshold;
    };

    int highCount = count_if(retrievedRecords.begin(), retrievedRecords.end(), isAboveThreshold);
    vector<Result> topScorers;
    copy_if(retrievedRecords.begin(), retrievedRecords.end(), back_inserter(topScorers), isAboveThreshold);

    // Sort top scorers in descending order of total marks using std::sort + Lambda
    sort(topScorers.begin(), topScorers.end(), [](const Result& a, const Result& b) {
        return a.getTotal() > b.getTotal();
    });

    printSectionBanner("STUDENTS SCORING >= " + to_string((int)threshold) + " MARKS (STL + LAMBDA)", topScorers);
    cout << "Total qualifying students found using std::count_if: " << highCount << "\n\n";
    if (!topScorers.empty()) {
        printTableHeader();
        for (const auto& top : topScorers) {
            top.display();
        }
    } else {
        cout << "No students scored above " << threshold << " marks.\n";
    }

    return 0;
}