//////////////////////////////////////////
// Created by: Nikolai Akulov, 316868132//
//             Maor Elbaz,     208418293//
//             Shay Shitrit,   207895798//
//////////////////////////////////////////
#pragma once
#include "items.h"

typedef struct purchase {
	char purchaseDate[11];
	int itemID;
}Purchase;

typedef struct nodepurchase {
	Purchase purchaseData;
	struct nodepurchase* next;
}NodePurchase;

typedef struct listpurchase {
	NodePurchase* head;
}ListPurchase;

typedef struct customer {
	char name[20];
	char dateJoin[11]; // Date of joining
	float moneySpent;
	ListPurchase* purchaseList;
}Customer;

typedef struct nodecustomer {
	Customer customerData;
	struct nodecustomer* next;
}NodeCustomer;

typedef struct listCustomer {
	NodeCustomer* head;
}ListCustomer;

char* currentDateTime();
void writeCustomersDataToFile(ListCustomer* csmlst);
void convertDateStringToInteger(char* date, int* day, int* month, int* year);
float totalIncome(ListCustomer* csmlst);
void displayAllPurchases(ListCustomer* csmlst);
void displayListPurchsedByCustomer(ListPurchase* purlst);
void addPurchaseToStartPurchaseList(ListPurchase* listpur, NodePurchase* nodepur);
void purchaseItem(ListCustomer* csmlst, ListItem* itmlst);
int isLeapYear(int year);
int isValidDate(char* dateToCheck);
int calculateDays(int day, int month, int year);
int calculateDistanceBetweenDates(char* older, char* newer);
void deleteFirstPurchaseNode(ListPurchase* listpur);
void deleteItemFromPurchaseList(ListPurchase* listpur, int returnID);
NodePurchase* searchItemByIDPurchase(ListPurchase* listpur, int id);
void returnItem(ListCustomer* csmlist, ListItem* itmlst);
NodePurchase* createNodePurchase(int id, char* date);
ListPurchase* createListPurchase();

char* currentDate();
void bootCustomersToList(ListCustomer* csmlst);
int checkHowManyCustomers(ListCustomer* csmlst);
NodeCustomer* findCustomerByName(ListCustomer* csm, char* name);
void deleteFirstCustomerNode(ListCustomer* csm);
void deleteCustomerFromList(ListCustomer* csm);
//NodeCustomer* findNodeWithName(ListCustomer* lst, char* name);
void displayListCustomer(ListCustomer* lst);
void addValueToStartCustomer(ListCustomer* csmlst, Customer csm);
void addValueToCustomerList(ListCustomer* lst, Customer csm);
void createNewCustomer(ListCustomer* csmList);
NodeCustomer* createNodeCustomer(Customer csm);
ListCustomer* createListCustomer();
