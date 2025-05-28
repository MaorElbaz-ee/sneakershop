//////////////////////////////////////////
// Created by: Nikolai Akulov, 316868132//
//             Maor Elbaz,     208418293//
//             Shay Shitrit,   207895798//
//////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include "items.h"
#include "employees.h"
#include "customers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

ListItem* itemsList;
FILE* fplog;

/*
* writeItemsDataToFile write customers linked list into txt file
* The data is using this tamplate: [id]    [brand]    [model]	[price]    [onStock]    [date]    [inventory]    [totalSales]
*  -> Insert items data from node to items.txt
*/
void writeItemsDataToFile(ListItem* itmlst) {
	FILE* fp = fopen("items.txt", "w");
	NodeItem* currentNode = itmlst->head;        // Pointer to the first item in the linked-list
	while (currentNode != NULL) {
		fprintf(fp, "%-10d    %-20s    %-20s    %7.2f    %d    %-10s    %-4d    %-6d", currentNode->itemData.id, currentNode->itemData.brand, currentNode->itemData.model, currentNode->itemData.price, currentNode->itemData.onStock, currentNode->itemData.date, currentNode->itemData.inventory, currentNode->itemData.totalSales);
		if (currentNode->next != NULL)           // If it isnt the last node in the linked-list, insert \n
			fprintf(fp, "\n");
		currentNode = currentNode->next;
	} 
	fprintf(fplog, "%sItems loaded successfully into database\n", currentDateTime());  // data logger
	fclose(fp);
}

/*
* bootItemsToList read items from txt to linked list.
* The data is using this tamplate: [id]    [brand]    [model]	[price]    [onStock]    [date]    [inventory]    [totalSales]
*	-> Read items info from txt file
*	-> Insert it to linked list
*/
void bootItemsToList(ListItem* itmlst) {
	FILE* fp = NULL;
	Item itm;                                                  // Auxiliary struct to hold customer data from file
	if (fp = fopen("items.txt", "r")) {                        // If file exist should return 1 and read the file
		while (!feof(fp)) {
			fscanf(fp, "%d %s %s %f %d %s %d %d ", &itm.id, itm.brand, itm.model, &itm.price, &itm.onStock, itm.date, &itm.inventory, &itm.totalSales); 
			addValueToItemsList(itmlst, itm);				   // Inserting the struct into items linked-list
		}
		fclose(fp);
		printf("--->Items added to database sucsesfully\n");
		fprintf(fplog, "%sItems loaded successfully from database\n", currentDateTime());
	}
}

/*
* addInventoryToAllStock adds stock to all items in the linked list
*/
void addInventoryToAllStock(ListItem* itmlst) {
	int inventoryToAdd;
	NodeItem* currentNode = itmlst->head;
	printf("Enter amount to add or decrease to all inventory:, negetive int to decrease, positive int to increase: ");
	scanf("%d", &inventoryToAdd);
	while (currentNode != NULL) {
		currentNode->itemData.inventory += inventoryToAdd;
		if (currentNode->itemData.inventory > 0)
			currentNode->itemData.onStock = 1;
		currentNode = currentNode->next;
	}
	printf("All items inventory updated by %d\n", inventoryToAdd);
	fprintf(fplog, "%sAll items inventory updated by %d\n", currentDateTime(), inventoryToAdd);
}

/*
* showMostPopularModel searching for the item with the maximum totalSales field
*/
char* showMostPopularModel(ListItem* itmlst) {
	NodeItem* currentNode = itmlst->head;
	int mostSales = 0;
	char popularModel[20];
	while (currentNode != NULL) {
		if (currentNode->itemData.totalSales > mostSales)
			strcpy(popularModel, currentNode->itemData.model);
		currentNode = currentNode->next;
	}
	return popularModel;
}

/*
* displayListItemsInStock displays all items linked list that field onStock is TRUE/"1".
*/
void displayListItemsInStock(ListItem* list) {
	NodeItem* currentNode = list->head;
	while (currentNode != NULL) {
		if (currentNode->itemData.inventory > 0)
			printf("ID: %-6d Brand: %-20s Model: %-20s Quantity: %-6d Price: %7.2f -> \n", currentNode->itemData.id, currentNode->itemData.brand, currentNode->itemData.model, currentNode->itemData.inventory, currentNode->itemData.price);
		currentNode = currentNode->next;
	}
	printf(" NULL\n");
	fprintf(fplog, "%sItems list displayed to screen\n", currentDateTime());
}


