#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ships.h"
#include "linkedlist.h"


#define LAT_MIN_PROX 0.1001 //added 0.0001 to compensate for inaccuracy of floats
#define LNG_MIN_PROX 0.2001

struct flock* file_lock(short type, short whence) {
    static struct flock ret;
    ret.l_type = type;
    ret.l_start = 0;
    ret.l_whence = whence;
    ret.l_len = 0;
    ret.l_pid = getpid();
    return &ret;
}

void compare_locs(ship *ships, int num_ships, float lat, float lng) {
	int fd = open("log.txt", (O_RDWR | O_CREAT | O_APPEND), 0664); //attempt to find log.
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
	} else { //if file is locked by this program then run ship comparisons
		puts("OK we have the lock\n\n");
		FILE* log = fdopen(fd, "a");
		fprintf(log,
				"proximity indication check was made for point: %.3f lat %.3f lng",
				LAT_MIN_PROX, LAT_MIN_PROX);
		 ship *current = NULL;
		    for (current = ships; current != NULL; current = current->next) {
		    float lat_proximity = current->lat - lat;
			float lng_proximity = current->lng - lng;
			//compare ships to lat + lng
			if (lat_proximity < LAT_MIN_PROX && lng_proximity < LNG_MIN_PROX) {
				printf("ship with mmsi '%d' is close to the given point",
						current->mmsi);
				fprintf(log, "ship with mmsi '%d' is close to the given point",
						current->mmsi);
			}
		}
		fcntl(fd, F_SETLKW, file_lock(F_UNLCK, SEEK_SET));
		fclose(log);
	}
}
/* gets ships by looking for ship files in the working directory*/
int get_ships(ship** ships) {
	int num_ships = 0;
	char dirname[FILENAME_MAX];
	getcwd(dirname, FILENAME_MAX); //get working directory
	DIR *dir;
	dir = opendir(dirname); //open working directory
	struct dirent *file;
	if (dir != NULL) { //dir should not be null as there should be a working directory but safeguard anyway
		while ((file = readdir(dir)) != NULL) { //loop through directory
			if (NULL != (strstr((file->d_name), ".shp"))) { //if ship file
				ship *new_ship = (read_ship(file));
				add_ship(ships, new_ship, &num_ships);
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
	char mmsi[10], name[40], lat[10], lng[10], course[5], speed[5];
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
