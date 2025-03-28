#include <iostream>
using namespace std;

class Queue
{
private:
    string *data;
    int front;
    int rear;
    int capacity;

public:
    Queue(int size = 100)
    {
        data = new string[size];
        front = -1;
        rear = -1;
        capacity = size;
    }

    ~Queue()
    {
        delete[] data;
    }

    bool isEmpty() const
    {
        return front == -1;
    }

    bool isFull() const
    {
        return (rear + 1) % capacity == front;
    }

    void enqueue(string studentID)
    {
        if (isFull())
        {
            cout << "Queue is full. Cannot enqueue " << studentID << endl;
            return;
        }
        if (isEmpty())
        {
            front = rear = 0;
        }
        else
        {
            rear = (rear + 1) % capacity;
        }
        data[rear] = studentID;
    }

    string dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue is empty. Nothing to dequeue." << endl;
            return "";
        }
        string studentID = data[front];
        if (front == rear)
        {
            front = rear = -1;
        }
        else
        {
            front = (front + 1) % capacity;
        }
        return studentID;
    }
};

class tnode
{
public:
    string coursename;
    int capacity;
    int spaceleft;
    tnode *left;
    tnode *right;
    Queue waitlist;

    tnode(string name, int cap) : waitlist(100)
    {
        coursename = name;
        capacity = cap;
        spaceleft = cap;
        left = nullptr;
        right = nullptr;
    }
};

class tree
{
private:
    tnode *root;

    tnode *searchHelper(tnode *current, string key)
    {
        if (current == nullptr || current->coursename == key)
        {
            return current;
        }
        tnode *result = searchHelper(current->left, key);
        if (result != nullptr)
        {
            return result;
        }
        return searchHelper(current->right, key);
    }

public:
    tree()
    {
        root = nullptr;
    }

    tnode *search(string key)
    {
        return searchHelper(root, key);
    }

    void insert(string pname, string cname, int cap)
    {
        if (root == nullptr)
        {
            root = new tnode(cname, cap);
            cout << "Root added: " << cname << endl;
        }
        else
        {
            tnode *parent = search(pname);
            if (parent)
            {
                if (parent->left == nullptr)
                {
                    parent->left = new tnode(cname, cap);
                    cout << "Added " << cname << " as the left child of " << pname << endl;
                }
                else if (parent->right == nullptr)
                {
                    parent->right = new tnode(cname, cap);
                    cout << "Added " << cname << " as the right child of " << pname << endl;
                }
                else
                {
                    cout << "Parent " << pname << " already has two children. Cannot add more." << endl;
                }
            }
            else
            {
                cout << "Parent node not found: " << pname << endl;
            }
        }
    }

    void processRegistrations()
    {
        processNode(root);
    }

    void processNode(tnode *node)
    {
        if (node == nullptr)
            return;

        while (!node->waitlist.isEmpty())
        {
            string studentID = node->waitlist.dequeue();
            if (node->spaceleft > 0)
            {
                node->spaceleft--;
                cout << "Student " << studentID << " successfully registered in " << node->coursename << endl;
            }
            else
            {
                cout << "No space left in " << node->coursename << " for student " << studentID << endl;
            }
        }

        processNode(node->left);
        processNode(node->right);
    }
};

class UndoStack
{
private:
    string actions[100];
    string courses[100];
    int top;

public:
    UndoStack() : top(-1) {}

    void push(string action, string course)
    {
        if (top >= 99)
        {
            cout << "Undo stack is full." << endl;
            return;
        }
        top++;
        actions[top] = action;
        courses[top] = course;
    }

    bool isEmpty() const
    {
        return top == -1;
    }

    bool undo(tree &courseTree)
    {
        if (isEmpty())
        {
            cout << "No actions to undo." << endl;
            return false;
        }

        string action = actions[top];
        string courseName = courses[top];
        top--;

        tnode *course = courseTree.search(courseName);
        if (!course)
        {
            cout << "Course not found: " << courseName << endl;
            return false;
        }

        if (action == "register")
        {
            if (course->spaceleft < course->capacity)
            {
                course->spaceleft++;
                cout << "Undone registration for course: " << courseName << endl;
            }
        }
        return true;
    }
};

class CourseNode
{
public:
    string CourseName;
    int CourseID;
    CourseNode *next;
    CourseNode(string CourseName, int CourseID)
    {
        this->CourseName = CourseName;
        this->CourseID = CourseID;
        next = nullptr;
    }
};

class CourseList
{
public:
    CourseNode *head;
    CourseList()
    {
        head = nullptr;
    }
    void add(string CourseName, int CourseID)
    {
        CourseNode *newCourse = new CourseNode(CourseName, CourseID);
        if (head == nullptr)
        {
            head = newCourse;
        }
        else
        {
            CourseNode *temp = head;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newCourse;
        }
    }
    void display()
    {
        CourseNode *temp = head;
        while (temp != nullptr)
        {
            cout << "Course Name: " << temp->CourseName << ", Course ID: " << temp->CourseID << endl;
            temp = temp->next;
        }
    }
};

