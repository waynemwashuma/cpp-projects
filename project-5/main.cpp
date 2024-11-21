// #include <iostream>
// #include <string>
#include "PatientManagement.h"

int main()
{
    // Load initial patients from patient.csv
    loadInitialPatients("patient.csv");

    while (true)
    {
        string clinic;
        int choice = mainMenu();
        clearScreen();

        if (choice == 1)
            clinic = "HC";
        else if (choice == 2)
            clinic = "PC";
        else if (choice == 3)
            clinic = "PSC";
        else
            break;

        while (true)
        {
            clearScreen();

            string clinic_name = clinic == "HC" ? "Heart" : clinic == "PC" ? "Pulmonary"
                                                                           : "Plastic Surgery";
            int department_choice = departmentMenu(clinic_name);

            if (department_choice == 1)
                addPatient(clinic,false);
            else if (department_choice == 2)
                addPatient(clinic,true);
            else if (department_choice == 3)
                takeOutPatient(clinic);
            else if (department_choice == 4)
                cancelPatient(clinic);
            else if (department_choice == 5)
                listAllPatients(clinic);
            else if (department_choice == 6)
                break;
        }
    }

    cout << "Exiting system. Goodbye!" << endl;
    return 0;
}
