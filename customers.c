//////////////////////////////////////////
// Created by: Nikolai Akulov, 316868132//
//             Maor Elbaz,     208418293//
//             Shay Shitrit,   207895798//
//////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "customers.h"
#include "items.h"
#include "employees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

ListCustomer* customerList;
ListPurchase* purchaseItemsList;
FILE* fplog;


/*
* currentDateTime create string of current date and time [DD-MM-YYYY HH:MM:SS]
* This function is to be used to implement date and time into log file
*/
char* currentDateTime() {
	char* current[20];
	time_t currentTime;					// variable that will hold ephoch time (seconds from 01-01-1970 up to now)
	struct tm* timeInfo;			    // Struct to hold year, month, day, hour, minute, second
	char formattedTime[80];
	time(&currentTime);                 // Stores the ephoch time to currentTime
	timeInfo = localtime(&currentTime); // Converts ephoch time to local time
	strftime(formattedTime, sizeof(formattedTime), "[%d-%m-%Y %H:%M:%S] ", timeInfo); // Converting the timeInfo struct to readable string
	strcpy(current, formattedTime);
	return current;
}

/*
* writeCustomersDataToFile write customers linked list into txt file
* The data is using this tamplate: [joinDate]	[name]	 [moneySpent]	[purchasedItem1]	[purchasedItem2]	... [purchasedItemN]
*  -> Insert customer data from node to customers.txt 
*  -> Go the customer's purchsed items linked-list and store in the same line all purchased items. 
*/
void writeCustomersDataToFile(ListCustomer* csmlst) {
	FILE* fp = fopen("customers.txt", "w"); 
	NodeCustomer* currentNode = csmlst->head;										      // Pointer to the first customer node in the linked-list
	while (currentNode != NULL) {													      // Loop to cover all the customers
		fprintf(fp, "%-10s    %-20s    %7.2f", currentNode->customerData.dateJoin, currentNode->customerData.name, currentNode->customerData.moneySpent);  // Insert into txt file the customer data
		NodePurchase* currentPurchase = currentNode->customerData.purchaseList->head;     // Pointer to current customer purchased items node
		while (currentPurchase != NULL) {											      // Loop to cover all the purchased itmes
			fprintf(fp, "    %-10s    %d", currentPurchase->purchaseData.purchaseDate, currentPurchase->purchaseData.itemID); // Insert into the same line as customer's data all the purchased items
			currentPurchase = currentPurchase->next;
		}
		if(currentNode->next != NULL)
			fprintf(fp, "\n");														      // When covered all purchased items, insert \n to the line to be ready for next customer		
		currentNode = currentNode->next;
	}
	fclose(fp);
	fprintf(fplog, "%sCustomers loaded successfully into database\n", currentDateTime()); // data logger
}

/*
* bootCustomersToList read customers from txt to linked list.
* The data is using this tamplate: [joinDate]	[name]	 [moneySpent]	[purchasedItem1]	[purchasedItem2]	... [purchasedItemN]	
*	-> Read customers info
*	-> Insert it to linked list
*	-> For each customer create purchased items linked-list
*	-> Insert the customer's purchased items linked-list all purchased items.
*/
void bootCustomersToList(ListCustomer* csmlst) {
	FILE* fp = NULL;
	//char currentName[20];
	char ch;
	if (fp = fopen("customers.txt", "r")) {											// If file exist should return 1 and read the file
		Customer csm = { "","",0 };                                                 // Auxiliary struct to hold customer data from file
		while (!feof(fp)) {
			fscanf(fp, "%s   %s   %f", csm.dateJoin, csm.name, &csm.moneySpent);    // Stores customer's joinDate, name, moneySpend into the struct 
			addValueToCustomerList(csmlst, csm);                                    // Inserting the struct into customers linked-list
			NodeCustomer* currentCustomer = findCustomerByName(csmlst, csm.name);   // Stores pointer to current added customer
			currentCustomer->customerData.purchaseList = createListPurchase();      // Creating empty purchased items linked-list 
			while ((ch = fgetc(fp)) != EOF  && ch != '\n')  {                       // While loop until EOF or reaching next line. Each line in customers.txt shows customer data and all items customer bought.
				Purchase pur;                                                       // uxiliary struct to hold purchased item data from file
				fscanf(fp, "%s   %d", pur.purchaseDate, &pur.itemID);               // Stores purchased item id and purchaseDate into the struct 
				NodePurchase* currentPurchase = createNodePurchase(pur.itemID, pur.purchaseDate); // Creating node of the purchase
				addPurchaseToStartPurchaseList(currentCustomer->customerData.purchaseList, currentPurchase); // Inserting the struct into customer's purchased items linked-list
			}
		}
		fclose(fp);
		fprintf(fplog, "%sCustomers loaded successfully from database\n", currentDateTime()); // data logger
		printf("--->Customers added to database successfully\n");
	}
}

