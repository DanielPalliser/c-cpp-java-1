/*
 * main.c
 *
 *  Created on: Mar 26, 2012
 *      Author: dan
 */
#define __USE_POSIX 1
#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ships.h"
#include "linkedlist.h"

#define MIN_LAT -90
#define MAX_LAT 90
#define MIN_LNG -180
#define MAX_LNG 180



ship* head;

int main() {
	run_menu();
	return EXIT_SUCCESS;
}

void run_menu(void) {
	float lat = 1000, lng = 1000;
	char choice = '\0';
	while ('q' != choice && 'Q' != choice) { //loop through menu
		puts("\nEnter a choice: \nL : enter location\nP : check proximity\nQ : quit");
		choice = getchar();
		char ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
		switch (choice) {
		case 'l':
		case 'L':
			/* get input*/
			get_input("enter lat", &lat);
			get_input("enter long", &lng);
			break;
		case 'p':
		case 'P':
			/* check lat and lng are valid. if invalid input used then these will be out of range*/
			if (MIN_LAT < lat && lat < MAX_LAT && MIN_LNG < lng && lng < MAX_LNG) {
				ship *ships = NULL;
				int num_ships = (get_ships(&ships));
				compare_locs(ships, num_ships, lat, lng);
			} else {
				puts("invalid latitude & longitude");
			}
			break;
		}
	}
}
/* takes input as a prompt string and float pointer, checks input is valid */
void get_input(char *prompt, float *pointer) {
	int done = 0;
	puts(prompt);
	done = scanf("%f", pointer);
	if (done != 1) {
		*pointer = 1000.0; // set to invalid lat or lng
		puts("invalid input");
	}
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}

/* this method compares the locations of ships to the location specified */

