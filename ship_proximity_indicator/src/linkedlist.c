#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ships.h"
#include "linkedlist.h"

void add_ship(ship **head, ship *new, int *listsize) {

	if (NULL == *head) {
		*head = new;
	} else {
		new->next = *head;
		*head = new;
	}
	(*listsize)++;
}

void display_list(ship **head, int listsize) {
	int i;
	ship *current = *head;
	for (i = 0; i < listsize; i++) {
		display_ship(current);
		current = current->next;
	}
}

void display_ship(ship *ship) {
	printf("\nAIS ID:%d\nLatitude:%f\nLongditude:%f\n", ship->mmsi, ship->lat,
			ship->lng);
}

void remove_ships(ship **head, int *listsize) {
	if (1 == *listsize) {
		free(*head);
		*head = NULL;
	} else {
		ship *previous;
		int i;
		ship *current = *head;
		for (i = 0; i < *listsize; i++) {
			previous = current;
			current = current->next;
			free(previous);
			previous = NULL;
		}
	}*listsize = 0;
}