/*
* convertDateStringToInteger will convert each part of the date DD-MM-YYYY into 3 integers and return them by reference.
*/
void convertDateStringToInteger(char* date, int* day, int* month, int* year) {
	char tmp[2];								// tmp[2] to store date and month
	*day = atoi(strncpy(tmp, &date[0], 2));		// atoi() converts string to integer	
	*month = atoi(strncpy(tmp, &date[3], 2));   // strncpy(destination, source, num) copy into auxiliary string the required part of date 
	char tmp1[4];							    // to store year
	*year = atoi(strncpy(tmp1, &date[6], 4));
}

/*
* totalIncome calculates the total income of store, by checking how much each customer spend in store.
* This data stored in each customer node by moneySpent field.
*/
float totalIncome(ListCustomer* csmlst) {
	float spent = 0;
	NodeCustomer* currentNode = csmlst->head;
	while (currentNode != NULL) {
		spent += currentNode->customerData.moneySpent;
		currentNode = currentNode->next;
	}
	return spent;
}

void displayAllPurchases(ListCustomer* csmlst) {
	NodeCustomer* currentCustomer;
	char name[20];
	displayListCustomer(csmlst);
	printf("Enter customer name: ");
	scanf("%s", name);
	currentCustomer = findCustomerByName(csmlst, name);
	if (currentCustomer != NULL){
		displayListPurchsedByCustomer(currentCustomer->customerData.purchaseList);
		fprintf(fplog, "%s user displayed purchased items of %s\n", currentDateTime(), currentCustomer->customerData.name);
	}
}

/*
* displayListPurchsedByCustomer displays purchased items linked list of chosen customer
*/
void displayListPurchsedByCustomer(ListPurchase* purlst) { 
	NodePurchase* currentNode = purlst->head;
	while (currentNode != NULL) {
		printf("(%d   %s) ->   ", currentNode->purchaseData.itemID, currentNode->purchaseData.purchaseDate);
		currentNode = currentNode->next;
	}
	printf("NULL\n");
	fprintf(fplog, "%sPurchesed items list displayed to screen\n", currentDateTime());
}

/*
* addPurchaseToStartPurchaseList inserts purchased item node into the head of customer's purchased items linked-list.
* On this way, we make sure that the purchased items list is sorted.
*/
void addPurchaseToStartPurchaseList(ListPurchase* listpur, NodePurchase* nodepur) {
	NodePurchase* newNode = nodepur;
	if (!newNode) return;

	if (listpur->head == NULL) // If the list is empty
		listpur->head = newNode;
	else {
		newNode->next = listpur->head;
		listpur->head = newNode;
	}
}