int calculateTotalSales(ListItem* itmlst) {
	NodeItem* currentNode = itmlst->head;
	int totalSales = 0;
	while (currentNode != NULL) {
		totalSales += currentNode->itemData.totalSales;
		currentNode = currentNode->next;
	}
	return totalSales;
}

/*
* deleteFirstItemNode handles with deletion of first node to linked-list
*/
void deleteFirstItemNode(ListItem* itemsList) {
	NodeItem* currentNode = itemsList->head;
	itemsList->head = currentNode->next;
	fprintf(fplog, "%sItem id %d deleted from database\n", currentDateTime(), currentNode->itemData.id);
	free(currentNode);
}

/*
* deleteCustomerFromList deletes item from customers linked list
*/
void deleteItemFromList(ListItem* itemsList) {
	int id;
	displayListItems(itemsList);
	printf("Enter ID of item to delete\n");
	scanf("%d", &id);
	NodeItem* prevNode = itemsList->head;
	NodeItem* nodeToRemove = binarySearchItemByID(itemsList, id);
	if (nodeToRemove == NULL) // No node with id
		printf("ID %d isnt in database", id);
	else if (itemsList->head == nodeToRemove)
		deleteFirstItemNode(itemsList);
	else {
		while (prevNode->next != nodeToRemove)
			prevNode = prevNode->next;
		prevNode->next = nodeToRemove->next;
		fprintf(fplog, "%sItem id %d deleted from database\n", currentDateTime(), nodeToRemove->itemData.id);
		free(nodeToRemove);
	}
}

/*
* findNodeByItem searching item by given ID and returns pointer to this node.
*/
NodeItem* findNodeByItem(ListItem* lst, Item itm) { 
	NodeItem* currentNode = lst->head;
	while (currentNode != NULL) {
		if (currentNode->itemData.id == itm.id)
			return currentNode;
		currentNode = currentNode->next;
	}
	return NULL;
}

/*
* convertListToArray converting items linked-list into array of Item struct
*/
Item* convertListToArray(ListItem* lst, int* arraySize) { 
	NodeItem* currentNode = lst->head;                                    // Pointer to the first node in the linked-list
	Item* itemsArray = NULL;                                             
	int size = 0;
	while (currentNode != NULL) {
		Item* temp = (Item*)realloc(itemsArray, (++size) * sizeof(Item)); // Reallocate memory for the array according to size
		if (temp != NULL) {                                               // allocation successed
			itemsArray = temp;
			itemsArray[size - 1] = currentNode->itemData;                 // Inserting the data into array's element
			currentNode = currentNode->next;
		}
	}
	*arraySize = size;
	return itemsArray;
}

/*
* binarySearchItemByID is a binary search inside a array of itmes
* The array is sorted by ID field
*	-> Find the middle of array.
*	-> if the id is in middle, return it if true
*	-> else if it smaller, define the low to be 0 and high to be middle-1
*	-> else if it greater, define the low to be middle+1 and high to be size-1
*	-> check again if: if the id is in middle, return it if true
*
*/
NodeItem* binarySearchItemByID(ListItem* itemsList, int id) {
	int arraySize = 0;
	Item* itemsArray = convertListToArray(itemsList, &arraySize);
	NodeItem* currentItem;
	int low = 0;
	int high = arraySize;
	while (low <= high) {
		int mid = low + ((high - low) / 2);
		if (itemsArray[mid].id == id) {
			currentItem = findNodeByItem(itemsList, itemsArray[mid]);
			return currentItem;
		}
		else if (itemsArray[mid].id < id)
			low = mid + 1;
		else high = mid - 1;
	}
	printf("Item not found!\n");
	return NULL;
}

/*
* Search items by ID
*/
int checkIfIdOnList(ListItem* itemsList, int id) {
	NodeItem* currentNode = itemsList->head;
	while (currentNode != NULL) {
		if (currentNode->itemData.id == id)
			return 1;
		currentNode = currentNode->next;
	}
	return 0;
}

