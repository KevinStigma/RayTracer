#ifndef CPARSERV1_H
#define CPARSERV1_H

#define PARSER_DEBUG_OFF // enables/disables conditional compilation 

#define PARSER_STRIP_EMPTY_LINES        1   // strips all blank lines
#define PARSER_LEAVE_EMPTY_LINES        2   // leaves empty lines
#define PARSER_STRIP_WS_ENDS            4   // strips ws space at ends of line
#define PARSER_LEAVE_WS_ENDS            8   // leaves it
#define PARSER_STRIP_COMMENTS           16  // strips comments out
#define PARSER_LEAVE_COMMENTS           32  // leaves comments in

#define PARSER_BUFFER_SIZE              256 // size of parser line buffer
#define PARSER_MAX_COMMENT              16  // maximum size of comment delimeter string

#define PARSER_DEFAULT_COMMENT          "#"  // default comment string for parser

// pattern language
#define PATTERN_TOKEN_FLOAT   'f'
#define PATTERN_TOKEN_INT     'i'
#define PATTERN_TOKEN_STRING  's'
#define PATTERN_TOKEN_LITERAL '\''

// state machine defines for pattern matching
#define PATTERN_STATE_INIT       0

#define PATTERN_STATE_RESTART    1
#define PATTERN_STATE_FLOAT      2
#define PATTERN_STATE_INT        3 
#define PATTERN_STATE_LITERAL    4
#define PATTERN_STATE_STRING     5
#define PATTERN_STATE_NEXT       6

#define PATTERN_STATE_MATCH      7
#define PATTERN_STATE_END        8

#define PATTERN_MAX_ARGS         16
#define PATTERN_BUFFER_SIZE      80

#include <stdio.h>
class CPARSERV1
{
public:

	// constructor /////////////////////////////////////////////////
	CPARSERV1();

	// destructor ///////////////////////////////////////////////////
	~CPARSERV1() ;

	// reset file system ////////////////////////////////////////////
	int Reset();

	// open file /////////////////////////////////////////////////////
	int Open(char *filename);

	// close file ////////////////////////////////////////////////////
	int Close();

	// get line //////////////////////////////////////////////////////
	char *Getline(int mode);

	// sets the comment string ///////////////////////////////////////
	int SetComment(char *string);

	// find pattern in line //////////////////////////////////////////
	int Pattern_Match(char *string, char *pattern, ...);

	// VARIABLE DECLARATIONS /////////////////////////////////////////

public: 

	FILE *fstream;                    // file pointer
	char buffer[PARSER_BUFFER_SIZE];  // line buffer
	int  length;                      // length of current line
	int  num_lines;                   // number of lines processed
	char comment[PARSER_MAX_COMMENT]; // single line comment string

	// pattern matching parameter storage, easier that variable arguments
	// anything matched will be stored here on exit from the call to pattern()
	char  pstrings[PATTERN_MAX_ARGS][PATTERN_BUFFER_SIZE]; // any strings
	int   num_pstrings;

	float pfloats[PATTERN_MAX_ARGS];                       // any floats
	int   num_pfloats;

	int   pints[PATTERN_MAX_ARGS];                         // any ints
	int   num_pints;

}; // end CLASS CPARSERV1 //////////////////////////////////////////////

typedef CPARSERV1 *CPARSERV1_PTR;


#endif