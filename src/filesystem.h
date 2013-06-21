#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define FILE_NAME_SIZE				32
#define DEPENDENCIES_SIZE			53
#define FILE_TABLE_SIZE				1024
#define PATH_SIZE					32
#define TRUE						1
#define FALSE 						0
#define bool						int
#define FILE_SIZE 					212
#define INITIAL_DEPENDENCIES_AMOUNT	10
#define FILE_SYS_SIZE				FILE_NAME_SIZE*sizeof(char) + sizeof(bool) + 4*sizeof(int)
#define FILE_STRUCT_SIZE			FILE_NAME_SIZE*sizeof(char) + 3*sizeof(int) + FILE_SIZE
#define SERIALIZED_FILE_SIZE		FILE_NAME_SIZE*sizeof(char) + 2*sizeof(int) + FILE_SIZE + sizeof(bool)
#define SERIALIZED_DIRECTORY_SIZE	FILE_NAME_SIZE*sizeof(char) + DEPENDENCIES_SIZE*sizeof(int) + sizeof(int) + sizeof(bool)
#define SECTOR_STEP					8192	// 4 MB
#define FREE 						'0'
#define OCCUPIED					'1'

typedef struct file_sys {
	char file_name[FILE_NAME_SIZE];
	bool is_file;
	struct file_sys *father_directory;
	int sector;
	struct file_sys **dependencies;
	struct file *content;
}file_sys_t;

typedef struct file {
	char file_name[FILE_NAME_SIZE];
	int sector;
	int next_sector;
	char content[FILE_SIZE];
	struct file *next;
}file_t;

typedef struct serialized_file {
	char name[FILE_NAME_SIZE];
	int sector;
	bool is_file;
	char content[FILE_SIZE];
	int next_sector;
}serialized_file_t;

typedef struct serialized_directory {
	char name[FILE_NAME_SIZE];
	int sector;
	bool is_file;
	int dependencies[DEPENDENCIES_SIZE];
	int next_sector;
}serialized_directory_t;

void setNewDependency(file_sys_t *dependency);
void ls(void);
void initializeFileSystem(void);
file_sys_t* mkdir(file_sys_t *father_directory, char file_name[]);
void cd(char path[]);
int getFreeDiscSector(void);
file_sys_t* getFatherDirectory(void);
void rm(char fileName[]);
bool fileExists(char fileName[]);

#endif