/*
* Applying search on items linked list by catergory
*/
void searchItemByCategory(ListItem* lst) { // NEED TO CHANGE IF DATE IS CORRECt
	int category;
	if (lst->head == NULL) { // If list is empty
		printf("There aren't items in database\n");
		return;
	}
	ListItem* isFound = createListItem();
	NodeItem* currentNode = lst->head;
	printf("Searching menu:\n");
	printf("0.  Search by items that their ID is less then X\n");
	printf("1.  Search by items that their ID is greater then X\n");
	printf("2.  Search by brand\n");
	printf("3.  Search by model\n");
	printf("4.  Search by items that their price is less then X\n");
	printf("5.  Search by items that their price is greater then X\n");
	printf("6.  Search items that they are in stock now\n");
	printf("7.  Search item that entered into stock before date\n");
	printf("8.  Search item that entered into stock after date\n");
	printf("9.  Search item that entered into stock between 2 dates\n");
	printf("10. Search items whose quantity in stock is less then X\n");
	printf("11. Search items whose quantity in stock is more then X\n");
	printf("12. Search by brand & model\n");
	printf("13. EXIT\n");
	printf("Enter your choise:");
	scanf("%d", &category);

	switch (category) {
	case 0: { // Search by smaller then id
		printf("Enter id to search up to: (Greater then zero) \n");
		int id;
		do {
			scanf("%d", &id);
		} while (id < 0);
		while (currentNode != NULL) {
			if (currentNode->itemData.id <= id)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by id\n", currentDateTime());
		break;
	}
	case 1: { // Search by higher then id
		printf("Enter id to search up to: (Greater then zero) \n");
		int id;
		do {
			scanf("%d", &id);
		} while (id < 0);
		while (currentNode != NULL) {
			if (currentNode->itemData.id >= id)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by id\n", currentDateTime());
		break;
	}
	case 2: { // Search by brand
		char brandToSearch[20];
		printf("Enter a brand name to search by:");
		scanf("%s", brandToSearch);
		while (currentNode != NULL) {
			if (!strcmp(currentNode->itemData.brand, brandToSearch))
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by brand\n", currentDateTime());
		break;
	}
	case 3: { // Search by model
		printf("Enter a model name to search by: ");
		char modelToSearch[20];
		scanf("%s", modelToSearch);
		while (currentNode != NULL) {
			if (!strcmp(currentNode->itemData.model, modelToSearch))
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by model\n", currentDateTime());
		break;
	}
	case 4: { // Search by price less then X
		printf("Enter a price to search by: (Greater then zero) ");
		float priceToSearch;
		do {
			scanf("%f", &priceToSearch);
		} while (priceToSearch < 0);
		while (currentNode != NULL) {
			if (currentNode->itemData.price <= priceToSearch)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by price\n", currentDateTime());
		break;
	}
	case 5: { // Search by price greater then X
		printf("Enter a price to search by: (Greater then zero) ");
		float priceToSearch;
		do {
			scanf("%f", &priceToSearch);
		} while (priceToSearch < 0);
		while (currentNode != NULL) {
			if (currentNode->itemData.price >= priceToSearch)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by price\n", currentDateTime());
		break;
	}
	case 6: { // Search if in stock
		printf("Enter 1 to search if the item in stock, or 0 to search if item isn't in stock ");
		int valueToSearch;
		do {
			scanf("%d", &valueToSearch);
		} while (valueToSearch != 0 && valueToSearch != 1);
		while (currentNode != NULL) {
			if (currentNode->itemData.onStock == valueToSearch)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched for all items in stock\n", currentDateTime());
		break;
	}
	case 7: { // Search by date - Before this date
		char dateToSearch[11];
		do {
			printf("Enter date DD-MM-YYYY to search by ");
			scanf("%s", dateToSearch);
		} while (!isValidDate(dateToSearch));
		while (currentNode != NULL) {
			if (calculateDistanceBetweenDates(currentNode->itemData.date, dateToSearch) >= 0)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by dates\n", currentDateTime());
		break;
	}
	case 8: { // Search by date - After this date
		char dateToSearch[11];
		do {
			printf("Enter date DD-MM-YYYY to search by ");
			scanf("%s", dateToSearch);
		} while (!isValidDate(dateToSearch));
		while (currentNode != NULL) {
			if (calculateDistanceBetweenDates(currentNode->itemData.date, dateToSearch) <= 0)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by dates\n", currentDateTime());
		break;
	}
	case 9: { // Search items between dates
		char dateStartFrom[11];
		do {
			printf("Enter start date DD-MM-YYYY to search: ");
			scanf("%s", dateStartFrom);
		} while (!isValidDate(dateStartFrom));

		char dateStop[11];
		do {
			printf("Enter end date DD-MM-YYYY: ");
			scanf("%s", dateStop);
		} while (!isValidDate(dateStop));
		while (currentNode != NULL) {
			if (calculateDistanceBetweenDates(currentNode->itemData.date, dateStop) >= 0 && calculateDistanceBetweenDates(currentNode->itemData.date, dateStartFrom) <= 0)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by dates\n", currentDateTime());
		break;
	}
	case 10: { // Less inventory then X
		printf("Enter a amount to search by: (Greater then zero) ");
		int inventoryToSearch;
		do {
			scanf("%d", &inventoryToSearch);
		} while (inventoryToSearch < 0);
		while (currentNode != NULL) {
			if (currentNode->itemData.inventory <= inventoryToSearch)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by inventory\n", currentDateTime());
		break;
	}
	case 11: { // More inventory then X
		printf("Enter a amount to search by: (Greater then zero) ");
		int inventoryToSearch;
		do {
			scanf("%d", &inventoryToSearch);
		} while (inventoryToSearch < 0);
		while (currentNode != NULL) {
			if (currentNode->itemData.inventory >= inventoryToSearch)
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by inventory\n", currentDateTime());
		break;
	}
	case 12: { // Search by brand and model
		printf("Search by brand and model \nEnter brand name: ");
		char brand[20];
		scanf("%s", brand);
		printf("Enter model name: ");
		char model[20];
		scanf("%s", model);
		while (currentNode != NULL) {
			if (!strcmp(currentNode->itemData.brand, brand) && !strcmp(currentNode->itemData.model, model))
				addValueToItemsList(isFound, currentNode->itemData);
			currentNode = currentNode->next;
		}
		fprintf(fplog, "%sUser searched items by brandl and model\n", currentDateTime());
		break;
	}
	case 13: {
		break;
	}
	}
	displayListItems(isFound);
	fprintf(fplog, "%sUser has been deployed search in items database\n", currentDateTime());
	if (isFound->head != NULL) {
		int choose;
		printf("Do you want to edit an item from search? Enter 1 for YES or 0 for NO: ");
		scanf("%d", &choose);
		if (choose == 1)
			updateItemFromListMenu(isFound);
	}
}

/*
* updateItemFromListMenu to edit values of node by emoloyee choise
*/
void updateItemFromListMenu(ListItem* itemsList) {
	int id, choise;
	NodeItem* nodeToEdit;
	displayListItems(itemsList);
	printf("Enter ID of item to be edited: ");
	scanf("%d", &id);
	nodeToEdit = binarySearchItemByID(itemsList, id);
	if (nodeToEdit != NULL) {
		printf("Item's data:\n");
		displayItemNodeData(nodeToEdit);
		printf("Edit menu\n");
		printf("1.  Edit brand\n");
		printf("2.  Edit model\n");
		printf("3.  Edit price\n");
		printf("4.  Edit inventory\n");
		printf("Enter your choise: ");
		scanf("%d", &choise);
		switch (choise) {
		case 1: { // Edit brand
			char newBrand[20];
			do {
				printf("Enter Brand: (max 20 characters)");
				scanf("%s", newBrand);
			} while (strlen(newBrand) > 20 || strlen(newBrand) < 1);
			strcpy(nodeToEdit->itemData.brand, newBrand);
			fprintf(fplog, "%sUser updated item id: %d brand to: %s\n", currentDateTime(), nodeToEdit->itemData.id, nodeToEdit->itemData.brand);
			break;
		}
		case 2: { // Edit model
			char newModel[20];
			do {
				printf("Enter Model: (max 20 characters) ");
				scanf("%s", newModel);
			} while (strlen(newModel) > 20 || strlen(newModel) < 1);
			strcpy(nodeToEdit->itemData.model, newModel);
			fprintf(fplog, "%sUser updated item id: %d model to: %s\n", currentDateTime(), nodeToEdit->itemData.id, nodeToEdit->itemData.model);
			break;
		}
		case 3: { // Edit price
			float newPrice;
			do {
				printf("Enter a price: (positive float)");
				scanf("%f", &newPrice);
			} while (newPrice < 0);
			nodeToEdit->itemData.price = newPrice;
			fprintf(fplog, "%sUser updated item id: %d price to: %f\n", currentDateTime(), nodeToEdit->itemData.id, nodeToEdit->itemData.price);
			break;
		}
		case 4: { // Edit inventory
			int newInventory;
			do {
				printf("Enter the quantity: (0 or greater)");
				scanf("%d", &newInventory);
			} while (newInventory < 0);
			nodeToEdit->itemData.inventory = newInventory;
			fprintf(fplog, "%sUser updated item id: %d inventory to: %d\n", currentDateTime(), nodeToEdit->itemData.id, nodeToEdit->itemData.inventory);
			if (newInventory > 0)
				nodeToEdit->itemData.onStock = 1;
			else
				nodeToEdit->itemData.onStock = 0;
			break;
		}
		}
		printf("Item's data changed to:\n");
		//fprintf(fplog, "%sItem id %d updated\n", currentDateTime(), nodeToEdit->itemData.id);
		displayItemNodeData(nodeToEdit);
	}
	else {
		printf("ERROR!\n");
	}
}

/*
* displayItemNodeData display actual node of item on screen
*/
void displayItemNodeData(NodeItem* node) {
	printf("id: %d\n", node->itemData.id);
	printf("brand: %s\n", node->itemData.brand);
	printf("model: %s\n", node->itemData.model);
	printf("price: %f\n", node->itemData.price);
	printf("date: %s\n", node->itemData.date);
	printf("inventory: %d\n", node->itemData.inventory);
	fprintf(fplog, "%sItem node id %d displayed on screen\n", currentDateTime(), node->itemData.id);
}

/*
* displayListItems displays items linked list on screen
*/
void displayListItems(ListItem* list) {
	NodeItem* currentNode = list->head;
	while (currentNode != NULL) {
		printf("ID: %-6d Brand: %-20s Model: %-20s Quantity: %d -> \n", currentNode->itemData.id, currentNode->itemData.brand, currentNode->itemData.model, currentNode->itemData.inventory);
		currentNode = currentNode->next;
	}
	printf(" NULL\n");
	fprintf(fplog, "%sItems linked list displayed to screen\n", currentDateTime());
}

/*
* This is auxiliary that handles with insertion of first node into linked list
*/
void addValueToStartItem(ListItem* lst, Item itm) {
	NodeItem* newNode = createNodeItem(itm);
	if (!newNode) return;

	if (lst->head == NULL) // If the list is empty
		lst->head = newNode;
	else {
		newNode->next = lst->head;
		lst->head = newNode;
	}
}

/*
* Function add item struct into linked list.
* The list is sorted by id
*/
void addValueToItemsList(ListItem* lst, Item itm) {
	NodeItem* newNode = createNodeItem(itm);
	if (!newNode) return; // Allocation failed

	NodeItem* currentItem = lst->head;
	if (lst->head == NULL || currentItem->itemData.id > itm.id) // Incase that the list is empty or the first value is greater.
		addValueToStartItem(lst, itm);
	else {
		NodeItem* nextItem = lst->head->next;
		while (currentItem->next != NULL && nextItem->itemData.id < itm.id) {
			currentItem = currentItem->next;
			nextItem = nextItem->next;
		}
		currentItem->next = newNode;
		newNode->next = nextItem;
	}
}

/*
* createNodeItem creating node from item struct
*/

NodeItem* createNodeItem(Item itm) {
	NodeItem* newNode = (NodeItem*)malloc(sizeof(NodeItem)); // Allocating memory for the node
	if (!newNode) {
		printf("Failed to create the node\n");
		return NULL;
	}
	newNode->itemData = itm;
	newNode->next = NULL;
	return newNode;
}

/*
* Function creates new item struct and insert it into the items linked list
* Items linked list is sorted by id.
*/
void createNewItem(ListItem* itemsList) {
	printf("Create a new item: \n");
	Item newItem;
	do {
		printf("Enter a new ID: (positive integer) ");
		scanf("%d", &newItem.id);

	} while (newItem.id < 0 || checkIfIdOnList(itemsList, newItem.id));
	do {
		printf("Enter Brand: (max 20 characters) ");
		scanf("%s", newItem.brand);
	} while (strlen(newItem.brand) > 20 || strlen(newItem.brand) < 1);
	do {
		printf("Enter Model: (max 20 characters) ");
		scanf("%s", newItem.model);
	} while (strlen(newItem.model) > 20 || strlen(newItem.model) < 1);
	do {
		printf("Enter a price: (positive float) ");
		scanf("%f", &newItem.price);
	} while (newItem.price < 0);

	strcpy(newItem.date, currentDate());
	printf("Automaticly adding today's date: %s\n", newItem.date);
	newItem.totalSales = 0;
	do {
		printf("Enter the quantity: (1 or greater) ");
		scanf("%d", &newItem.inventory);
	} while (newItem.inventory <= 0);
	newItem.onStock = 1;
	addValueToItemsList(itemsList, newItem);
	printf("New item id: %d, brand %s, model %s added to database successfully!\n", newItem.id, newItem.brand, newItem.model);
	fprintf(fplog, "%sItem id: %d model: %s added successfully to database\n", currentDateTime(), newItem.id, newItem.model);
}

/*
* Function creating linked list to store items
*/
ListItem* createListItem() {
	ListItem* newList = (ListItem*)malloc(sizeof(ListItem));
	if (!newList) {
		printf("Failed to create employees linked list!\n");
		return NULL;
	}
	newList->head = NULL;
	//printf("Empty linked has been created\n");
	return newList;
}
