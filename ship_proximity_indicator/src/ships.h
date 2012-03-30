/*
 * main.h
 *
 *  Created on: Mar 30, 2012
 *      Author: dan
 */
#include <dirent.h>
#include <unistd.h>

#ifndef SHIPS_H_
#define SHIPS_H_



typedef struct ship_struct {
	int mmsi;
	char name[31];
	float lat;
	float lng;
	int course;
	float speed;
	struct ship_struct *next;
}ship;


struct flock* file_lock(short type, short whence);
void run_menu(void);
void get_input(char *prompt, float *pointer);
void compare_locs(ship* ships, int num_ships,float lat,float lng);
ship *read_ship(struct dirent *file);
int get_ships(ship** ships);

#endif /* SHIPS_H_ */
