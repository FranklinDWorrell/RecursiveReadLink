#! /bin/bash 

################################################################################
# Given a symbolic link as an argument, reads the link and continues reading   #
# any links pointed to by that link until the file ultimately being pointed to #
# is reached. Prints that file to the console.                                 #
#                                                                              #
# Includes an option `-t` that prints verbose output in a tree-like fashion.   #
################################################################################

# TODO: Include an option that prints the link sequence in `tree` fashion. 
VERBOSE=false

# Parse input to separate option and filename 
if [ $# -eq 2 ]; then 
	FILE=$2 
	OPTION=$1 
	# Verify flag is recognized. 
	if [[ "$OPTION" != "-t" ]]; then 
		echo "Option not recognized."
		echo "Usage: rrlink [-option] <filename>" 
		exit 1
	else 
		VERBOSE=true
	fi 
elif [ $# -eq 1 ]; then 
	FILE=$1 
else 
	echo "Usage: rrlink [-option] <filename>" 
	exit 1 
fi 

# Only output the provided file for verbose, formatted output. 
if [ $VERBOSE = true ] && [ -L $FILE ]; then 
	echo "$FILE" 
fi 

# Used for formatting verbose, tree-like output. 
NUMBER_OF_SPACES=0

while [ -L $FILE ]; do 
	FILE=`readlink $FILE` 
	# Output verbose, formatting output if option enabled. 
	if [ $VERBOSE = true ]; then 
		NUMBER_OF_SPACES=$(( NUMBER_OF_SPACES + 2 ))
		printf -v LEADING_SPACES '%*s' $NUMBER_OF_SPACES " "
		echo "$LEADING_SPACES|-> $FILE" 
	fi 
done 

echo $FILE 
