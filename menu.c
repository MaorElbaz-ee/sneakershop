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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

ListEmployee* employeeList;
ListItem* itemList;
ListCustomer* customerList;
ListPurchase* purchaseItemsList;


void showStatistics(ListItem* itmlst, ListCustomer* csmlst) {
	char popularModel[20];
	strcpy(popularModel, showMostPopularModel(itmlst));
	printf("The most popular item is: %s\n", popularModel);
	printf("The shop has %d costumers\n", checkHowManyCustomers(csmlst));
	printf("The store sold %d items\n", calculateTotalSales(itmlst));
	printf("Total income is %7.2f\n", totalIncome(csmlst));
}

void showMenu(unsigned int userLevel) {
	int chosenOption;
	printf("=====================================\n");
	printf("Wellcome to SNEAKERS SHOP\n");
	printf("You are logged as: ");
	switch (userLevel) {
	case 1: {
		printf("Administrator\n");
		break;
	}
	case 2: {
		printf("Employee\n");
		break;
	}
	case 3: {
		printf("Trainee\n");
		break;
	}
	}
	do {
		printf("=====================================\n");

		if (userLevel <= 3) {
			printf("   0.  Exit application\n");
			printf("   1.  Search for item\n");
			printf("   2.  Add new item\n");
			printf("   3.  Add new customer\n");
			printf("   4.  View items list\n");
			printf("   5.  View customer list\n");
			printf("   6.  View purchased items by customer\n");
		}
		if (userLevel <= 2) {
			printf("   7.  Remove item\n");
			printf("   8.  Remove customer\n");
			printf("   9.  Update item\n");
			printf("   10. Sell item\n");
			printf("   11. Return item\n");
			printf("   12. Add inventory to all items\n");

		}
		if (userLevel == 1) {
			printf("   13. Add new employee\n");
			printf("   14. Show shop statistics\n");
			printf("   15. Show employees list\n");
		}
		printf("=====================================\n");
		printf("Choose your option: ");
		scanf("%d", &chosenOption);
		if (chosenOption > 15)
			printf("Incorrect option! \n");
		if (userLevel == 3 && chosenOption > 6) {
			printf("Practicioner can choose only 0  1  2  3  4  5  6\n");
			chosenOption = 16;
		}
		if (userLevel == 2 && chosenOption > 12){
			printf("Employee can choose only 0  1  2  3  4  5  6  7  8  9  10  11  12\n");
			chosenOption = 16;
		}

		switch (chosenOption) {
		case 1: {
			searchItemByCategory(itemList);
			break;
		}
		case 2: {
			createNewItem(itemList);
			break;
		}
		case 3: {
			createNewCustomer(customerList);
			break;
		}
		case 4: {
			displayListItems(itemList);
			break;
		}
		case 5: {
			displayListCustomer(customerList);
			break;
		}
		case 6: {
			displayAllPurchases(customerList);
			break;
		}
		case 7: {
			deleteItemFromList(itemList);
			break;
		}
		case 8: {
			deleteCustomerFromList(customerList);
			break;
		}
		case 9: {
			updateItemFromListMenu(itemList);
			break;
		}
		case 10: {
			purchaseItem(customerList, itemList);
			break;
		}
		case 11: {
			returnItem(customerList, itemList);
			break;
		}
		case 12: {
			addInventoryToAllStock(itemList);
			break;
		}
		case 13: {
			createNewUser(employeeList);
			break;
		}
		case 14: {
			showStatistics(itemList, customerList);
			break;
		}
		case 15: {
			displayListEmployee(employeeList);
			break;
		}
		case 16: { // In case the user chooses an option that does not correspond to his level 
			break;
		}
		}
	} while (chosenOption > 0);

}