/*************************************************************************/
/**	WordlistGenerator is a program that dynamically generates 			**/
/**		wordlists based on certain ASCII character sets that are 		**/
/**		chosen by the user.												**/
/**																		**/
/**	@author dot_Cipher													**/
/**	Copyright under the GNU Public License 2011							**/
/*************************************************************************/

// VERSION: 1.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

char *initNumSet(){
	int i, j;
	char *set = (char *)malloc(10);
	j=0;
	if(set==NULL){
		fprintf(stderr, "OUT OF MEMORY\n");
		exit(EXIT_FAILURE);
	}
	for(i=48; i<=57; i++){
		set[j] = (char)i;
		j++;
	}
	return set;
}

char *initUpperSet(){
	int i, j;
	char *set = (char *)malloc(26);
	j=0;
	if(set==NULL){
		fprintf(stderr, "OUT OF MEMORY\n");
		exit(EXIT_FAILURE);
	}
	for(i=65; i<=90; i++){
		set[j] = (char)i;
		j++;
	}
	return set;
}

char *initLowerSet(){
	int i, j;
	char *set = (char *)malloc(26);
	j=0;
	if(set==NULL){
		fprintf(stderr, "OUT OF MEMORY\n");
		exit(EXIT_FAILURE);
	}
	for(i=97; i<=122; i++){
		set[j] = (char)i;
		j++;
	}
	return set;
}

char *initSpecialSet(){
	int i, j;
	char *set = (char *)malloc(32);
	j=0;
	if(set==NULL){
		fprintf(stderr, "OUT OF MEMORY\n");
		exit(EXIT_FAILURE);
	}
	for(i=33; i<=47; i++){
		set[j] = (char)i;
		j++;
	}
	for(i=58; i<=64; i++){
		set[j] = (char)i;
		j++;
	}
	for(i=91; i<=96; i++){
		set[j] = (char)i;
		j++;
	}
	for(i=123; i<=126; i++){
		set[j] = (char)i;
	}
	return set;
}

