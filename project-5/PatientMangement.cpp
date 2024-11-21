#include "PatientManagement.h"

map<string, queue<Patient>> criticalQueues = {{"HC", {}}, {"PC", {}}, {"PSC", {}}};
map<string, queue<Patient>> regularQueues = {{"HC", {}}, {"PC", {}}, {"PSC", {}}};

Patient::Patient(string fn, string ln, string ssnNum) : firstName(fn), lastName(ln), ssn(ssnNum) {}

void logTransaction(const string &message)
{
    ofstream logFile("transaction.log", ios::app);
    if (logFile.is_open())
    {
        logFile << message << endl;
        logFile.close();
    }
    else
    {
        cerr << "Error: Unable to open transaction log file." << endl;
    }
}

void continueToMenu()
{
    cout << endl;
#ifdef _WIN32
    system("pause");
#else
    system("read -p \"Press any key to continue...\" -n 1 -s");
#endif
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int mainMenu()
{
    int choice;
    cout << "\nKC RESEARCH HOSPITAL\n";
    cout << "1: Heart Clinic\n";
    cout << "2: Pulmonary Clinic\n";
    cout << "3: Plastic Surgery\n";
    cout << "4: Quit\n";
    cout << "Please enter the clinic of interest: ";
    cin >> choice;
    return choice;
}

int departmentMenu(const string &clinicName)
{
    int choice;
    cout << "\nWelcome to the " << clinicName << " Clinic\n";
    cout << "1: Add Patient\n";
    cout << "2: Add Critical Patient\n";
    cout << "3: Take Out Patient for Operation\n";
    cout << "4: Cancel Patient\n";
    cout << "5: List All Current Patients\n";
    cout << "6: Exit Current Department\n";
    cout << "Enter your choice: ";
    cin >> choice;

    return choice;
}

void loadInitialPatients(const string &fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string clinic, firstName, lastName, ssn;

        getline(ss, clinic, ',');
        getline(ss, firstName, ',');
        getline(ss, lastName, ',');
        getline(ss, ssn, ',');

        if (criticalQueues.find(clinic) == criticalQueues.end())
        {
            logTransaction("Error: Invalid clinic name in file for " + firstName + " " + lastName);
            continue;
        }

        if (!all_of(ssn.begin(), ssn.end(), ::isdigit))
        {
            logTransaction("Error: Invalid SSN in file for " + firstName + " " + lastName);
            continue;
        }

        size_t totalPatients = criticalQueues[clinic].size() + regularQueues[clinic].size();
        if (totalPatients >= MAX_CAPACITY)
        {
            logTransaction("Error: Clinic " + clinic + " is full for " + firstName + " " + lastName);
            continue;
        }

        regularQueues[clinic].push(Patient(firstName, lastName, ssn));
        logTransaction("Success: Added patient " + firstName + " " + lastName + " (" + ssn + ") to " + clinic);
    }

    file.close();
}

void addPatient(const string &clinic, bool is_critical)
{
    clearScreen();
    string firstName, lastName, ssn;

    cout << "Enter Patient's First Name: ";
    cin >> firstName;
    cout << "Enter Patient's Last Name: ";
    cin >> lastName;
    cout << "Enter Patient's SSN: ";
    cin.ignore();
    getline(cin, ssn);

    if (ssn.empty())
    {
        cout << "Operation canceled. SSN cannot be empty." << endl;
        continueToMenu();
        return;
    }

    if (!all_of(ssn.begin(), ssn.end(), ::isdigit))
    {
        cout << "Error: SSN must be numeric." << endl;
        logTransaction("Error: Invalid SSN for " + firstName + " " + lastName);
        continueToMenu();
        return;
    }

    size_t totalPatients = criticalQueues[clinic].size() + regularQueues[clinic].size();
    if (totalPatients >= MAX_CAPACITY)
    {
        cout << "Error: Clinic is full. Cannot add more patients." << endl;
        logTransaction("Error: Clinic " + clinic + " is full for " + firstName + " " + lastName);
        continueToMenu();
        return;
    }

    if (!is_critical)
    {
        regularQueues[clinic].push(Patient(firstName, lastName, ssn));
        cout << clinic << " Patient (Regular): " << firstName << " " << lastName << " was added to the waiting room." << endl;
        logTransaction("Success: Added regular patient " + firstName + " " + lastName + " (" + ssn + ") to " + clinic);
    }
    else
    {
        criticalQueues[clinic].push(Patient(firstName, lastName, ssn));
        cout << clinic << " Patient (Critical): " << firstName << " " << lastName << " was added to the waiting room." << endl;
        logTransaction("Success: Added critically ill patient " + firstName + " " + lastName + " (" + ssn + ") to " + clinic);
    }
    continueToMenu();
}

