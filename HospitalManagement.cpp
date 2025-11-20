#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

struct Patient {
    string name;
    int severity;
    string condition;
    time_t arrivalTime;
    int patientID;
    
    Patient(string n, int s, string c, int id) 
        : name(n), severity(s), condition(c), patientID(id) {
        arrivalTime = time(nullptr);
    }
};

struct ComparePriority {
    bool operator()(const Patient& p1, const Patient& p2) {
        if (p1.severity == p2.severity) {
            return p1.arrivalTime > p2.arrivalTime;
        }
        return p1.severity < p2.severity;
    }
};

class ERManager {
private:
    priority_queue<Patient, vector<Patient>, ComparePriority> waitingQueue;
    vector<Patient> treatedPatients;
    int nextPatientID;
    
    string getSeverityLabel(int severity) {
        switch(severity) {
            case 5: return "CRITICAL";
            case 4: return "SEVERE";
            case 3: return "MODERATE";
            case 2: return "MINOR";
            case 1: return "MINIMAL";
            default: return "UNKNOWN";
        }
    }
    
    string getTimeString(time_t t) {
        char buffer[80];
        struct tm* timeinfo = localtime(&t);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
        return string(buffer);
    }
    
public:
    ERManager() : nextPatientID(1001) {}
    
    void addPatient(string name, int severity, string condition) {
        if (severity < 1 || severity > 5) {
            cout << "\nInvalid severity! Must be 1-5.\n";
            return;
        }
        
        Patient p(name, severity, condition, nextPatientID++);
        waitingQueue.push(p);
        
        cout << "\nPatient Added!\n";
        cout << "   ID: " << p.patientID << endl;
        cout << "   Name: " << p.name << endl;
        cout << "   Severity: " << getSeverityLabel(p.severity) << " (" << p.severity << ")" << endl;
        cout << "   Condition: " << p.condition << endl;
        cout << "   Time: " << getTimeString(p.arrivalTime) << endl;
    }
    
    void treatNextPatient() {
        if (waitingQueue.empty()) {
            cout << "\nNo patients waiting. ER is clear!\n";
            return;
        }
        
        Patient p = waitingQueue.top();
        waitingQueue.pop();
        treatedPatients.push_back(p);
        
        long waitTime = time(nullptr) - p.arrivalTime;
        
        cout << "\nNOW TREATING:\n";
        cout << "   ID: " << p.patientID << endl;
        cout << "   Name: " << p.name << endl;
        cout << "   Severity: " << getSeverityLabel(p.severity) << " (" << p.severity << ")" << endl;
        cout << "   Condition: " << p.condition << endl;
        
        if (waitTime < 25) {
            cout << "   Waited: " << waitTime << " days\n";
        } else {
            cout << "   Waited: " << waitTime << " hours\n";
        }
    }
    
    void displayQueue() {
        if (waitingQueue.empty()) {
            cout << "\nWaiting Queue: Empty\n";
            return;
        }
        
        cout << "\nWAITING QUEUE (" << waitingQueue.size() << " patients)\n";
        cout << string(80, '=') << endl;
        cout << left << setw(6) << "ID" 
             << setw(20) << "Name" 
             << setw(12) << "Severity"
             << setw(25) << "Condition" 
             << "Time" << endl;
        cout << string(80, '-') << endl;
        
        priority_queue<Patient, vector<Patient>, ComparePriority> temp = waitingQueue;
        while (!temp.empty()) {
            Patient p = temp.top();
            temp.pop();
            
            cout << left << setw(6) << p.patientID
                 << setw(20) << p.name
                 << setw(12) << getSeverityLabel(p.severity)
                 << setw(25) << p.condition
                 << getTimeString(p.arrivalTime) << endl;
        }
    }
    
    void displayTreated() {
        if (treatedPatients.empty()) {
            cout << "\nTreated Patients: None yet\n";
            return;
        }
        
        cout << "\nTREATED PATIENTS (" << treatedPatients.size() << " total)\n";
        cout << string(80, '=') << endl;
        cout << left << setw(6) << "ID" 
             << setw(20) << "Name" 
             << setw(12) << "Severity"
             << setw(25) << "Condition" << endl;
        cout << string(80, '-') << endl;
        
        for (const auto& p : treatedPatients) {
            cout << left << setw(6) << p.patientID
                 << setw(20) << p.name
                 << setw(12) << getSeverityLabel(p.severity)
                 << setw(25) << p.condition << endl;
        }
    }
    
    void displayDashboard() {
        cout << "\n";
        cout << "==============================================================================\n";
        cout << "              EMERGENCY ROOM PRIORITY MANAGEMENT SYSTEM                     \n";
        cout << "==============================================================================\n";
        
        cout << "\nSTATISTICS:\n";
        cout << "   Patients Waiting: " << waitingQueue.size() << endl;
        cout << "   Patients Treated: " << treatedPatients.size() << endl;
        cout << "   Total Processed: " << (waitingQueue.size() + treatedPatients.size()) << endl;
        
        if (!waitingQueue.empty()) {
            Patient next = waitingQueue.top();
            cout << "\nNEXT PRIORITY CASE:\n";
            cout << "   " << next.name << " - " << getSeverityLabel(next.severity) 
                 << " (" << next.condition << ")\n";
        }
    }
};

void displayMenu() {
    cout << "\n==============================================================================\n";
    cout << "                              MAIN MENU                                     \n";
    cout << "==============================================================================\n";
    cout << "  1. Add New Patient                                                        \n";
    cout << "  2. Treat Next Patient (Priority)                                          \n";
    cout << "  3. View Waiting Queue                                                     \n";
    cout << "  4. View Treated Patients                                                  \n";
    cout << "  5. View Dashboard                                                         \n";
    cout << "  6. Exit                                                                   \n";
    cout << "==============================================================================\n";
    cout << "Enter choice: ";
}

int main() {
    ERManager er;
    int choice;
    
    cout << "\nWelcome to the Emergency Room Priority Manager!\n";
    cout << "Patients are automatically sorted by severity level (5=Critical, 1=Minimal)\n";
    
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline
        
        if (choice == 1) {
            string name, condition;
            int severity;
            
            cout << "\nEnter patient name: ";
            getline(cin, name);
            
            cout << "Enter severity (1-5, where 5=Critical): ";
            cin >> severity;
            cin.ignore();
            
            cout << "Enter condition/symptoms: ";
            getline(cin, condition);
            
            er.addPatient(name, severity, condition);
            
        } else if (choice == 2) {
            er.treatNextPatient();
            
        } else if (choice == 3) {
            er.displayQueue();
            
        } else if (choice == 4) {
            er.displayTreated();
            
        } else if (choice == 5) {
            er.displayDashboard();
            
        } else if (choice == 6) {
            cout << "\nThank you for using ER Priority Manager. Stay safe!\n\n";
            break;
            
        } else {
            cout << "\nInvalid choice! Please try again.\n";
        }
    }
    
    return 0;
}