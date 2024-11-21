#ifndef PATIENT_MANAGEMENT_H
#define PATIENT_MANAGEMENT_H

#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Struct to represent a patient
struct Patient {
    string firstName;
    string lastName;
    string ssn;
    Patient(string fn, string ln, string ssnNum);
};

// Constants
const size_t MAX_CAPACITY = 10;

// Function declarations
void logTransaction(const string& message);
void clearScreen();
int mainMenu();
int departmentMenu(const string& clinicName);
void loadInitialPatients(const string& fileName);
void addPatient(const string& clinic,bool is_critical);
void takeOutPatient(const string& clinic);
void cancelPatient(const string& clinic);
void listAllPatients(const string& clinic);

// Globals
extern map<string, queue<Patient>> criticalQueues;
extern map<string, queue<Patient>> regularQueues;

#endif // PATIENT_MANAGEMENT_H
