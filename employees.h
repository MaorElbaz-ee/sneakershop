//////////////////////////////////////////
// Created by: Nikolai Akulov, 316868132//
//             Maor Elbaz,     208418293//
//             Shay Shitrit,   207895798//
//////////////////////////////////////////
#pragma once

typedef struct employee {
	char userName[20];
	char firstName[10];
	char password[10];
	unsigned int level; // 1 - admin, 2 - employee, 3 - trainee
}Employee;

typedef struct nodeemployee {
    Employee employeeData;
    struct node* next;
}NodeEmployee;

typedef struct listemployee {
    NodeEmployee* head;
}ListEmployee;

void writeEmployeesDataToFile(ListEmployee* emplst);
void bootEmployeesToList(ListEmployee* emplst);
ListEmployee* createList();
NodeEmployee* createNodeEmployee(Employee emp);
void addValueToStart(ListEmployee* emplst, Employee emp);
void displayListEmployee(ListEmployee* emplst);
NodeEmployee* findNodeByUsername(ListEmployee* emplst, char* userName);
void createNewUser(ListEmployee* emplst);
int checkIfEmployeeFileExists();
void createDefaultAdmin();
Employee login();
