//////////////////////////////////////////
// Created by: Nikolai Akulov, 316868132//
//             Maor Elbaz,     208418293//
//             Shay Shitrit,   207895798//
//////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "employees.h"
#include "customers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

ListEmployee* employeeList;
FILE* fplog;

void writeEmployeesDataToFile(ListEmployee* emplst) {
	FILE* fp = fopen("employees.txt", "w");
	NodeEmployee* currentNode = emplst->head;
	while (currentNode != NULL) {
		fprintf(fp, "%-20s    %-10s    %-10s    %d\n", currentNode->employeeData.userName, currentNode->employeeData.firstName, currentNode->employeeData.password, currentNode->employeeData.level);
		currentNode = currentNode->next;
	}
	fclose(fp);
	fprintf(fplog, "%sEmployees loaded successfully into database\n", currentDateTime());
}

void bootEmployeesToList(ListEmployee* emplst) {
	FILE* fp = NULL;
	if(fp = fopen("employees.txt", "r")) { // If file exist should return 1
		Employee emp = {"","","",0};
		while (!feof(fp)) {
			fscanf(fp, "%s %s %s %d ", emp.userName, emp.firstName, emp.password, &emp.level);
			addValueToStart(emplst, emp);
		}
		fclose(fp);
		printf("--->Employess added to database sucsesfully\n");
		fprintf(fplog, "%sEmployees loaded successfully from database\n", currentDateTime());
	}
}

/*
* Function displays the list of employees
*/
void displayListEmployee(ListEmployee* emplst) {
	NodeEmployee* currentNode = emplst->head;
	while (currentNode != NULL) {
		printf("%s ->  ", currentNode->employeeData.userName);
		currentNode = currentNode->next;
	}
	fprintf(fplog, "%sEmployees list printed to screen\n", currentDateTime());
	printf("NULL\n");
}

/*
* Searching employee user node in employees linked list
*/
NodeEmployee* findNodeByUsername(ListEmployee* emplst, char* userName) {
	NodeEmployee* currentNode = emplst->head;
	while (currentNode != NULL) {
		if (strcmp(currentNode->employeeData.userName, userName) == 0)
			return currentNode;
		currentNode = currentNode->next;
	}
	return currentNode;
}

/*
* Function check if there is a match between the input and employee from employees linked list 
*/
Employee login() {
	Employee loginUser;
	Employee defaultUser = { "default", "default", "1", 0 };
	int wrongPasswordCnt = 0;
	printf("Hello, Please login\n");
	printf("Enter username: ");
	scanf("%s", loginUser.userName);
	NodeEmployee* loginUserData = findNodeByUsername(employeeList, loginUser.userName);
	if (loginUserData == NULL) {
		printf("\nWrong Username!\n");
		fprintf(fplog, "%sIncorrect username inserted\n", currentDateTime());
		return defaultUser;
	}
	do {
		printf("Enter password: ");
		scanf("%s", loginUser.password);
		wrongPasswordCnt++;
	} while (strcmp(loginUser.password, loginUserData->employeeData.password) != 0 && wrongPasswordCnt < 3);
	if (strcmp(loginUser.password, loginUserData->employeeData.password)) {
		printf("\nIncorrect password\n");
		fprintf(fplog, "%s%s inserted wrong password\n", currentDateTime(), loginUser.userName);
		return defaultUser;
	}
	//if (wrongPasswordCnt == 3) {
	//	printf("\nToo many attempts...\n");
	//	fprintf(fplog, "%s%s inserted wrong password\n", currentDateTime(), loginUser.userName);
	//	return defaultUser;
	//}

	strcpy(loginUser.firstName, loginUserData->employeeData.firstName);
	loginUser.level = loginUserData->employeeData.level;
	printf("User: %s logged in successfully\n", loginUser.userName);
	fprintf(fplog, "%s%s logged into the system\n", currentDateTime(), loginUser.userName);
	return loginUser;
}

/*
* Function check if employee file exist
*/
int checkIfEmployeeFileExists() {
	FILE* fp = NULL;
	if (!(fp = fopen("employees.txt", "r"))) { // If trying opening not existing file to reading - fopen will return NULL
		printf("The employeess.txt file is not exist!\n--->Creating defualt administartor\n");
		fprintf(fplog, "%semployees.txt dosn't exist\n", currentDateTime());
		return 0;
	}
	return 1; // The employee.txt file exists
}

/*
* Function create node of employee struct
*/
NodeEmployee* createNodeEmployee(Employee emp) {
	NodeEmployee* newNode = (NodeEmployee*)malloc(sizeof(NodeEmployee)); // Allocating memory for the node
	if (!newNode) {
		printf("Failed to create the node\n");
		return NULL;
	}
	newNode->employeeData = emp;
	newNode->next = NULL;
	return newNode;
}
/*
* Function inserting employee into the employees linked list.
* No demend to be sorted.
*/
void addValueToStart(ListEmployee* emplst, Employee emp) {
	NodeEmployee* newNode = createNodeEmployee(emp);
	if (!newNode) return;

	if (emplst->head == NULL) // If the list is empty
		emplst->head = newNode;
	else {
		newNode->next = emplst->head;
		emplst->head = newNode;
	}
}

/*
* Function create new employees and insert it into the employees linked list
*/
void createNewUser(ListEmployee* emplst) {
	printf("Create a new employee:\n");
	Employee newEmployee;
	char tempLevel = 0;
	do {
		printf("Enter username: (max 20 characters) ");
		scanf("%s", newEmployee.userName);
	} while (strlen(newEmployee.userName) > 20 || strlen(newEmployee.userName) < 1);
	do {
		printf("Enter first name: (max 20 characters) ");
		scanf("%s", newEmployee.firstName);
	} while (strlen(newEmployee.firstName) > 20 || strlen(newEmployee.firstName) < 1);
	do {
		printf("Enter password: (max 10 characters) ");
		scanf("%s", newEmployee.password);
	} while (strlen(newEmployee.userName) > 10 || strlen(newEmployee.password) < 1);
	do {
		printf("Enter user level: (1 - Administartor, 2 - Employee, 3 - Trainee) \n");
		scanf(" %c", &tempLevel);
		newEmployee.level = tempLevel - '0';
	} while (newEmployee.level > 3 || newEmployee.level < 1);

	addValueToStart(employeeList, newEmployee);
	printf("\nUser: %s level: %d added successfully to database \n", newEmployee.userName, newEmployee.level);
	fprintf(fplog, "%sUser %s created successfully\n", currentDateTime(), newEmployee.userName);
}

/*
* Function create user incase the employee.txt file dosnt exists.
*/
void createDefaultAdmin() {
	Employee defaultAdmin = { "admin", "manager", "12345678", 1 };
	addValueToStart(employeeList, defaultAdmin);
	fprintf(fplog, "%sDefault admin created\n", currentDateTime());
}

/*
* Create linked-list to store all employess data
*/
ListEmployee* createList() {
	ListEmployee* newList = (ListEmployee*)malloc(sizeof(ListEmployee));
	if (!newList) {
		printf("Failed to create employees linked list!\n");
		return NULL;
	}
	newList->head = NULL;
	//printf("Empty linked has been created\n");
	return newList;
}

