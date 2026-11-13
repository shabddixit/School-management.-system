
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

// Base Class
class Person {
protected:
    int id;
    string name;

public:
    Person(int personId, const string& personName)
        : id(personId), name(personName) {}

    virtual void displayDetails() const = 0;

    virtual ~Person() = default;

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }
};

// Student Class
class Student : public Person {
private:
    int age;
    string className;
    double marks1, marks2, marks3;

public:
    Student(int studentId, const string& studentName,
            int studentAge, const string& studentClass,
            double m1, double m2, double m3)
        : Person(studentId, studentName),
          age(studentAge),
          className(studentClass),
          marks1(m1), marks2(m2), marks3(m3) {}

    double percentage() const {
        return (marks1 + marks2 + marks3) / 3.0;
    }

    string grade() const {
        double p = percentage();

        if (p >= 80) return "A";
        if (p >= 70) return "B";
        if (p >= 60) return "C";
        if (p >= 40) return "D";

        return "F";
    }

    void displayDetails() const override {
        cout << "\nStudent ID : " << id
             << "\nName       : " << name
             << "\nAge        : " << age
             << "\nClass      : " << className
             << "\nPercentage : " << fixed
             << setprecision(2) << percentage() << "%"
             << "\nGrade      : " << grade() << "\n";
    }

    void save(ofstream& out) const {
        out << id << '\n'
            << name << '\n'
            << age << '\n'
            << className << '\n'
            << marks1 << '\n'
            << marks2 << '\n'
            << marks3 << '\n';
    }
};

// Teacher Class
class Teacher : public Person {
private:
    string subject;
    double salary;

public:
    Teacher(int teacherId, const string& teacherName,
            const string& teacherSubject, double teacherSalary)
        : Person(teacherId, teacherName),
          subject(teacherSubject),
          salary(teacherSalary) {}

    void displayDetails() const override {
        cout << "\nTeacher ID : " << id
             << "\nName       : " << name
             << "\nSubject    : " << subject
             << "\nSalary     : " << fixed
             << setprecision(2) << salary << "\n";
    }

    void save(ofstream& out) const {
        out << id << '\n'
            << name << '\n'
            << subject << '\n'
            << salary << '\n';
    }
};

// School Management Class
class School {
private:
    vector<Student> students;
    vector<Teacher> teachers;

    bool studentIdExists(int studentId) const {
        for (const Student& s : students) {
            if (s.getId() == studentId)
                return true;
        }
        return false;
    }

    bool teacherIdExists(int teacherId) const {
        for (const Teacher& t : teachers) {
            if (t.getId() == teacherId)
                return true;
        }
        return false;
    }

    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static int readInt(const string& prompt) {
        int value;

        while (true) {
            cout << prompt;

            if (cin >> value) {
                clearInput();
                return value;
            }

            cout << "Invalid input. Enter a whole number.\n";
            clearInput();
        }
    }

    static double readDouble(const string& prompt) {
        double value;

        while (true) {
            cout << prompt;

            if (cin >> value) {
                clearInput();
                return value;
            }

            cout << "Invalid input. Enter a number.\n";
            clearInput();
        }
    }

    static string readLine(const string& prompt) {
        string value;
        cout << prompt;
        getline(cin, value);
        return value;
    }

    static double readMarks(const string& prompt) {
        double marks;

        do {
            marks = readDouble(prompt);

            if (marks < 0 || marks > 100)
                cout << "Marks must be between 0 and 100.\n";

        } while (marks < 0 || marks > 100);

        return marks;
    }

    // Save student records
    void saveStudents() const {
        ofstream out("students.txt");

        if (!out) {
            cout << "Could not save student records.\n";
            return;
        }

        for (const Student& s : students)
            s.save(out);
    }

    // Save teacher records
    void saveTeachers() const {
        ofstream out("teachers.txt");

        if (!out) {
            cout << "Could not save teacher records.\n";
            return;
        }

        for (const Teacher& t : teachers)
            t.save(out);
    }