/*
* purchaseItem sales chosen item to chosen customer. 
* On each sale, the customer can buy up to 3 items only.
* On each sale, the item is added to purchased items linked-list of the current customer.
*	-> Find customer on linked-list.
*	-> Choose item to buy
*	-> Add the item to customers purchesed items linked-list.
*   -> Display purchased items.
*/
void purchaseItem(ListCustomer* csmlst, ListItem* itmlst) {
	int itemsCnt = 0;
	int id;																	                   // ID of item to be purchased
	char name[20];															                   // Customer's name
	char datePurchase[11];
	int flagStopPurchase = 0;											                       // Flag to indicate if customer want to buy more items.
	displayListCustomer(csmlst);
	printf("Enter customer name: ");
	scanf("%s", name);
	NodeCustomer* currentCustomer = findCustomerByName(csmlst, name);                          // Search and store pointer to the customer that will purchase item 
	if (currentCustomer != NULL) {                                                             // The customer exists in the customer linked list
		strcpy(datePurchase, currentDate());                                  
		printf("Date of purchase: %s \n", datePurchase);
		do {
			displayListItemsInStock(itmlst);                                                   // Let the customer decide which item to buy from items that are in stock
			printf("\nHello Mr/Ms. %s. Choose item from the shelf by ID!: \n", currentCustomer->customerData.name);
			scanf("%d", &id);
			NodeItem* chosenItem = binarySearchItemByID(itmlst, id);			               // Search and store pointer to the item that has been chosen
			if (chosenItem != NULL) {                                                          // If the item exists in the items linked list
				if (chosenItem->itemData.inventory > 0) {								       // If the item is in stock
					chosenItem->itemData.inventory--;
					if (chosenItem->itemData.inventory == 0)								   // After the sale, if the items inventory became 0, update the onStock variable to be zero
						chosenItem->itemData.onStock = 0;
					currentCustomer->customerData.moneySpent += chosenItem->itemData.price;    // Update moneySpent field in customer's struct
					NodePurchase* itemPurchase = createNodePurchase(chosenItem->itemData.id, datePurchase);  // Create node of the struct Purchase  
					addPurchaseToStartPurchaseList(currentCustomer->customerData.purchaseList, itemPurchase);// Add the node to customers purchased items linked-list. Each purchase add to the start of the linked-list. So the linked-list will be sorted.
					itemsCnt++;                                                                // Counts how many items the customer purchased
					chosenItem->itemData.totalSales++;										   // Updating Item field totalSales, used later on to calculate store's statistics.
					fprintf(fplog, "%s%s sold to %s\n", currentDateTime(), chosenItem->itemData.model, currentCustomer->customerData.name); // data logger
				}
				if (itemsCnt < 3) {                                                            // Customer can buy up to 3 items
					printf("Do you want purchase more items? Enter 1 - Yes, 0 - No: ");
					scanf("%d", &flagStopPurchase);											   // If entered 0, the purchasing action will stop.
				}
				else
					printf("Cannot buy more then 3 items! Closing purchasing...\n");
			}
		} while (currentCustomer != NULL && itemsCnt < 3 && flagStopPurchase);                 // The purchasing action will go on while the customer didnt purchased 3 items or customer doesnt stopped the purchase OR the customer isnt in database 
		displayListPurchsedByCustomer(currentCustomer->customerData.purchaseList);             // Displays the summary of what the customer purchased
	}
}

/*
* isLeapYear check if the given year is leap. For year to be leap need to follow the conditions:
* year should devide by 4 by not devide by 100 OR devide by 400
* Return 1 if year is leap.
*/
int isLeapYear(int year) {
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		return 1;
	return 0;
}

/*
* Function checks if entered date is valid 
*/
int isValidDate(char* dateToCheck) {
	int year, month, day;
	convertDateStringToInteger(dateToCheck, &day, &month, &year);
	if (year <= 0) { // Check if year is valid (greater than 0)
		printf("Invalid date!\n");
		return 0;
	}

	if (month < 1 || month > 12) { // Check if month is valid (between 1 and 12)
		printf("Invalid date!\n");
		return 0;
	}
	// Check if day is valid for the given month
	int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (isLeapYear(year))
		daysInMonth[2] = 29; // Update February days for leap year
	if (day < 1 || day > daysInMonth[month]) {
		printf("Invalid date!\n");
		return 0;
	}
	return 1;
}

/*
* calculateDays will calculate how many days are from 01-01-0001 up to given date.
*	-> Calculate how many days up to year before.
*	-> Calculate how many days are in the given year up to 1 month before the give month
*	-> Calculate how many days up to give day.
*/
int calculateDays(int day, int month, int year) { 
	int days = (year - 1) * 365;    // Days in full years until one year before the give date
	int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // // Each month is placed in an array according to its month number. no month 0. each element contains how many days are in each month.
	for (int y = 1; y < year; y++)  // For each year from 0001 up to one year before given year add 1 day if it leap year
		if (isLeapYear(y)) 
			days++;
	for (int m = 1; m < month; m++) // For the given year, calculate how many days are in each month up to one month before the give month
		days += daysInMonth[m];     // dayInMonth[m] stores how many days in each month.
	if (isLeapYear(year))           
		if (month > 2)
			days++;
	days += day;                    // How many days up to given day on the given month.
	return days;
}

