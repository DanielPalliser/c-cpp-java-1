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

#define INPUT_SIZE 7
#define MIN_LAT -90
#define MAX_LAT 90
#define MIN_LNG -180
#define MAX_LNG 180
#define LAT_MIN_PROX 0.1001 //added 0.0001 to compensate for float error
#define LNG_MIN_PROX 0.2001

int main() {
	run_menu();
	return EXIT_SUCCESS;
}

void run_menu(void) {
	float lat = 1000, lng = 1000;
	char choice = '\0';
	while ('q' != choice && 'Q' != choice) { //loop through menu
		puts(
				"\nEnter a choice: \nL : enter location\nP : check proximity\nQ : quit");
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
				ship ships[10];
				int num_ships = get_ships(ships);
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
}


/* this method compares the locations of ships to the location specified */
void compare_locs(ship* ships, int num_ships, float lat, float lng) {
	int fd = open("log.txt", (O_RDWR | O_CREAT | O_APPEND)); //attempt to find log.
	if (fd == -1) {
		printf("File descriptor error");
		return;
	}

	struct flock* fl = file_lock(F_WRLCK, SEEK_SET);

	if (fcntl(fd, F_SETLK, fl) == -1) { //try to lock file
		if (errno == EACCES || errno == EAGAIN) {
			printf("Already locked by another process... bad luck\n");
			exit(1);
		} else {
			/* Handle unexpected error */;
		}
	} else { //if file is locked then run ship comparisons
		puts("OK we have the lock\n\n");
		FILE* log = fdopen(fd, "a");
		fprintf(log,
				"proximity indication check was made for point: %.3f lat %.3f lng",
				LAT_MIN_PROX, LAT_MIN_PROX);
		int i;
		for (i = 0; i < num_ships; i++) { //need to compensate for float error
			float lat_proximity = ships[i].lat - lat;
			float lng_proximity = ships[i].lng - lng;
			//compare ships to lat + lng
			if (lat_proximity < LAT_MIN_PROX && lng_proximity < LNG_MIN_PROX) {
				printf("ship with mmsi '%d' is close to the given point",
						ships[i].mmsi);
				fprintf(log, "ship with mmsi '%d' is close to the given point",
						ships[i].mmsi);
			}
		}
		fcntl(fd, F_SETLKW, file_lock(F_UNLCK, SEEK_SET));
		fclose(log);
	}
}
/* gets ships by looking for ship files in the working directory*/
int get_ships(ship* ships) {
	int num_ships = 0;
	char dirname[FILENAME_MAX];
	getcwd(dirname, FILENAME_MAX);//get working directory
	DIR *dir;
	dir = opendir(dirname); //open working directory
	struct dirent *file;
	if (dir != NULL) { //dir should not be null as there should be a working directory but safeguard anyway
		while ((file = readdir(dir)) != NULL) { //loop through directory
			if (NULL != (strstr((file->d_name), ".shp"))) { //if ship file
				ships[num_ships] = *(read_ship(file));
				num_ships++;
				if (num_ships == (sizeof(ships))) {
					realloc(ships, (sizeof(ships) * 2));
				}
			}
		}
	}
	return num_ships;
}

/**
 * reads in ship files, extracting the information and constructing a ship struct from them
 */
ship *read_ship(struct dirent *file) {
	ship *sh = malloc(sizeof(ship));
	char mmsi[9], name[40], lat[8], lng[8], course[4], speed[5];
	FILE *fp = fopen(file->d_name, "r");
	fgets(mmsi, sizeof(mmsi), fp);
	fgets(name, sizeof(name), fp);
	fgets(lat, sizeof(lat), fp);
	fgets(lng, sizeof(lng), fp);
	fgets(course, sizeof(course), fp);
	fgets(speed, sizeof(speed), fp);
	sh->mmsi = atoi(mmsi);
	strcpy(sh->name, name);
	sh->lat = atof(lat);
	sh->lng = atof(lng);
	sh->course = atoi(course);
	sh->speed = atof(speed);
	return sh;
}
