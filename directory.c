#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "directory.h"

#define TRUE 1
#define FALSE 0
#define MAX_SIZE 2048

struct entryNode {
	char * name;
	struct entryNode * next;	/* sibling */
	int isDirectory;
	struct entryNode * parent;
	union {
		char * contents;	/* for a text file */
		struct entryNode * entryList;	/* for a directory */
	} entry;
};

struct entryNode * root;

/* Helper functions */
void pwdHelper (struct entryNode *);
struct entryNode * located (char *, struct entryNode *);
struct entryNode * locatePrevious (char *, struct entryNode *); 
void addEntry (struct entryNode *entryList, struct entryNode *newEntry);

/* Return an initialized file system (an empty directory named "/") 
   after storing it in the root variable. */
struct entryNode * initialFileSystem ( ) {
	/* YOU SUPPLY THE BODY FOR THIS FUNCTION; THE FOLLOWING STATEMENTS
	   ARE PROVIDED TO AVOID COMPILE ERRORS IN THE FRAMEWORK. */
	root = malloc(sizeof(struct entryNode));
	if (root == NULL){
		printf("Unable to Initialize\n");
		free(root);
	}
	char rootname = '/';
	root->name = strdup(&rootname);
	root->next = NULL;
	root->isDirectory = TRUE;
	root->parent = root;
	return root;
	
}

/* implements the "create" command (one argument; not in standard UNIX) */
/* wd is the current working directory */
/* fileName is the name of the text file to create */
void createFile (struct entryNode * wd, char * fileName) {
	struct entryNode * newFile;
	if (located (fileName, wd->entry.entryList)) {
		printf ("create: %s: File exists\n", fileName);
	} else {
		newFile = malloc(sizeof(struct entryNode));
		if (newFile == NULL){
			printf("Unable to create new File\n");
			free(newFile);
		}

		//set members of new directory
		newFile->name = strdup(fileName);
		newFile->next = NULL;
		newFile->isDirectory = FALSE;
		newFile->parent = wd;

		// add code to enter file contents from user input
		char line[MAX_SIZE];
		newFile->entry.contents = NULL;

	 // MAXLINESIZE + 1 leaves room for the null byte added by fgets().
		newFile->entry.contents = malloc(MAX_SIZE + 1);
		if (newFile->entry.contents == NULL) {
			// out of space
			exit(1);
		}

		printf("enter file contents: \n");
    	while(fgets(line, MAX_SIZE + 1, stdin) != NULL && strcmp(line, "\n") != 0){
     	//process line   
        	strcat(newFile->entry.contents, line);
    	}

		//add file to entry list
		addEntry(wd, newFile);	
	}
}

/* implements the "mkdir" command (one argument; no options) */
/* wd is the current working directory */
/* dirName is the name of the directory to create */
void createDir (struct entryNode * wd, char * dirName) {
	struct entryNode * newDir;
	if (located (dirName, wd->entry.entryList)) {
		printf ("mkdir: %s: File exists\n", dirName);
	} else {
		newDir = malloc(sizeof(struct entryNode));
		if (newDir == NULL){
			printf("Unable to create new Directory\n");
			free(newDir);
		}

		//set members of new directory
		newDir->name = strdup(dirName); 
		newDir->next = NULL;
		newDir->isDirectory = TRUE;		
		newDir->parent = wd;			
		newDir->entry.contents = NULL;

		//add new directory to working directory
		addEntry(wd, newDir);
	}
}

/* implements the "rm" command (one argument, unlike standard UNIX; no options) */
/* wd is the current working directory */
/* fileName is the name of the text file to delete */
void removeFile (struct entryNode * wd, char * fileName) {
	struct entryNode * file;
	file = located (fileName, wd->entry.entryList);
	if (file == NULL) {
		printf ("rm: %s: No such file or directory.\n", fileName);
	} else if (file->isDirectory) {
		printf ("rm: %s: is a directory.\n", fileName);
	} else {
		//rm head file
		if(wd->entry.entryList == file ){
			wd->entry.entryList = file->next;
			free(file->entry.contents);
			file->entry.contents = NULL;
			free(file);
			file->next = NULL;
			file->parent = NULL;
		} else {
			//rm any other file
			struct entryNode * filePrevious;
			filePrevious = locatePrevious(fileName, wd->entry.entryList);

			filePrevious->next = file->next;
			free(file->entry.contents);
			file->entry.contents = NULL;
			free(file);
			file->next = NULL;
			file->parent = NULL;
		}
	}
}

