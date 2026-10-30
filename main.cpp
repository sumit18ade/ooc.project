#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>
#include <stdexcept>
using namespace std;

// ABSTRACT CLASS
class Person {
protected:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a) {}

    virtual void display() const = 0;
    virtual int getId() const = 0;

    string getName() const {
        return name;
    }

    virtual ~Person() {
        cout << "";
    }
};

// STUDENT CLASS: INHERITANCE + ENCAPSULATION
class Student : public Person {
private:
    int id;
    double marks;
    string course;
    static int totalCreated;

public:
    // Constructor overloading
    Student(string n, int a, int i, double m)
        : Person(n, a), id(i), marks(m), course("CSE") {
        totalCreated++;
    }

    Student(string n, int a, int i, double m, string c)
        : Person(n, a), id(i), marks(m), course(c) {
        totalCreated++;
    }

    // Function overriding
    void display() const override {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nAge: " << age
             << "\nCourse: " << course
             << "\nMarks: " << marks
             << "\nGrade: " << calculateGrade()
             << "\n";
    }

    int getId() const override {
        return id;
    }

    double getMarks() const {
        return marks;
    }

    string calculateGrade() const {
        if (marks >= 90) return "A+";
        if (marks >= 75) return "A";
        if (marks >= 60) return "B";
        if (marks >= 40) return "C";
        return "Fail";
    }

    static int getTotalCreated() {
        return totalCreated;
    }

    // OPERATOR OVERLOADING
    bool operator<(const Student& other) const {
        return marks < other.marks;
    }

    // FRIEND FUNCTION
    friend void showTopper(const Student&, const Student&);

    virtual bool scholarshipEligible() const {
        return marks >= 85;
    }

    virtual ~Student() {}
};

int Student::totalCreated = 0;

// MULTILEVEL / HIERARCHICAL EXTENSION
class ScholarshipStudent : public Student {
public:
    ScholarshipStudent(string n, int a, int i, double m,
                       string c)
        : Student(n, a, i, m, c) {}

    // Runtime polymorphism: overriding
    void display() const override {
        Student::display();
        cout << "Category: Scholarship Student\n";
        cout << "Scholarship: "
             << (scholarshipEligible() ? "Eligible" : "Not eligible")
             << "\n";
    }

    bool scholarshipEligible() const override {
        return getMarks() >= 75;
    }
};

// FRIEND FUNCTION accesses private marks
void showTopper(const Student& s1, const Student& s2) {
    cout << "\n--- Marks Comparison ---\n";

    if (s1.marks > s2.marks)
        cout << s1.name << " has higher marks.\n";
    else if (s2.marks > s1.marks)
        cout << s2.name << " has higher marks.\n";
    else
        cout << "Both students have equal marks.\n";
}

// FUNCTION OVERLOADING
void addStudent(vector<unique_ptr<Person> >& students,
                string n, int a, int id, double marks) {
    if (a <= 0 || marks < 0 || marks > 100)
        throw invalid_argument("Invalid age or marks!");

    students.push_back(
        unique_ptr<Person>(new Student(n, a, id, marks))
    );
}

void addStudent(vector<unique_ptr<Person> >& students,
                string n, int a, int id, double marks,
                string course) {
    if (a <= 0 || marks < 0 || marks > 100)
        throw invalid_argument("Invalid age or marks!");

    students.push_back(
        unique_ptr<Person>(
            new Student(n, a, id, marks, course)
        )
    );
}

void addScholarshipStudent(
    vector<unique_ptr<Person> >& students,
    string n, int a, int id, double marks, string course) {

    if (a <= 0 || marks < 0 || marks > 100)
        throw invalid_argument("Invalid age or marks!");

    students.push_back(
        unique_ptr<Person>(
            new ScholarshipStudent(n, a, id, marks, course)
        )
    );
}

int main() {
    vector<unique_ptr<Person> > students;
    int choice;

    do {
        cout << "\n===== STUDENT MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Student\n";
        cout << "2. Add Scholarship Student\n";
        cout << "3. Display All Students\n";
        cout << "4. Search Student by ID\n";
        cout << "5. Compare Two Students\n";
        cout << "6. Total Students Created\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        try {
            if (choice == 1 || choice == 2) {
                string n, course;
                int a, id;
                double marks;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter name: ";
                getline(cin, n);

                cout << "Enter age: ";
                if (!(cin >> a))
                    throw invalid_argument("Invalid age input!");

                cout << "Enter student ID: ";
                if (!(cin >> id))
                    throw invalid_argument("Invalid ID input!");

                cout << "Enter marks (0-100): ";
                if (!(cin >> marks))
                    throw invalid_argument("Invalid marks input!");

                if (choice == 1) {
                    cout << "Enter course: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, course);

                    if (course.empty())
                        throw invalid_argument("Course cannot be empty!");

                    addStudent(students, n, a, id, marks, course);
                } else {
                    cout << "Enter course: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, course);

                    if (course.empty())
                        throw invalid_argument("Course cannot be empty!");

                    addScholarshipStudent(
                        students, n, a, id, marks, course
                    );
                }

                cout << "Student added successfully!\n";
            }
            else if (choice == 3) {
                if (students.empty()) {
                    cout << "No students available.\n";
                } else {
                    for (size_t i = 0; i < students.size(); i++)
                        students[i]->display();
                }
            }
            else if (choice == 4) {
                int id;
                cout << "Enter student ID: ";

                if (!(cin >> id))
                    throw invalid_argument("Invalid ID input!");

                bool found = false;

                for (size_t i = 0; i < students.size(); i++) {
                    if (students[i]->getId() == id) {
                        students[i]->display();
                        found = true;
                        break;
                    }
                }

                if (!found)
                    cout << "Student not found.\n";
            }
            else if (choice == 5) {
                int id1, id2;
                Student* s1 = NULL;
                Student* s2 = NULL;

                cout << "Enter first student ID: ";
                if (!(cin >> id1))
                    throw invalid_argument("Invalid ID input!");

                cout << "Enter second student ID: ";
                if (!(cin >> id2))
                    throw invalid_argument("Invalid ID input!");

                for (size_t i = 0; i < students.size(); i++) {
                    Student* s = dynamic_cast<Student*>(students[i].get());

                    if (s && s->getId() == id1) s1 = s;
                    if (s && s->getId() == id2) s2 = s;
                }

                if (!s1 || !s2) {
                    cout << "One or both students were not found.\n";
                } else {
                    showTopper(*s1, *s2);

                    if (*s1 < *s2)
                        cout << s2->getName() << " ranks higher.\n";
                    else if (*s2 < *s1)
                        cout << s1->getName() << " ranks higher.\n";
                    else
                        cout << "Equal marks.\n";
                }
            }
            else if (choice == 6) {
                cout << "Student objects created: "
                     << Student::getTotalCreated() << "\n";
            }
            else if (choice == 0) {
                cout << "Thank you for using the system!\n";
            }
            else {
                cout << "Invalid choice. Try again.\n";
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
            if (cin.fail()) {
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );
            }
        }

    } while (choice != 0);

    return 0;
}
