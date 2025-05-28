//////////////////////////////////////////
// Created by: Nikolai Akulov, 316868132//
//             Maor Elbaz,     208418293//
//             Shay Shitrit,   207895798//
//////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"
#include "items.h"
#include "customers.h"
#include "employees.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

// Global pointers declerations
ListEmployee* employeeList;
ListItem* itemList;
ListCustomer* customerList;
ListPurchase* purchaseItemsList;
FILE* fplog;


void printLoadingBar() {
    for (int i = 0; i < 100; i++) {
        printf("|");
        Sleep(25);
    }
    printf("\n");
}

void printShopLogo() {
    FILE* fp = fopen("logo.txt", "r");
    while (!feof(fp))
        printf("%c", fgetc(fp));
    printf("\n");
    fclose(fp);
}

void shutdownSystem() {
    writeEmployeesDataToFile(employeeList);
    writeItemsDataToFile(itemList);
    writeCustomersDataToFile(customerList);
}


int main() {

    printf("Hello World, Booting system \n");
    printLoadingBar();
    printShopLogo();

    fplog = fopen("logs.txt", "a");
    employeeList = createList();
    bootEmployeesToList(employeeList);
    itemList = createListItem();
    bootItemsToList(itemList);
    customerList = createListCustomer();
    bootCustomersToList(customerList);

    if (!checkIfEmployeeFileExists()) {
        createDefaultAdmin();
    }

    Employee currentEmployee = login();
    if (currentEmployee.level == 0) {
        printf("Login failed. Try again later..\n");
        return 0;
    }

    showMenu(currentEmployee.level);
    
    
    printf("Exiting system, saving all data into files\n");
    shutdownSystem();
    fprintf(fplog, "%s%s exit from application\n", currentDateTime(), currentEmployee.userName);
    fclose(fplog);
    printLoadingBar();
    printf("Goodbye! We are hopping to get 100 :)\n");
    printShopLogo();
    return 0;
}