/*
* calculateDistanceBetweenDates calculates distance between 2 dates and returns integer.
* char* older, char* newer recives 2 strings.
* When older < newer, the function will return positive integer.
* When older > newer, the function will return negetive integer.
*/
int calculateDistanceBetweenDates(char* older, char* newer) {
	int yearolder, montholder, dayolder;									 // Auxiliary variables that will receive integers after breaking down the string
	int yearnewer, monthnewer, daynewer;									 // Auxiliary variables that will receive integers after breaking down the string
	convertDateStringToInteger(older, &dayolder, &montholder, &yearolder);   // Convert string to int
	convertDateStringToInteger(newer, &daynewer, &monthnewer, &yearnewer);   // Convert string to int
	int daysOlder = calculateDays(dayolder, montholder, yearolder);          // 01-01-0001 up to older
	int daysNewer = calculateDays(daynewer, monthnewer, yearnewer);		   	 // 01-01-01 up to newer
	return daysNewer - daysOlder;
}


/*
* deleteFirstPurchaseNode handle with deletion of first item in customer's purchased items list
*/
void deleteFirstPurchaseNode(ListPurchase* listpur) {
	NodePurchase* currentNode = listpur->head;
	listpur->head = currentNode->next;
	free(currentNode);
}

/*
* deleteItemFromPurchaseList delete node from customer purchased linked-list.
*/
void deleteItemFromPurchaseList(ListPurchase* listpur, int returnID) {
	NodePurchase* prevNode = listpur->head;
	NodePurchase* nodeToRemove = searchItemByIDPurchase(listpur, returnID);
	if (nodeToRemove == NULL) // No node with id
		printf("ID %d isnt in database", returnID);
	else if (listpur->head == nodeToRemove)
		deleteFirstPurchaseNode(listpur);
	else {
		while (prevNode->next != nodeToRemove)
			prevNode = prevNode->next;
		prevNode->next = nodeToRemove->next;
		free(nodeToRemove);
	}
}

/*
* searchItemByIDPurchase searching id of purchased item in purchased item linked-list
*/
NodePurchase* searchItemByIDPurchase(ListPurchase* listpur, int id) {
	NodePurchase* currentNode = listpur->head;
	while (currentNode != NULL) {
		if (currentNode->purchaseData.itemID == id)
			return currentNode;
		currentNode = currentNode->next;
	}
	return NULL;
}

/*
* returnItem deals with item return to store
*	-> Find the customer and the item to be return
*	-> Check if pass less then 14 days from purchase
*	-> If yes, delete the item from customer's linked-list, update the item's linked-list.
*	-> If no, print messege to screen: cannot be return.
* Assume: if the item is not longer available in the items list, the item cannot be returned.
*/
void returnItem(ListCustomer* csmlist, ListItem* itmlst) {
	char todayDate[11];
	int idReturn, distanceDates;
	char customerName[20];
	displayListCustomer(csmlist);
	do {
		printf("Enter customer name: (Max 20 characters) ");
		scanf("%s", customerName);
	} while (strlen(customerName) < 0 && strlen(customerName) > 20); 
	strcpy(todayDate, currentDate());
	NodeCustomer* currentCustomer = findCustomerByName(csmlist, customerName);                                   // Searching the customer in the linked-list
	displayListPurchsedByCustomer(currentCustomer->customerData.purchaseList);                                   // Displaying the purchased items
	if (currentCustomer->customerData.purchaseList->head != NULL) {
		printf("Enter which id you would like to return: ");
		scanf("%d", &idReturn);
		NodePurchase* currentNode = searchItemByIDPurchase(currentCustomer->customerData.purchaseList, idReturn);    // Searching the item in customer's purchased items linked-list
		if (currentNode != NULL) {                                                                                   // If the item exist
			distanceDates = calculateDistanceBetweenDates(currentNode->purchaseData.purchaseDate, todayDate);        // Calculating how many days pass from the purchase
			if (distanceDates <= 14 && distanceDates >= 0) {
				deleteItemFromPurchaseList(currentCustomer->customerData.purchaseList, idReturn);                    // If less then 14 days, deleting the purchase from the customer
				NodeItem* currentNodeItem = binarySearchItemByID(itmlst, idReturn);                                  // Searching item's id in the items linked-list
				if (currentNodeItem != NULL) {
					currentNodeItem->itemData.onStock = 1;		                                                     // Updating stock status of the item
					currentNodeItem->itemData.inventory++;
					currentNodeItem->itemData.totalSales--;
					currentCustomer->customerData.moneySpent -= currentNodeItem->itemData.price;                     // Updating moneySpent by customer
					fprintf(fplog, "%s%s returned by %s into stock\n", currentDateTime(), currentNodeItem->itemData.model, currentCustomer->customerData.name); // data logger
					printf("Item returned back to stock\n");
				}
				else // If this node has been deleted from items list
					printf("The item is not available in database!\n");
			}
			else
				printf("More then 14 days over.. cannot return item to store\n");
		}
		else
			printf("Item not found on purchased items\n");
	}
	else
		printf("Customer have not purchased items yet :(\n");
}