class Student
{
public:
    string StudentName;
    int StudentID;
    CourseList courses;
    Student(string StudentName, int StudentID)
    {
        this->StudentName = StudentName;
        this->StudentID = StudentID;
    }
    void registerCourse(string CourseName, int CourseID)
    {
        courses.add(CourseName, CourseID);
    }
    void displayStudentInfo()
    {
        cout << "Courses registered for student " << StudentName << " (ID: " << StudentID << "):" << endl;
        courses.display();
    }
};

class Menu
{
public:
    void teacherMenu(tree &courseTree)
{
    int choice;
    do
    {
        cout << "\n--- Teacher Menu ---\n";
        cout << "1. Add Course\n";
        cout << "2. Process Registrations\n";
        cout << "3. Search for a Course\n";
        cout << "4. View Course Waitlist\n";
        cout << "5. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string parentCourse, courseName;
            int capacity;
            cout << "Enter parent course name (or leave empty for root): ";
            cin >> parentCourse;
            cout << "Enter new course name: ";
            cin >> courseName;
            cout << "Enter capacity for the course: ";
            cin >> capacity;
            courseTree.insert(parentCourse, courseName, capacity);
            break;
        }
        case 2:
            courseTree.processRegistrations();
            break;
        case 3:
        {
            string courseName;
            cout << "Enter course name to search: ";
            cin >> courseName;
            tnode *course = courseTree.search(courseName);
            if (course)
            {
                cout << "Course Found: " << course->coursename << "\n";
                cout << "Capacity: " << course->capacity << ", Space Left: " << course->spaceleft << "\n";
            }
            else
            {
                cout << "Course not found.\n";
            }
            break;
        }
        case 4:
        {
            string courseName;
            cout << "Enter course name to view waitlist: ";
            cin >> courseName;
            tnode *course = courseTree.search(courseName);
            if (course)
            {
                if (course->waitlist.isEmpty())
                {
                    cout << "The waitlist for " << courseName << " is empty.\n";
                }
                else
                {
                    cout << "Waitlist for " << courseName << ":\n";
                    while (!course->waitlist.isEmpty())
                    {
                        string studentID = course->waitlist.dequeue();
                        cout << studentID << endl;
                    }
                }
            }
            else
            {
                cout << "Course not found.\n";
            }
            break;
        }
        case 5:
            cout << "Exiting to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);
}

    void studentMenu(tree &courseTree, UndoStack &undoStack)
    {
        Student student("", 0);
        cout << "Enter your name: ";
        cin >> student.StudentName;
        cout << "Enter your ID: ";
        cin >> student.StudentID;

        int choice;
        do
        {
            cout << "\n--- Student Menu ---\n";
            cout << "1. Register for a Course\n";
            cout << "2. View Registered Courses\n";
            cout << "3. Search for a Course\n";
            cout << "4. Add to Waitlist for a Course\n"; 
            cout << "5. Undo Last Registration\n";
            cout << "6. Exit to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                string courseName;
                cout << "Enter course name to register: ";
                cin >> courseName;
                tnode *course = courseTree.search(courseName);
                if (course)
                {
                    if (course->spaceleft > 0)
                    {
                        course->spaceleft--;
                        student.registerCourse(courseName, student.StudentID);
                        undoStack.push("register", courseName);
                        cout << "Successfully registered for " << courseName << ".\n";
                    }
                    else
                    {
                        cout << "Course full! Adding to the waitlist.\n";
                        course->waitlist.enqueue(to_string(student.StudentID));
                    }
                }
                else
                {
                    cout << "Course not found.\n";
                }
                break;
            }
            case 2:
                student.displayStudentInfo();
                break;
            case 3:
            {
                string courseName;
                cout << "Enter course name to search: ";
                cin >> courseName;
                tnode *course = courseTree.search(courseName);
                if (course)
                {
                    cout << "Course Found: " << course->coursename << "\n";
                    cout << "Capacity: " << course->capacity << ", Space Left: " << course->spaceleft << "\n";
                }
                else
                {
                    cout << "Course not found.\n";
                }
                break;
            }
            case 4:
            {
                string courseName;
                cout << "Enter course name to add to waitlist: ";
                cin >> courseName;
                tnode *course = courseTree.search(courseName);
                if (course)
                {
                    cout << "Adding to the waitlist for " << courseName << ".\n";
                    course->waitlist.enqueue(student.StudentName);
                }
                else
                {
                    cout << "Course not found.\n";
                }
                break;
            }
            case 5:
                if (!undoStack.undo(courseTree))
                {
                    cout << "No actions to undo.\n";
                }
                break;
            case 6:
                cout << "Exiting to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 6);
    }
};

int main()
{
    tree courseTree;
    UndoStack undoStack;

    Menu menu;
    int mainChoice;
    do
    {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Teacher Menu\n";
        cout << "2. Student Menu\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> mainChoice;

        switch (mainChoice)
        {
        case 1:
            menu.teacherMenu(courseTree);
            break;
        case 2:
            menu.studentMenu(courseTree, undoStack);
            break;
        case 3:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (mainChoice != 3);

    return 0;
}
