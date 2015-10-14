#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Rules:
Phrase := Reverse | Preserve
Subphrase := Phrase | Word
Reverse := '<' Subphrase Subphrase '>'
Preserve := '[' Subphrase Subphrase ']'
Word := Any group of characters delimited by a space

To tackle this, we will use a recursive descent algorithm
which will step through, check for the rules and recursively
apply them until it has reached the end of the string
*/

//skip all spaces
void skipSpace(char** in)
{
	//increment location pointer while we see a space
	while(**in == ' ')
	{
		(*in)++;
	}
}

//takes formatted input, places the result in output
//in is formatted input, out is resulting output,
int yoda(char* in, char** out)
{
	static char* cin = NULL; //current input
	static char* loc = NULL; //location pointer
	static char* cout = NULL; //current output
	static int level = -1; //for debugging

	level++;
	if(in == NULL && cin == NULL) //if both inputs are null, just return
	{
		level--;
		return -1; //no input given
	}

	//new input given
	if(in != NULL)
	{
		cin = loc = in;
		if(cout != NULL) free(cout);
		cout = (char*)calloc(strlen(cin), sizeof(char));
	}

	if(*loc == 0)
	{
		level--;
		return -1; //no more tokens left
	}

	skipSpace(&loc); //skip spaces
	
	if(*loc == '[')
	{
		//Rule: [ Subphrase Subphrase ]
		//printf("%d: [\n", level);
		loc++;
		if(yoda(NULL, out) == -1) goto FAIL; //first subphrase
		if(yoda(NULL, out) == -1) goto FAIL; //second subphrase
		skipSpace(&loc);
		if(*loc != ']') goto FAIL; // ] character
		loc++;
		//printf("%d: ]\n", level);
	}
	else if(*loc == '<')
	{
		//Rule: < Subphrase Subphrase >
		//printf("%d: <\n", level);
		loc++;

		//save the static pointers (except location) as we are starting anew temporarily
		char* t1 = cin;
		char* t2 = cout;
		char* temp;
		cin = loc;
		cout = NULL;

		//grab the first subphrase and save it into temp
		if(yoda(cin, &temp) == -1) goto FAIL;
		cin = t1;
		cout = t2;

		//grab the second subphrase (this one will be written to cout)
		if(yoda(NULL, out) == -1) goto FAIL;
		
		//write temp to cout. this completes the swap
		if(*cout != 0) strcat(cout, " ");
		strcat(cout, temp);
		free(temp);
		
		skipSpace(&loc);
		if(*loc != '>') goto FAIL; // > character
		loc++;
		//printf("%d: >\n", level);
	}
	else
	{
		//Rule: Word
		char* temp = strpbrk(loc, " []<>\0"); //delimiters
		if(*cout != 0) strcat(cout, " "); //write to cout
		strncat(cout, loc, temp-loc);
		//printf("%d: \"%s\"\n", level, cout);
		loc += temp-loc + 1; //advance location pointer
	}

	*out = cout;
	level--;
	return 0; //finish

FAIL:
	//if we fail, clear memory
	printf("Fail: %d: \"%s\"\n", level, loc);
	if(cout != NULL) free(cout);
	cout = NULL;
	*out = NULL;
	level--;
	return -1;
}

void usage(char* cmd)
{
	printf("Usage: %s <Formatted Phrase>\n", cmd);
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		usage(argv[0]);
		return 0;
	}

	char* out;
	printf("In: %s\n", argv[1]);
	yoda(argv[1], &out);
	if(out != NULL)
	{
		printf("Out: %s\n", out);
	}
	else
	{
		printf("Failed\n");
	}

	return 0;
}