/*
* createNodePurchase creating node of each purchase
*/
NodePurchase* createNodePurchase(int id, char* date) {
	NodePurchase* newNode = (NodePurchase*)malloc(sizeof(NodePurchase)); // Allocating memory for the node
	if (!newNode) { // Allocation failed
		printf("Failed to create the node\n");
		return NULL;
	}
	newNode->purchaseData.itemID = id;
	strcpy(newNode->purchaseData.purchaseDate, date);
	newNode->next = NULL;
	return newNode;
}

/*
* createListPurchase creating empty linked list to store all of the purchases of each customer.
*/
ListPurchase* createListPurchase() {
	ListPurchase* newList = (ListPurchase*)malloc(sizeof(ListPurchase));
	if (!newList) { // Allocation failed
		printf("Failed to create employees linked list!\n");
		return NULL;
	}
	newList->head = NULL;
	//printf("Empty linked has been created\n");
	return newList;
}

/*
* currentDate create string with actual date and return string 
*/
char* currentDate() { 
	char current[11];
	time_t currentTime;                                                    // variable that will hold ephoch time (seconds from 01-01-1970 up to now)
	struct tm* timeInfo;                                                   // Struct to hold year, month, day, hour, minute, second
	char formattedTime[11];
	time(&currentTime);                                                    // Stores the ephoch time to currentTime                                     
	timeInfo = localtime(&currentTime);                                    // Convert to local time
	strftime(formattedTime, sizeof(formattedTime), "%d-%m-%Y", timeInfo);  // Converting the timeInfo struct to readable string
	strcpy(current, formattedTime);
	return current;
}


/*
* checkHowManyCustomers count how many customers are in database and return the amount
*/
int checkHowManyCustomers(ListCustomer* csmlst) {
	int cnt = 0;
	NodeCustomer* currentNode = csmlst->head;
	while (currentNode != NULL) {
		cnt++;
		currentNode = currentNode->next;
	}
	return cnt;
}

/*
* findCustomerByName searching by name in the customer list and returns pointer to the found node
*/
NodeCustomer* findCustomerByName(ListCustomer* csm, char* name) {
	NodeCustomer* currentNode = csm->head;
	fprintf(fplog, "%sSearching in customer list by name\n", currentDateTime());
	while (currentNode != NULL) {
		if (!strcmp(currentNode->customerData.name, name))
			return currentNode;
		currentNode = currentNode->next;
	}
	printf("Customer is not in database\n");
	return NULL;
}

/*
* deleteFirstCustomerNode handles with deletion of first node to linked-list
*/
void deleteFirstCustomerNode(ListCustomer* csm) {
	NodeCustomer* currentNode = csm->head;
	csm->head = currentNode->next;
	fprintf(fplog, "%sCustomer %s deleted from database\n", currentDateTime(), currentNode->customerData.name);
	free(currentNode);
}

/*
* deleteCustomerFromList deletes customer from customers linked list
*/
void deleteCustomerFromList(ListCustomer* csm) {
	char name[20];
	printf("Enter customer name\n");
	scanf("%s", name);
	NodeCustomer* prevNode = csm->head;
	NodeCustomer* nodeToRemove = findCustomerByName(csm, name);
	if (nodeToRemove == NULL) // No node with id
		printf("Customer %s is not in data base", name);
	else if (csm->head == nodeToRemove)
		deleteFirstCustomerNode(csm);
	else {
		while (prevNode->next != nodeToRemove)
			prevNode = prevNode->next;
		prevNode->next = nodeToRemove->next;
		fprintf(fplog, "%sCustomers %s deleted from database\n", currentDateTime(), nodeToRemove->customerData.name);
		free(nodeToRemove);
	}
}

