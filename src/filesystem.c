#include "filesystem.h"
#include "../include/disk.h"
#include <stdio.h>
#include <string.h>

char *sector_table;
file_sys_t *root;
file_sys_t *current_directory;

int
main(void) {
	int i;
	initializeFileSystem();
	printf("current_directory: %s\nsector: %d\n", current_directory->file_name, current_directory->sector);
	file_sys_t *aux = mkdir(current_directory, "caca");
	setNewDependency(aux);
	aux = mkdir(current_directory, "caca");
	ls();
	rm("caca");
	cd("caca");
	return 0;
}

void
setNewDependency(file_sys_t *dependency) {
	file_sys_t **aux = current_directory->dependencies;
	while(*aux != NULL) {
		aux += sizeof(file_sys_t*);
	}
	if (*aux == NULL) {
		*aux = dependency;
		return;
	}
}

void
ls(void) {
	file_sys_t **aux = current_directory->dependencies;
	int i;
	printf("\n\nList of directories and files in %s\n", current_directory->file_name);
	while(*aux != NULL && strlen((*aux)->file_name) > 0) {
		if ((*aux)->is_file) {
			printf("\tfile: ");
		} else {
			printf("\tdirectory: ");
		}
		printf("%s\n", (*aux)->file_name);
		aux += sizeof(file_sys_t*);
	}
	return;
}

void
initializeFileSystem(void) {
	int i;

	setTableSize();

	for (i = 0; i < FILE_TABLE_SIZE; i++) {
		sector_table[i] = FREE;
	}
	root = mkdir(root, "root");
	current_directory = root;
}

void
setTableSize(void) {
	char *send = "test";
	long sectors = 0;
	while(getErrorRegister(ATA0) != ERROR) { // ERROR ESTA MAL!!!
		ata_write(ATA0, send, 4, sector, 0);
		sector += SECTOR_STEP;
	}
	sector_table = malloc(sectors*sizeof(char));
	
}

bool
fileExists(char fileName[]) {
	if (current_directory == NULL) {
		return FALSE;
	} else if (current_directory->dependencies == NULL) {
			return FALSE;
	} else {
		file_sys_t **aux = current_directory->dependencies;
		while(*aux != NULL && strlen((*aux)->file_name) > 0) {
			if (strcmp((*aux)->file_name, fileName) == 0) {
				printf("ERROR: File already exists\n");
				return TRUE;
			}
			aux += sizeof(file_sys_t*);
		}
		return FALSE;
	}
}

file_sys_t*
mkdir(file_sys_t *father_directory, char file_name[]) {
	if (fileExists(file_name)) {
		return NULL;
	} else {
		file_sys_t *ans = (file_sys_t *)malloc(sizeof(file_sys_t));
		file_sys_t **aux;

		strcpy(ans->file_name, file_name);
		ans->is_file = FALSE;
		ans->father_directory = father_directory;
		ans->sector = getFreeDiscSector();
		ans->dependencies = malloc(INITIAL_DEPENDENCIES_AMOUNT*sizeof(file_sys_t*));
		aux = ans->dependencies;
		return ans;
	}
}

void
cd(char path[]) {
	if (strcmp(path, "..") == 0) {
		current_directory = getFatherDirectory();
	} else if (strcmp(path, "~") == 0) {
		current_directory = root;
	} else {
		file_sys_t **aux = current_directory->dependencies;
		while(*aux != NULL && strlen((*aux)->file_name) > 0) {
			if (strcmp((*aux)->file_name, path) == 0) {
				current_directory = *aux;
				printf("changed to %s directory\n", (*aux)->file_name);
				return;
			}
			aux += sizeof(file_sys_t*);
		}
		printf("No such directory\n");
	}
}

int
getFreeDiscSector(void) {
	int i;

	for (i = 0; i < FILE_TABLE_SIZE; i++) {
		if (sector_table[i] == FREE) {
			sector_table[i] = OCCUPIED;
			return i;
		}
	}
	printf("ERROR\n");
}

file_sys_t*
getFatherDirectory(void) {
	if (current_directory->father_directory != NULL) {
		return current_directory->father_directory;
	} else {
		return current_directory;
	}
}

void
rm(char fileName[]) {
	file_sys_t **aux = current_directory->dependencies;
	while(*aux != NULL && strlen((*aux)->file_name) > 0) {
		if (strcmp((*aux)->file_name, fileName) == 0) {
			sector_table[(*aux)->sector] = -1;
			rm()
			removeDependencies(current_directory->dependencies);
			printf("rm: File %s successfully removed\n", fileName);
			return;
		}
		aux += sizeof(file_sys_t*);
	}
	printf("rm: File doesn't exist\n");
}

void
removeDependencies(file_sys_t **dependencies) {
	file_sys_t *currDep;
	while(*dependencies != NULL) {
		removeDependencies(*dependencies->dependencies);
		currDep = *dependencies;
		free(*currDep);
		dependencies += 
	}
	free(dependencies);
}