/* implements the "rmdir" command (one argument, unlike standard UNIX; no options) */
/* wd is the current working directory */
/* dirName is the name of the directory to delete */
void removeDir (struct entryNode * wd, char * dirName) {
	struct entryNode * dir;
	dir = located (dirName, wd->entry.entryList);
	if (dir == NULL) {
		printf ("rmdir: %s: No such file or directory.\n", dirName);
	} else if (!dir->isDirectory) {
		printf ("rmdir: %s: Not a directory.\n", dirName);
	} else if (dir->entry.entryList != NULL) {
		printf ("rmdir: %s: Directory not empty\n", dirName);
	} else {
		if(wd->entry.entryList == dir ){
			wd->entry.entryList = dir->next;
			free(dir->entry.entryList);
			dir->entry.entryList = NULL;
			free(dir);
			dir->next = NULL;
			dir->parent = NULL;
		} else {
			//rm any other file
			struct entryNode * dirPrevious;
			dirPrevious = locatePrevious(dirName, wd->entry.entryList);

			dirPrevious->next = dir->next;
			free(dir->entry.entryList);
			dir->entry.entryList = NULL;
			free(dir);
			dir->next = NULL;
			dir->parent = NULL;
		}
	}
}

/* implements the "cp" command (two named arguments, no options) */
/* wd is the current working directory */
/* "from" must name an existing file or directory.
   It's copied--a deep copy is made in the case of a directory--and the
   copy is given the "to" argument as its name.
   It's an error if "to" names an existing entry in the working directory.
 */
void copyFile (struct entryNode * wd, char * from, char * to) {
	struct entryNode * file;
	file = located (from, wd->entry.entryList);
	if (file == NULL) {
		printf ("cp: %s: No such file or directory.\n", from);
	} else if (located(to, wd->entry.entryList)){
		printf ("cp: %s: File exists\n", to);
	} else {
		

		
	}
}

/* implements the "mv" command (two named arguments, no options) */
/* wd is the current working directory */
/* "from" must name an existing file or directory.
   If "to" doesn't name an existing file or directory, the "from" entry is renamed.
   If "to" names an existing directory, that directory can't already contain 
   an entry named "from"; the "from" entry is removed from the working directory
   and added to the "to" directory.
   It's an error if "to" names an existing file.
 */
void moveFile (struct entryNode * wd, char * from, char * to) {
	struct entryNode * file;
	struct entryNode * copyFile;
	file = located (from, wd->entry.entryList);
	if (file == NULL) {
		printf ("mv: %s: No such file or directory.\n", from);
	} else {
		copyFile = located(to, wd->entry.entryList);
		if(copyFile == NULL){
			printf("moving file\n");
			copyFile = malloc(sizeof(struct entryNode));
			if(copyFile == NULL){
				printf("no memory\n");
				exit(1);
			}
			copyFile->name = strdup(to);
			copyFile->entry.contents = strdup(file->entry.contents);
			copyFile->parent = wd;
			copyFile->next = NULL;
			copyFile->isDirectory = FALSE;
			removeFile(wd, from);
			addEntry(wd, copyFile);

		} else if (copyFile->isDirectory == FALSE){
			//rm 'from' file, rename it, replace it in wd
			printf ("mv: %s: File exists.\n", to);
		} else {
			printf("moving into directory\n");
			copyFile = located(to, )
		}
		
	}
}