/*
* Function searching by name in the customer list and returns pointer to the found node
*/
//NodeCustomer* findNodeWithName(ListCustomer* lst, char* name) {
//	NodeCustomer* currentNode = lst->head;
//	while (currentNode != NULL) {
//		if (strcmp(currentNode->customerData.name, name) == 0)
//			return currentNode;
//		currentNode = currentNode->next;
//	}
//	printf("Customer is not in database\n");
//	fprintf(fplog, "%sSearching in customer list by name", currentDateTime());
//	return NULL;
//}

/*
* displayListCustomer displays the linked-list to screen.
*/
void displayListCustomer(ListCustomer* csmlst) {
	NodeCustomer* currentNode = csmlst->head;
	while (currentNode != NULL) {
		printf("Name: %-20s   join date: %s ->  \n", currentNode->customerData.name, currentNode->customerData.dateJoin);
		currentNode = currentNode->next;
	}
	printf("NULL\n");
	fprintf(fplog, "%sCustomers list displayed to screen\n", currentDateTime());
}

/*
* addValueToStartCustomer handles with insertion of first node to linked-list
*/
void addValueToStartCustomer(ListCustomer* csmlst, Customer csm) {
	NodeCustomer* newNode = createNodeCustomer(csm);
	if (!newNode) return; // If allocation failed in creating new node

	if (csmlst->head == NULL) // If the list is empty
		csmlst->head = newNode;
	else {
		newNode->next = csmlst->head;
		csmlst->head = newNode;
	}
}

/*
* addValueToCustomerList adding customers into the linked list. 
* The linked list is sorted by joining date.
*/
void addValueToCustomerList(ListCustomer* lst, Customer csm) {
	NodeCustomer* newNode = createNodeCustomer(csm);
	if (!newNode) return; // Allocation failed
	NodeCustomer* currentCustomer = lst->head;
	if (lst->head == NULL || calculateDistanceBetweenDates(currentCustomer->customerData.dateJoin, csm.dateJoin) >= 0) // Incase that the list is empty or the inserted node should be the first
		addValueToStartCustomer(lst, csm);
	else {
		NodeCustomer* nextCustomer = lst->head->next;
		while (currentCustomer->next != NULL && calculateDistanceBetweenDates(nextCustomer->customerData.dateJoin, csm.dateJoin) <= 0) { // Searching for the correct place to insert the node
			currentCustomer = currentCustomer->next;
			nextCustomer = nextCustomer->next;
		}
		currentCustomer->next = newNode; // Node insertion
		newNode->next = nextCustomer;
	}
}

/*
* createNewCustomer creating new customer data and insert it into the customer linked-list
*/
void createNewCustomer(ListCustomer* csmList) {
	Customer newCustomer;
	printf("Create a new customer: \n");
	do {
		printf("Enter Name: (max 20 characters) ");
		scanf("%s", newCustomer.name);
	} while (strlen(newCustomer.name) > 20 || strlen(newCustomer.name) < 1); 
	strcpy(newCustomer.dateJoin, currentDate()); 
	printf("Join date automaticly configured!\n");
	newCustomer.moneySpent = 0;
	newCustomer.purchaseList = createListPurchase(); // Each customer has linked-list of purchased items
	addValueToCustomerList(csmList, newCustomer);    // Customer inserted to sorted linked-list by date of join
	printf("New customer name: %s added to database successfully!\n", newCustomer.name);
	fprintf(fplog, "%sNew customer %s added to database\n", currentDateTime(), newCustomer.name);
}


/*
* createNodeCustomer creating node for customer linked-list
*/
NodeCustomer* createNodeCustomer(Customer csm) {
	NodeCustomer* newNode = (NodeCustomer*)malloc(sizeof(NodeCustomer)); // Allocating memory for the node
	if (!newNode) { // If allocation failed
		printf("Failed to create the node\n");
		return NULL;
	}
	newNode->customerData = csm;
	newNode->next = NULL;
	return newNode;
}

/*
* createListCustomer creating empty linked list to store all of the customers.
*/
ListCustomer* createListCustomer() {
	ListCustomer* newList = (ListCustomer*)malloc(sizeof(ListCustomer)); // Allocating memory for the list
	if (!newList) { // If allocation failed
		printf("Failed to create customer linked-list\n");
		return NULL;
	}
	newList->head = NULL;
	return newList;
}

