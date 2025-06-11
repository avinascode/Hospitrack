#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_BEDS = 10;

class Patient {
public:
    string name;
    int age;
    string disease;
    int bedNumber;

    Patient(string name, int age, string disease, int bedNumber) {
        this->name = name;
        this->age = age;
        this->disease = disease;
        this->bedNumber = bedNumber;
    }

    string toString() const {
        return name + "," + to_string(age) + "," + disease + "," + to_string(bedNumber);
    }

    static Patient fromString(const string& line) {
        string name, disease, temp;
        int age, bed;
        size_t pos1 = line.find(',');
        name = line.substr(0, pos1);

        size_t pos2 = line.find(',', pos1 + 1);
        age = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));

        size_t pos3 = line.find(',', pos2 + 1);
        disease = line.substr(pos2 + 1, pos3 - pos2 - 1);

        bed = stoi(line.substr(pos3 + 1));

        return Patient(name, age, disease, bed);
    }
};

class Hospital {
private:
    vector<Patient> patients;
    bool beds[MAX_BEDS] = {false};

public:
    void loadFromFile() {
        ifstream file("patients.txt");
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Patient p = Patient::fromString(line);
                patients.push_back(p);
                beds[p.bedNumber] = true;
            }
        }
        file.close();
    }

    void saveToFile() {
        ofstream file("patients.txt");
        for (auto& p : patients) {
            file << p.toString() << endl;
        }
        file.close();
    }

    void addPatient() {
        if (getAvailableBed() == -1) {
            cout << "No available beds right now.\n";
            return;
        }

        string name, disease;
        int age;

        cout << "Enter patient's name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter age: ";
        cin >> age;
        cout << "Enter disease: ";
        cin.ignore();
        getline(cin, disease);

        int bed = getAvailableBed();
        beds[bed] = true;

        Patient p(name, age, disease, bed);
        patients.push_back(p);
        cout << "Patient admitted to bed " << bed << ".\n";

        saveToFile();
    }

    int getAvailableBed() {
        for (int i = 0; i < MAX_BEDS; ++i) {
            if (!beds[i]) return i;
        }
        return -1;
    }

    void viewPatients() {
        if (patients.empty()) {
            cout << "No patients admitted.\n";
            return;
        }

        cout << "\n--- Admitted Patients ---\n";
        cout << left << setw(15) << "Name" << setw(8) << "Age" << setw(15)
             << "Disease" << setw(10) << "Bed No" << endl;

        for (auto& p : patients) {
            cout << left << setw(15) << p.name << setw(8) << p.age
                 << setw(15) << p.disease << setw(10) << p.bedNumber << endl;
        }
    }

    void dischargePatient() {
        string name;
        cout << "Enter patient's name to discharge: ";
        cin.ignore();
        getline(cin, name);

        bool found = false;
        for (int i = 0; i < patients.size(); ++i) {
            if (patients[i].name == name) {
                beds[patients[i].bedNumber] = false;
                patients.erase(patients.begin() + i);
                found = true;
                cout << "Patient discharged.\n";
                break;
            }
        }

        if (!found) {
            cout << "Patient not found.\n";
        }

        saveToFile();
    }

    void showBedStatus() {
        cout << "\n--- Bed Status ---\n";
        for (int i = 0; i < MAX_BEDS; ++i) {
            cout << "Bed " << i << ": " << (beds[i] ? "Occupied" : "Available") << endl;
        }
    }
};

int main() {
    Hospital hospital;
    hospital.loadFromFile();

    int choice;
    do {
        cout << "\n--- Hospital Bed Allocation ---\n";
        cout << "1. Admit Patient\n";
        cout << "2. View Patients\n";
        cout << "3. Discharge Patient\n";
        cout << "4. Show Bed Status\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                hospital.addPatient();
                break;
            case 2:
                hospital.viewPatients();
                break;
            case 3:
                hospital.dischargePatient();
                break;
            case 4:
                hospital.showBedStatus();
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);

    return 0;
}
