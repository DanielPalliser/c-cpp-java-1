/*
 * linkedlist.h
 *
 *  Created on: Mar 30, 2012
 *      Author: dan
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

void add_ship(ship **head, ship *new, int *listsize);
void display_ship(ship *ship);
void display_list(ship **head, int listsize);
void display_ship(ship *ship);
void remove_ships(ship **head,int *listsize);

#endif /* LINKEDLIST_H_ */
