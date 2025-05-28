//////////////////////////////////////////
// Created by: Nikolai Akulov, 316868132//
//             Maor Elbaz,     208418293//
//             Shay Shitrit,   207895798//
//////////////////////////////////////////
#pragma once
#include <stdbool.h>

typedef struct item {
	int id;
	char brand[20];
	char model[20];
	float price;
	bool onStock;
	char date[11];
	int inventory;
	int totalSales;
} Item;

typedef struct nodeitem {
	Item itemData;
	struct nodeitem* next;
}NodeItem;

typedef struct listItem {
	NodeItem* head;
}ListItem;

void writeItemsDataToFile(ListItem* itmlst);
void bootItemsToList(ListItem* itmlst);
void addInventoryToAllStock(ListItem* itmlst);
char* showMostPopularModel(ListItem* itmlst);
void displayListItemsInStock(ListItem* list);
int calculateTotalSales(ListItem* itmlst);
void deleteItemFromList(ListItem* itemsList);
void deleteFirstItemNode(ListItem* itemsList);
NodeItem* findNodeByItem(ListItem* lst, Item itm);
Item* convertListToArray(ListItem* lst, int* arraySize);
NodeItem* binarySearchItemByID(ListItem* itemsList, int id);
int checkIfIdOnList(ListItem* itemsList, int id);
void searchItemByCategory(ListItem* lst);
void updateItemFromListMenu(ListItem* itemsList);
void displayItemNodeData(NodeItem* node);
void displayListItems(ListItem* list);
void addValueToItemsList(ListItem* lst, Item itm);
void addValueToStartItem(ListItem* list, Item itm);
NodeItem* createNodeItem(Item itm);
void createNewItem(ListItem* itemsList);
ListItem* createListItem();