void listAllPatients(const string &clinic)
{
    clearScreen();
    cout << "\nPatient List for " << clinic << " Clinic:\n";

    queue<Patient> tempRegular = regularQueues[clinic];
    queue<Patient> tempCritical = criticalQueues[clinic];

    if (tempCritical.empty() && tempRegular.empty())
    {
        cout << "No patients in the queue.\n";
    }
    else
    {
        cout << "\nCritical Patients (C):\n";
        while (!tempCritical.empty())
        {
            Patient patient = tempCritical.front();
            tempCritical.pop();
            cout << left << setw(10) << patient.firstName << setw(10) << patient.lastName << setw(5) << patient.ssn << "C\n";
        }

        // List regular patients
        cout << "\nRegular Patients (R):\n";
        while (!tempRegular.empty())
        {
            Patient patient = tempRegular.front();
            tempRegular.pop();
            cout << left << setw(10) << patient.firstName << setw(10) << patient.lastName << setw(5) << patient.ssn << "R\n";
        }
    }

    continueToMenu();
}

void takeOutPatient(const string &clinic)
{
    clearScreen();
    if (!criticalQueues[clinic].empty())
    {
        // Remove the first patient from the critical queue
        Patient patient = criticalQueues[clinic].front();
        criticalQueues[clinic].pop();
        cout << clinic << " Patient (Critical): " << patient.firstName << " " << patient.lastName << " was taken to the operating room." << endl;
        logTransaction("Success: Critical patient " + patient.firstName + " " + patient.lastName + " taken to the operating room from " + clinic);
        continueToMenu();
        return;
    }

    if (!regularQueues[clinic].empty())
    {
        // Remove the first patient from the regular queue
        Patient patient = regularQueues[clinic].front();
        regularQueues[clinic].pop();
        cout << clinic << " Patient: " << patient.firstName << " " << patient.lastName << " was taken to the operating room." << endl;
        logTransaction("Success: Regular patient " + patient.firstName + " " + patient.lastName + " taken to the operating room from " + clinic);
        continueToMenu();
        return;
    }

    // If no patients in either queue
    cout << "No more patients in the queue for " << clinic << "." << endl;
    continueToMenu();
}

void cancelPatient(const string &clinic)
{
    clearScreen();
    string ssn;
    cout << "Enter Patient SSN: ";
    cin >> ssn;

    bool found = false;

    // Search in the critical queue
    queue<Patient> tempCritical;
    while (!criticalQueues[clinic].empty())
    {
        Patient patient = criticalQueues[clinic].front();
        criticalQueues[clinic].pop();
        if (patient.ssn == ssn)
        {
            found = true;
            cout << clinic << " Patient (Critical): " << patient.firstName << " " << patient.lastName << " was removed from the waiting list." << endl;
            logTransaction("Success: " + clinic + " Patient (Critical): " + patient.firstName + " " + patient.lastName + " (" + ssn + ") was removed from the waiting list.");
        }
        else
        {
            tempCritical.push(patient);
        }
    }
    criticalQueues[clinic] = tempCritical;

    // Search in the regular queue if not found in critical
    if (!found)
    {
        queue<Patient> tempRegular;
        while (!regularQueues[clinic].empty())
        {
            Patient patient = regularQueues[clinic].front();
            regularQueues[clinic].pop();
            if (patient.ssn == ssn)
            {
                found = true;
                cout << clinic << " Patient: " << patient.firstName << " " << patient.lastName << " was removed from the waiting list." << endl;
                logTransaction("Success: " + clinic + " Patient: " + patient.firstName + " " + patient.lastName + " (" + ssn + ") was removed from the waiting list.");
            }
            else
            {
                tempRegular.push(patient);
            }
        }
        regularQueues[clinic] = tempRegular;
    }

    if (!found)
    {
        cout << "ERROR " << clinic << ": Unable to find patient with SSN " << ssn << "." << endl;
        logTransaction("ERROR " + clinic + ": Unable to find patient with SSN " + ssn + ".");
    }
    continueToMenu();
}
