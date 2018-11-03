/**
 * A program that takes a symbolic link as an argument and follows it. If the 
 * linked file is also a link, it will read that link and continue in that 
 * fashion until a non-link file is reached. 
 * 
 * Future iterations are intended to include an option for verbose output 
 * listing all intermediate symbolic links between the provided argument and 
 * the ultimately pointed to file. 
 * 
 * @author Franklin D. Worrell 
 * @version 0.1.0
 */ 

#include <limits.h>
#include <sys/stat.h>
#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

/* 
 * Prototypes
 * TODO: Refactor into .h file
 */ 
char* follow_link(char* filename, char** buffer); 
void usage(void); 
void system_call_failed(const char* call); 
bool is_file_symlink(const char* filename); 

int main(int argc, char *argv[]) { 
	char *filename; 
	char *p_readlink_buffer; 		// Buffer for storing result of system call. 
	bool isOutputVerbose = false; 

	// An option was entered, so parse the input. 
	if (argc == 3) {
		// Parse the option. 
		if (strcmp(argv[1], "-t") == 0) {
			isOutputVerbose = true; 
		} else {
			printf("Unrecognized option entered.\n"); 
			usage(); 
			exit(1); 
		} 

		// Set filename to argv[2]. 
		filename = argv[2]; 
	} 

	// No option entered, just capture the file name. 
	else if (argc == 2) {
		// Set filename to argv[1]. 
		filename = argv[1]; 
	} 

	// Otherwise, incorrect number of command-line arguments provided. 
	else {
		printf("Incorrect number of command-line arguments provided.\n"); 
		usage(); 
		exit(1); 
	} 

	if ((p_readlink_buffer = malloc(PATH_MAX)) == NULL) {
		system_call_failed("malloc"); 
	} 

	if (isOutputVerbose) {
		printf("%s\n", filename); 
	} 

	while (is_file_symlink(filename)) {
		// Update filename. 
		filename = follow_link(filename, &p_readlink_buffer); 
		// Print the verbose output if requested. 
		if (isOutputVerbose) {
			
		} 
	} 

	printf("%s\n", filename); 
	free(p_readlink_buffer); 
	exit(0); 
} 

/**
 * 
 * 
 * @param filename 
 * @param isOutputVerbose 
 */ 
char* follow_link(char* filename, char** buffer) {
	if (readlink(filename, *buffer, PATH_MAX - 1) < 0) {
		printf("Error resolving target of symlink: %s\n", filename); 
		exit(1); 
	} else {
		return *buffer; 
	}
} 

/**
 * Prints usage information to the console for users in the event of misuse. 
 */ 
void usage(void) {
	printf("Usage: rrlink [option] <filename>\n"); 
	printf("Available options: \n"); 
	printf("	-t	display verbose output in tree-like format\n"); 
} 

/**
 * Prints the error message produced by the most recent failed system call. 
 *
 * @param call the name of the system call that failed
 */ 
void system_call_failed(const char* call) {
	perror(call); 
} 

/**
 * 
 * 
 * @param filename 
 * @return whether the file whose name is provided is a symbolic link 
 */ 
bool is_file_symlink(const char* filename) {
	struct stat p_lstat_buffer; 
	
	if (lstat(filename, &p_lstat_buffer) < 0) {
		system_call_failed("lstat"); 
		exit(1); 
	} 
	
	return S_ISLNK(p_lstat_buffer.st_mode); 
} 