    // Load student records
    void loadStudents() {
        ifstream in("students.txt");

        if (!in) return;

        int studentId, age;
        string studentName, studentClass;
        double m1, m2, m3;

        while (in >> studentId) {
            in.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!getline(in, studentName)) break;
            if (!(in >> age)) break;

            in.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!getline(in, studentClass)) break;
            if (!(in >> m1 >> m2 >> m3)) break;

            in.ignore(numeric_limits<streamsize>::max(), '\n');

            students.emplace_back(
                studentId, studentName, age,
                studentClass, m1, m2, m3
            );
        }
    }

    // Load teacher records
    void loadTeachers() {
        ifstream in("teachers.txt");

        if (!in) return;

        int teacherId;
        string teacherName, subject;
        double salary;

        while (in >> teacherId) {
            in.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!getline(in, teacherName)) break;
            if (!getline(in, subject)) break;
            if (!(in >> salary)) break;

            in.ignore(numeric_limits<streamsize>::max(), '\n');

            teachers.emplace_back(
                teacherId, teacherName, subject, salary
            );
        }
    }

public:

    // Constructor
    School() {
        loadStudents();
        loadTeachers();
    }

    // Add Student
    void addStudent() {
        int studentId = readInt("Enter student ID: ");

        if (studentIdExists(studentId)) {
            cout << "A student with this ID already exists.\n";
            return;
        }

        string studentName = readLine("Enter student name: ");
        int age = readInt("Enter age: ");

        string studentClass = readLine("Enter class/grade: ");

        double m1 = readMarks("Enter marks in subject 1 (0-100): ");
        double m2 = readMarks("Enter marks in subject 2 (0-100): ");
        double m3 = readMarks("Enter marks in subject 3 (0-100): ");

        students.emplace_back(
            studentId, studentName, age,
            studentClass, m1, m2, m3
        );

        saveStudents();

        cout << "Student added successfully.\n";
    }

    // Display Students
    void showStudents() const {
        if (students.empty()) {
            cout << "No student records found.\n";
            return;
        }

        for (const Student& s : students)
            s.displayDetails();
    }

    // Search Student
    void searchStudent() const {
        int wantedId = readInt("Enter student ID to search: ");

        for (const Student& s : students) {
            if (s.getId() == wantedId) {
                s.displayDetails();
                return;
            }
        }

        cout << "Student not found.\n";
    }

    // Add Teacher
    void addTeacher() {
        int teacherId = readInt("Enter teacher ID: ");

        if (teacherIdExists(teacherId)) {
            cout << "A teacher with this ID already exists.\n";
            return;
        }

        string teacherName = readLine("Enter teacher name: ");
        string subject = readLine("Enter subject taught: ");

        double salary;

        do {
            salary = readDouble("Enter monthly salary: ");

            if (salary < 0)
                cout << "Salary cannot be negative.\n";

        } while (salary < 0);

        teachers.emplace_back(
            teacherId, teacherName, subject, salary
        );

        saveTeachers();

        cout << "Teacher added successfully.\n";
    }

    // Display Teachers
    void showTeachers() const {
        if (teachers.empty()) {
            cout << "No teacher records found.\n";
            return;
        }

        for (const Teacher& t : teachers)
            t.displayDetails();
    }

    // Main Menu
    void run() {
        int choice;

        do {
            cout << "\n========== SCHOOL MANAGEMENT SYSTEM ==========\n"
                 << "1. Add student\n"
                 << "2. Display all students\n"
                 << "3. Search student by ID\n"
                 << "4. Add teacher\n"
                 << "5. Display all teachers\n"
                 << "0. Exit\n"
                 << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid choice. Please enter a number.\n";
                clearInput();
                continue;
            }

            clearInput();

            switch (choice) {
                case 1:
                    addStudent();
                    break;

                case 2:
                    showStudents();
                    break;

                case 3:
                    searchStudent();
                    break;

                case 4:
                    addTeacher();
                    break;

                case 5:
                    showTeachers();
                    break;

                case 0:
                    cout << "Thank you for using the system.\n";
                    break;

                default:
                    cout << "Invalid choice. Try again.\n";
            }

        } while (choice != 0);
    }
};

// Main Function
int main() {
    School school;
    school.run();

    return 0;
}