int main(int argc, char *argv[]){
	FILE *output;
	FILE *log;
	char *numSet;		// 0123456789
	char *upperSet;		// ABCDEFGHIJKLMNOPQRSTUVWXYZ
	char *lowerSet;		// abcdefghijklmnopqrstuvwxyz
	char *specialSet;	// !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
	char *charSet;
	char *password;
	char *endword;
	char *path;
	int min, max, i, j, k, numPass, saveBool, loadBool;
	int shift[255];
	saveBool = loadBool = 0;
	i = j = k = numPass = 0;
	min = max = 6;
	charSet = NULL;

	// CHECK CONSOLE INPUT:
	// and dynamically allocate space for char sets
	for(i=1; i<argc; i++){
		if(strncmp(argv[i], "-min", 2) == 0){
			min = atoi(argv[i+i]);
		}
		if(strncmp(argv[i], "-max", 2) == 0){
			max = atoi(argv[i+i]);
		}
		if(strncmp(argv[i], "-a", 2) == 0){
			lowerSet = initLowerSet();
			if(charSet == NULL){
				charSet = (char *)malloc(sizeof(lowerSet));
			}
			else{
				charSet = (char *)realloc(charSet, strlen(charSet)+strlen(lowerSet));
			}
			strncat(charSet, lowerSet, strlen(lowerSet));
		}
		if(strncmp(argv[i], "-A", 2) == 0){
			upperSet = initUpperSet();
			if(charSet == NULL){
				charSet = (char *)malloc(sizeof(upperSet));
			}
			else{
				charSet = (char *)realloc(charSet, strlen(charSet)+strlen(upperSet));
			}
			strncat(charSet, upperSet, strlen(upperSet));
		}
		if(strncmp(argv[i], "-n", 2) == 0){
			numSet = initNumSet();
			if(charSet == NULL){
				charSet = (char *)malloc(sizeof(numSet));
			}
			else{
				charSet = (char *)realloc(charSet, strlen(charSet)+strlen(numSet));
			}
			strncat(charSet, numSet, strlen(numSet));
		}
		if(strncmp(argv[i], "-u", 2) == 0){
			specialSet = initSpecialSet();
			if(charSet == NULL){
				charSet = (char *)malloc(sizeof(specialSet));
			}
			else{
				charSet = (char *)realloc(charSet, strlen(charSet)+strlen(specialSet));
			}
			strncat(charSet, specialSet, strlen(specialSet));
		}
		if(strncmp(argv[i], "-s", 2) == 0){
			if(charSet == NULL){
				charSet = (char *)malloc(sizeof(" "));
			}
			else{
				charSet = (char *)realloc(charSet, strlen(charSet)+strlen(" "));
			}
			strncat(charSet, " ", strlen(" "));
		}
		if(strncmp(argv[i], "-c", 2) == 0){
			if(charSet == NULL){
				charSet = (char *)malloc(sizeof(argv[i+1]));
			}
			else{
				charSet = (char *)realloc(charSet, strlen(charSet)+strlen(argv[i+1]));
			}
			strncat(charSet, argv[i+1], strlen(argv[i+1]));
		}
		if(strncmp(argv[i], "-o", 2) == 0){
			path = (char *)malloc(sizeof(argv[i+1]));
			path = argv[i+1];
			output = fopen(path, "w+");
			saveBool = 1;
		}
		if(strncmp(argv[i], "-l", 2) == 0){
			log = fopen("wordlistgen.log", "w+");
			fscanf(log, "%s", charSet);
			fscanf(log, "%s", password);
			fscanf(log, "%d", min);
			fscanf(log, "%d", max);
			fclose(log);
			loadBool = 1;
		}
		if (strncmp(argv[i],"-h",2)==0){
			printf("%s \n","Wordlist Generator v1.0 by dot_Cipher");
			printf("%s \n","Usage: wordlistgen [options] -o [path]");
			printf("%s \n","Options:");
			printf("%s \n","   -c [string]	: use custom [string] as character set");
			printf("%s \n","   -min			: minimum string output length [default is 6]");
			printf("%s \n","   -max			: maximum string output length");
			printf("%s \n","   -a			: use lower-case letters (a-z) [default]");
			printf("%s \n","   -A			: use upper-case letters (A-Z)");
			printf("%s \n","   -n			: use decimal numbers (0-9)");
			printf("%s \n","   -u			: use unique characters (!#$%& ...)");
			printf("%s \n","   -s			: use the space character ( )");
			printf("%s \n","   -l			: load and continue from previous output");
			printf("%s \n","   -h			: show help screen (you are looking at it)");
			exit(0);
		}
	}
	// Lock max if invalid input given
	if(max<min) max=min;
	if(charSet == NULL){
		charSet="abcdefghijklmnopqrstuvwxyz";
		i=26;
	}
	else{
		i=strlen(charSet);
	}
	/*
	i=strlen(charSet);
	if(i<1){
		charSet="abcdefghijklmnopqrstuvwxyz";
		i=26;
	}
	*/
	i=i-1;
	for(j=0; j<=i; j++){
		shift[charSet[j]]=j;
	}
	min=min-1;
	max=max-1;
	for(j=min; j<=max; j++){
		if(loadBool==0){
			password = NULL;
			for(k=0;k<=j;k++){
				if(password == NULL){
					password = (char *)malloc(sizeof(charSet[0]));
				}
				else{
					password = (char *)realloc(password, strlen(password)+1);
				}
				strncat(password, (char *)charSet[0], 1);
			}
		}
		loadBool=0;
		endword=NULL;
		for(k=0; k<=j; k++){
			if(endword == NULL){
				endword = (char *)malloc(sizeof(charSet[i]));
			}
			else{
				endword = (char *)realloc(endword, strlen(endword)+strlen((char *)charSet[i]));
			}
			strncat(endword, (char *)charSet[i], strlen((char *)charSet[i]));
		}
		while(password!=endword){
			numPass++;
			for(k=0; k<=j; k++){
				if(password[j-k] == (charSet[i+1])){
					password[j-k] = charSet[0];
					password[j-1-k] = charSet[shift[password[j-1-k]]+1];
				}
			}
			printf("%s \n", password);
			if(numPass%10000==1){
				log = fopen("wordlistgen.log", "w+");
				fprintf(log, "%s\n", charSet);
				fprintf(log, "%s\n", password);
				fprintf(log, "%d\n", j+1);
				fprintf(log, "%d\n", max+1);
				fclose(log);
			}
			if(saveBool==1) fprintf(output, "%s\n", password);
			password[j] = charSet[shift[password[j]]+1];
		}
	}
	if(saveBool==1) fclose(output);
	return 0;
}