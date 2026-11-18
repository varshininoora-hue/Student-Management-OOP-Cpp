#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

class Student {
private:
    int rollNo;
    char* name;
    float marks[3];
    float totalMarks;
    float percentage;

    // Static member to track total active Student objects
    static int studentCount;

    // Private helper method to compute total and percentage
    void calculateResult() {
        totalMarks = marks[0] + marks[1] + marks[2];
        percentage = totalMarks / 3.0f;
    }

public:
    // Default Constructor
    Student() {
        rollNo = 0;
        name = new char[8];
        strcpy(name, "Unknown");
        marks[0] = marks[1] = marks[2] = 0.0f;
        totalMarks = 0.0f;
        percentage = 0.0f;
        studentCount++;
    }

    // Parameterized Constructor (with Dynamic Memory Allocation)
    Student(int r, const char* n, float m1, float m2, float m3) {
        rollNo = r;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        marks[0] = m1;
        marks[1] = m2;
        marks[2] = m3;
        calculateResult();
        studentCount++;
    }

    // Copy Constructor (Deep Copy)
    Student(const Student& other) {
        rollNo = other.rollNo;
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        for (int i = 0; i < 3; i++) {
            marks[i] = other.marks[i];
        }
        totalMarks = other.totalMarks;
        percentage = other.percentage;
        studentCount++;
    }

    // Destructor to release dynamically allocated memory
    ~Student() {
        delete[] name;
        studentCount--;
    }

    // Function Overloading: updateMarks()
    // Overload 1: Update marks for all three subjects
    void updateMarks(float m1, float m2, float m3) {
        marks[0] = m1;
        marks[1] = m2;
        marks[2] = m3;
        calculateResult();
    }

    // Overload 2: Update marks for a single subject (subjectIndex: 1 to 3)
    void updateMarks(int subjectIndex, float newMark) {
        if (subjectIndex >= 1 && subjectIndex <= 3) {
            marks[subjectIndex - 1] = newMark;
            calculateResult();
        } else {
            cout << "Invalid subject index!" << endl;
        }
    }

    // Overload 3: Apply uniform grace marks across all subjects (capped at 100)
    void updateMarks(float graceMarks) {
        for (int i = 0; i < 3; i++) {
            marks[i] = (marks[i] + graceMarks > 100.0f) ? 100.0f : (marks[i] + graceMarks);
        }
        calculateResult();
    }

    // Overloaded '>' operator to compare students based on total marks
    bool operator>(const Student& other) const {
        return this->totalMarks > other.totalMarks;
    }

    // Static member function to return active student count
    static int getStudentCount() {
        return studentCount;
    }

    // Friend function declaration to access private members for formatted output
    friend void displayStudentReport(const Student& s);
};

// Definition and initialization of static data member
int Student::studentCount = 0;

// Friend function definition
void displayStudentReport(const Student& s) {
    cout << left << setw(10) << s.rollNo
         << setw(20) << s.name
         << setw(8) << fixed << setprecision(1) << s.marks[0]
         << setw(8) << s.marks[1]
         << setw(8) << s.marks[2]
         << setw(10) << s.totalMarks
         << setprecision(2) << s.percentage << "%" << endl;
}

int main() {
    cout << "==========================================================================" << endl;
    cout << "       STUDENT MANAGEMENT & PERFORMANCE COMPARISON SYSTEM (OOP C++)       " << endl;
    cout << "==========================================================================" << endl;

    cout << "\nInitial Active Student Count: " << Student::getStudentCount() << endl;

    // Creating Student objects using Parameterized Constructor
    Student s1(101, "Varshini Noora", 88.5f, 92.0f, 85.5f);
    Student s2(102, "Ananya Sharma", 84.0f, 89.5f, 90.0f);
    Student s3(103, "Rohan Verma", 78.0f, 81.5f, 80.0f);

    cout << "Active Student Count after creating 3 objects: " << Student::getStudentCount() << endl;

    // Displaying initial records using Friend Function
    cout << "\n--- Initial Student Academic Records ---" << endl;
    cout << left << setw(10) << "Roll No"
         << setw(20) << "Student Name"
         << setw(8) << "Sub 1"
         << setw(8) << "Sub 2"
         << setw(8) << "Sub 3"
         << setw(10) << "Total"
         << "Percentage" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    displayStudentReport(s1);
    displayStudentReport(s2);
    displayStudentReport(s3);

    // Demonstrating Function Overloading (updateMarks)
    cout << "\n--- Demonstrating Function Overloading (Updating Marks) ---" << endl;
    cout << "1. Updating Subject 2 marks of Rohan Verma (Roll 103) to 91.0..." << endl;
    s3.updateMarks(2, 91.0f);

    cout << "2. Adding 2.5 grace marks to all subjects of Ananya Sharma (Roll 102)..." << endl;
    s2.updateMarks(2.5f);

    cout << "\n--- Updated Student Academic Records ---" << endl;
    cout << left << setw(10) << "Roll No"
         << setw(20) << "Student Name"
         << setw(8) << "Sub 1"
         << setw(8) << "Sub 2"
         << setw(8) << "Sub 3"
         << setw(10) << "Total"
         << "Percentage" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    displayStudentReport(s1);
    displayStudentReport(s2);
    displayStudentReport(s3);

    // Demonstrating Operator Overloading ('>') to compare students
    cout << "\n--- Comparing Students Using Overloaded '>' Operator ---" << endl;
    if (s1 > s2) {
        cout << "Varshini Noora scored higher than Ananya Sharma." << endl;
    } else {
        cout << "Ananya Sharma scored higher than or equal to Varshini Noora." << endl;
    }

    if (s2 > s3) {
        cout << "Ananya Sharma scored higher than Rohan Verma." << endl;
    } else {
        cout << "Rohan Verma scored higher than or equal to Ananya Sharma." << endl;
    }

    cout << "\n==========================================================================" << endl;
    return 0;
}