/* implements the "cd" command (one argument, which may be ".." or "/"; no options) */
/* wd is the current working directory */
struct entryNode * newWorkingDir (struct entryNode * wd, char * dirName) {
	struct entryNode * newWd;
	if (strcmp (dirName, "/") == 0) {
		return root;
	} else if (strcmp (dirName, "..") == 0) {
		return wd->parent;
	} else {
		newWd = located (dirName, wd->entry.entryList);
		if (newWd == NULL) {
			printf ("cd: %s: No such file or directory.\n", dirName);
			return wd;
		} else if (!newWd->isDirectory) {
			printf ("cd: %s: Not a directory.\n", dirName);
			return wd;
		} else {
			return newWd;
		}
	}
}

/* implements the "pwd" command (no arguments; no options) */
/* wd is the current working directory */
void printWorkingDir (struct entryNode * wd) {
	if (strcmp (wd->name, "/") == 0) {
		printf ("/\n");
	} else {
		pwdHelper (wd);
		printf ("\n");
	}
}

void pwdHelper (struct entryNode * wd) {
	if (strcmp (wd->name, "/") != 0) {
		pwdHelper (wd->parent);
		printf ("/%s", wd->name);
	}
}

/* implements the "ls" command (0 or 1 argument, unlike standard UNIX; no options) */
/* wd is the current working directory */
/* Behavior is as follows:
	if no arguments, list the names of the files in wd;
	if one argument that names a text file in wd, echo the argument;
	if one argument that names a directory d in wd, list the names of the files in d;
	if one argument that names nothing in wd, print
		ls: ___: No such file or directory
 */
void listWorkingDir (struct entryNode * wd) {
	struct entryNode * p  = wd->entry.entryList;
	while (p != NULL) {
		printf ("%s\n", p->name);
		p = p->next;
	}
}

void listWithinWorkingDir (struct entryNode * wd, char * name) {
	struct entryNode * entryPtr;
	entryPtr = located (name, wd->entry.entryList);
	if (entryPtr == NULL) {
		printf ("ls: %s: No such file or directory\n", name);
	} else if (entryPtr->isDirectory) {
		listWorkingDir (entryPtr);
	} else {
		printf ("%s\n", name);
	}
}

/* implements the "cat" command (arbitrary number of arguments, which all must
   name text files; no options) */
/* wd is the current working directory */
/* This function prints the contents of a single file with the given name. */
void listFileContents (struct entryNode * wd, char * name) {
	struct entryNode * entryPtr;
	entryPtr = located (name, wd->entry.entryList);
	if (entryPtr == NULL) {
		printf ("cat: %s: No such file or directory\n", name);
	} else if (entryPtr->isDirectory) {
		printf ("cat: %s: Operation not permitted\n", name);
	} else {
		printf ("%s", entryPtr->entry.contents);
	}
}

/* Return a pointer to the entry with the given name in the given list,
   or NULL if no such entry exists. */
struct entryNode * located (char * name, struct entryNode * list) {
	if (list == NULL) {
		return NULL;
	} else if (strcmp (list->name, name) == 0) {
		return list;
	} else {
		return located (name, list->next);
	}
}
//return file of previous file
struct entryNode * locatePrevious (char * name, struct entryNode * list) {
	if (list == NULL) {
		return NULL;
	} else if (strcmp (list->next->name, name) == 0) {
		return list;
	} else {
		return locatePrevious (name, list->next);
	}
}
void addEntry (struct entryNode *wd, struct entryNode *newEntry){
	int check; //string  return value check
	if(wd->entry.entryList == NULL){
		//entry list is empty
		wd->entry.entryList = newEntry;
	} else if((check = strcmp(newEntry->name, wd->entry.entryList->name) < 0)){
		//enter at head
		newEntry->next = wd->entry.entryList;
		wd->entry.entryList = newEntry;
	} else {
		//enter anywhere else
		struct entryNode *currEntry;
		struct entryNode *prevEntry;
		struct entryNode *nextEntry;
		currEntry = wd->entry.entryList;
		//stop traversing at end of the list or when new name is less than next name
		while(currEntry != NULL && (check = strcmp(newEntry->name, currEntry->name) > 0)){
			prevEntry = currEntry;
			currEntry = currEntry->next;
		}
		nextEntry = prevEntry->next;
		prevEntry->next = newEntry;
		newEntry->next = nextEntry;
	}	
}