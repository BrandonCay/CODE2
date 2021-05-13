#include <iostream>
#include <stdio.h>
#include <string>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iomanip>
//using namespace std;

enum  tokenType {eof=-1,
	AND = 0, BEGIN = 1, END = 2, FOR = 3, IF = 4, NOT = 5, OR = 6, READ = 7, WHILE = 8, WRITE = 9,
	COMMENT = 10, ID = 11, REAL = 12, STRING = 13, PLUS = 14, MULTIPLICATION = 15, ASSIGNMENT = 16, EQUAL = 17,
	GREATERTHAN = 18, LESSTHAN = 19, LEFTP = 20, COMMA = 21, RIGHTP = 22, SEMICOLON = 23,
	INVALID = 24, DIVISION = 25, INTEGER = 26
};

typedef tokenType(*checkFunctions) ();



void clearBuffer(void);
void displayToken(tokenType code);
void skipSpacesWrap(bool input);
void skipSpacesAuto();
//Use only if a text file with the inputs is used.
void skipSpaces();

tokenType checkFirst(tokenType, char, tokenType=INVALID);//helper function
tokenType getId(void);
tokenType getComment(void);
tokenType getReal(void);
tokenType getStrings(void);
tokenType getPlus(void);
tokenType getMul(void);
tokenType getAssign(void);
tokenType getEqual(void);
tokenType getGreater(void);
tokenType getLess(void);
tokenType getLP(void);
tokenType getRP(void);
tokenType getComma(void);
tokenType getSColon(void);
tokenType getCommentDivision(void);
tokenType getRealInt(void);

//PART 3: Basic Functions
tokenType getNextToken(void);
tokenType peekToken(void);
tokenType readToken(void);
void match(tokenType token);
void syntaxError(tokenType token);

//PART 3: non-terminal functions
void Program(void);
void StatementList(void);
void StatementTail(void);
void Statement(void);
void Expression(void);
void ExpressionTail(void);
void Factor(void);
void FactorTail(void);
void Primary(void);
void Parameter(void);
void ExpressionList(void);
void ExpressionListTail(void);
void IDlist(void);
void IDTail(void);

checkFunctions funcList[12] = {
	getPlus,
	getMul,
	getEqual,
	getGreater,
	getLess,
	getLP,
	getRP,
	getComma,
	getSColon,
	getAssign,
	getCommentDivision,
	getRealInt
};

std::unordered_map<char, checkFunctions>scannerMap{
	{'A', getId},
	{'1',getRealInt},
	{'/',getCommentDivision},
	{'"',getStrings},
	{'+',getPlus},
	{'*',getMul},
	{':',getAssign},
	{'=',getEqual},
	{'>',getGreater},
	{'<',getLess},
	{'(',getLP},
	{',',getComma},
	{')',getRP},
	{';',getSColon}
};

//PART2
tokenType lexical_error(void);
tokenType scanner(void);

const int BUFFER_SIZE = 80;
char tokenBuffer[BUFFER_SIZE];
std::ifstream inputFile("inputFile.txt");
std::istream& cin = inputFile;
bool needToken = true;



bool skipType = false;
int main() {
	/*
	tokenType res;
	clearBuffer();
	std::cout << std::endl << "PART 3: Display" << std::endl;
	for (int i=0; i < 27; ++i) {
		tokenType token = static_cast<tokenType>(i);
		skipSpacesWrap(skipType);
		cin.getline(tokenBuffer, BUFFER_SIZE);
		displayToken(token);
		clearBuffer();
	}
	std::cout << std::endl;
	
	inputFile.close();
	
	//getID
	std::cout <<std::endl<< "PART 4: ID" << std::endl;
	inputFile.open("getIDinput.txt");
	for (int i = 0; i < 8; ++i) {
		skipSpacesWrap(skipType);
		tokenType res = getId();
		displayToken(res);
		clearBuffer();
	}

	inputFile.close();
	//comments: par8
	std::cout << std::endl << "PART 8: Comments" << std::endl;
	
	inputFile.open("commentInput.txt");

	skipSpacesWrap(skipType);
	res=getComment();
	displayToken(res);
	clearBuffer();
	skipSpacesWrap(skipType);
	res=getComment();
	displayToken(res);
	
	clearBuffer();
	inputFile.close();
	//real inputs
	
	std::cout << std::endl << "PART 9: Real" << std::endl;

	inputFile.open("realinput.txt");

	skipSpacesWrap(skipType);
	res = getReal();
	displayToken(res);
	clearBuffer();
	skipSpacesWrap(skipType);
	res = getReal();
	displayToken(res);
	
	clearBuffer();
	inputFile.close();
	
	
	std::cout << std::endl << "PART 12: String" <<  std::endl;
	
	inputFile.open("stringInput.txt");
	skipSpacesWrap(skipType);
	res = getStrings();
	displayToken(res);
	clearBuffer();
	skipSpacesWrap(skipType);
	res = getStrings();
	displayToken(res);
	clearBuffer();
	inputFile.close();

	//other functions
	std::cout << "OTHER FUNCTION LIST" << std::endl;
	inputFile.open("otherFuncIn.txt");
	for (int i = 0; i < 12; ++i) {
		skipSpacesWrap(skipType);
		//std::cout << char(cin.peek()) << std::endl;//debug
		res = funcList[i]();
		displayToken(res);
		clearBuffer();
		if (i == 9) {//skips the semi-colon after matching :=
			skipSpacesWrap(skipType);
			//std::cout<<char(cin.peek())<<std::endl;//debug
			cin.ignore(1);
			//std::cout << char(cin.peek()) << std::endl;//debug
		}
	}
	inputFile.close();
	inputFile.open("part2.txt");
	std::cout << std::endl << "PART 2" << std::endl<< std::endl;
	std::cout <<std::left<< std::setw(50)<< "Token Types" << "WORD/LEXEMS";
	tokenType code = scanner();
	while (code != eof) {
		displayToken(code);
		clearBuffer();
		code=scanner();
		//std::cout << "LOOP" << std::endl;//debug
	}
	*/

	inputFile.close();
	inputFile.open("part3.txt");
	Program();
	std::cout << std::endl << "PROGRAM 1 END"<<std::endl<<"============" << std::endl;
	inputFile.close();
	inputFile.open("part3Invalid.txt");
	Program();
	std::cout << std::endl << "PROGRAM 2 END" << std::endl << "============" << std::endl;
	inputFile.close();
	inputFile.open("part3Invalid2.txt");
	Program();
	std::cout << std::endl << "PROGRAM 3 END" << std::endl << "============" << std::endl;

	return 0;
}

void skipSpacesWrap(bool input) {
	if (input) {
		skipSpaces();

	}
	else {
		skipSpacesAuto();
	}
	
}
void skipSpacesAuto(){
	int   ch;
	ch = cin.get();
	while (isspace(ch))
		ch = cin.get();
	//std::cout << "CHARACTER: "<<ch << std::endl;//debug
	cin.putback(ch);
	//std::cout << "P: "<<cin.peek() << std::endl;//debug

}
void  skipSpaces()//skip spaces is the input + space filter
{//modified
		int   ch;
		ch = std::cin.get();
			while (isspace(ch))
				ch = std::cin.get();
			//std::cout << "CHARACTER: "<<ch << std::endl;//debug
			std::cin.putback(ch);
		

}

void clearBuffer(void) {
	for (int i = 0; i < BUFFER_SIZE; ++i) {
		tokenBuffer[i] = '\0';
	}
}

void displayToken(tokenType code) {
	static char message[][20] = 
	{
		"and", "begin", "end", "for", "if", "not", "or", "read", "while", "write"
		,"comment", "identifier",  "real constant",  "string", "plus", "multiplication", 
		"assignment", "equal", "greater than", "less than", "left parenthesis", "comma", 
		"right parenthesis", "semicolon", "invalid", "division", "integer" 
	};
	std::string msg(message[code]), buffer(tokenBuffer);
	
	std::cout << std::left << std::setw(50) << msg;
	std::cout << buffer << std::endl;
//	int msgSize=sizeof(message[code])
	//for()
	//cout << message << endl;tokenType  

}


tokenType  getId()
{
	static char reservedWord[][10] = { "AND", "BEGIN", "END", "FOR", "IF", "NOT",
									"OR", "READ", "WHILE", "WRITE" };
	tokenType code;
	int   ch, i = 0;
	ch = cin.get();
	if (isalpha(ch))
	{
		tokenBuffer[i++] = ch;
		ch = cin.get();
		while (isalnum(ch))
		{
			tokenBuffer[i++] = ch;
			ch = cin.get();
		}
		cin.putback(ch);

		//find out if it is a reserved word using the binary search
		int first = 0, mid, last = 9;
		bool  notFound = true;
		while (first <= last && notFound)
		{
			mid = (first + last) / 2;
			int  answer = strcmp(tokenBuffer, reservedWord[mid]);
			if (answer == 0)
				notFound = false;
			else if (answer > 0)
				first = mid + 1;
			else
				last = mid - 1;
		}
		if (notFound)
			code = ID;
		else
			code = (tokenType)mid;
	}
	else    // it is not a letter
	{
		cin.putback(ch);
		code = INVALID;
	}
	std::cout <<std::endl<< "GETID FROM READ: " << std::string(tokenBuffer) << std::endl;
	return(code);
}


tokenType getComment(void) {//myCode
	char ch = cin.get(), prevCh;
	int i = 0;
	if (ch != '/') {
		cin.putback(ch);
		return INVALID;
	}
	tokenBuffer[i] = ch;
	ch = cin.get();
	++i;
	if (ch != '*') {
		cin.putback(ch);
		return INVALID;
	}

	tokenBuffer[i] = ch;
	ch = cin.get();
	++i;
	prevCh = ch;
	while (ch != '\n' && cin.peek() != EOF) {
		ch = cin.get();
		if (ch == '/' && prevCh == '*') {
			return COMMENT;
		}
		tokenBuffer[i] = ch;
		++i;
		prevCh = ch;
	}
	cin.putback(ch);
	return INVALID;
}
/*
tokenType  getComment() //professor code. doesn't work. cuts "invalid comment" to "invalid comm"
{
	tokenType code;
	int   ch, i = 0;
	ch = cin.get();
	if (ch == '/' && cin.peek() == '*')
	{
		tokenBuffer[i++] = ch;     // store the first symbol into the buffer

		// read the second symbol and store it into the buffer
		ch = cin.get();
		tokenBuffer[i++] = ch;

		ch = cin.get();
		while (!(ch == '*' && cin.peek() == '/') && cin.peek() != EOF)
		{
			tokenBuffer[i++] = ch;     // store the first symbol into the buffer

			// read the second symbol and store it into the buffer
			ch = cin.get();
		}
		if (cin.peek() == EOF)
			code = INVALID;
		else
		{
			tokenBuffer[i++] = ch;     // store the first symbol into the buffer

			// read the second symbol and store it into the buffer
			ch = cin.get();
			tokenBuffer[i++] = ch;
			code = COMMENT;
		}
	}
	else       // it is not "/*"
	{
		cin.putback(ch);
		code = INVALID;
	}
	return(code);
}*/


tokenType  getReal()
{
	tokenType code;
	int   ch, i = 0;
	ch = cin.get();
	if (isdigit(ch))
	{
		tokenBuffer[i++] = ch;
		ch = cin.get();
		while (isdigit(ch))
		{
			tokenBuffer[i++] = ch;
			ch = cin.get();
		}
		if (ch == '.')
		{
			tokenBuffer[i++] = ch;
			ch = cin.get();
			if (isdigit(ch))
			{
				tokenBuffer[i++] = ch;
					ch = cin.get();
					while (isdigit(ch))
					{
						tokenBuffer[i++] = ch;
							ch = cin.get();
					}
				cin.putback(ch);
					code = REAL;
			}
			else
			{

				cin.putback(ch);
				code = INVALID;
			}
		}
		else    // it is not a period
		{
			cin.putback(ch);
			code = INVALID;
		}
	}
	else    // it is not a digit
	{
		cin.putback(ch);
		code = INVALID;
	}
	return(code);
}
tokenType  getStrings()
		{
			tokenType code;
			int   ch, i = 0;
			ch = cin.get();
			if (ch == '\"')
			{
				tokenBuffer[i++] = ch;     // store the first symbol into the buffer

				ch = cin.get();
				while (!(ch == '\"') && ch != EOF)
				{
					tokenBuffer[i++] = ch;     // store the symbol into the buffer

					// read the second symbol and store it into the buffer
					ch = cin.get();
				}
				if (ch == EOF)
					code = INVALID;
				else
				{
					tokenBuffer[i++] = ch;     // store the first symbol into the buffer
					code = STRING;
				}
			}
			else       // it is not “
			{
				cin.putback(ch);
				code = INVALID;
			}
			return(code);
		}



// My Code: Tested 5/12
/*
tokenType getId(void) {
	static char reservedWord[][10] = { "AND", "BEGIN", "END", "FOR", "IF", "NOT", "OR", "READ",
																  "WHILE", "WRITE" };
	//build tokenBuffer
	char ch = cin.get();
	int i = 0;
	while (isalnum(ch)) {
		//std::cout << ch;//debug
		tokenBuffer[i] = ch;
		ch = cin.get();
		++i;
	}
	//std::cout << std::endl;
	cin.putback(ch);
		std::string buffer(tokenBuffer);
	//std::cout << buffer << std::endl;//debug
	for (int i = 0; i < 10; ++i) {
		std::string reserved(reservedWord[i]);
		if (buffer == reserved) {
			return static_cast<tokenType>(i);
		}
	}
	return ID;
}

/*

tokenType getReal(void) {
	int deciCnt = 0;
	bool numFnd = false;
	char ch;

	int i = 0;

	while (cin.peek() != EOF && i<BUFFER_SIZE) {
		//debug: std::cout << "REAL:" << ch << std::endl;
		ch = cin.get();
		if (isdigit(ch)) {
			numFnd = true;
		}
		else if (ch == '.') {
			++deciCnt;
		}
		else {
			cin.putback(ch);
			return INVALID;
		}
		if (deciCnt > 1) {
			cin.putback(ch);
			return INVALID;
		}
		tokenBuffer[i]=ch;
		++i;
		
	}
	if (deciCnt ==1 && numFnd)
		return REAL;
	return INVALID;//incomplete
}

tokenType getStrings(void) {
	if (cin.peek() == EOF) {
		return INVALID;
	}
	char ch = cin.get();
	int i = 0;
	if (ch != '"' || cin.peek() == EOF) {
		cin.putback(ch);
		return INVALID;
	}
	tokenBuffer[i] = ch;
	++i;
	bool endFnd = false;
	while(cin.peek()!=EOF && !endFnd && i<BUFFER_SIZE) {
		ch = cin.get();
		tokenBuffer[i] = ch;
		if (ch == '"') {
			endFnd=true;
		}
		//std::cout << ch << std::endl;//debug
		++i;
	}
	if (endFnd)
		return STRING;
	cin.putback(ch);
	return INVALID;
}*/

tokenType getPlus() {
	return checkFirst(PLUS, '+');
}
tokenType checkFirst(tokenType successVal, char matchCh, tokenType failVal) {
	if (cin.peek() == EOF)
		return INVALID;
	char ch = cin.get();
	tokenBuffer[0] = ch;
	if (ch == matchCh)
		return successVal;
	else {
		cin.putback(ch);
		return failVal;
	}
}

tokenType getMul() {
	return checkFirst(MULTIPLICATION, '*');
}

tokenType getAssign()
{
	tokenType code;
	int   ch, i = 0;
	ch = cin.get();
	if (ch == ':' && cin.peek() == '=')
	{
		tokenBuffer[i++] = ch;     // store the first symbol into the buffer

		// read the second symbol and store it into the buffer
		ch = cin.get();
		tokenBuffer[i] = ch;
		code = ASSIGNMENT;
	}
	else       // it is not an assignment
	{
		cin.putback(ch);
		code = INVALID;
	}
	return(code);
}

/* MyCode: not tested 5/12
tokenType getAssign() {
	char ch = cin.get(), ch2 = cin.get();
	if (ch != ':' || ch2 != '=' ) {
		cin.putback(ch2);
		cin.putback(ch);
		return INVALID;
	}//may need to include any numbers
	tokenBuffer[0] = ch;
	tokenBuffer[1] = ch2;
	return ASSIGNMENT;
}*/
tokenType getEqual() {
	return checkFirst(EQUAL, '=');
}
tokenType getGreater() {
	return checkFirst(GREATERTHAN, '>');
}

tokenType getLess() {
	return checkFirst(LESSTHAN, '<');
}

tokenType getLP() {
	return checkFirst(LEFTP, '(');
}

tokenType getRP() {
	return checkFirst(RIGHTP, ')');
}
tokenType getComma() {
	return checkFirst(COMMA, ',');
}
tokenType getSColon() {
	return checkFirst(SEMICOLON, ';');
}

tokenType getCommentDivision() {
	char ch = cin.get();
	if (ch == '/') {
		tokenBuffer[0] = ch;
	}
	else {
		return INVALID;
	}
	if (cin.peek() == '*') {
		return	getComment();
	}
	return DIVISION;
}

tokenType getRealInt() {
	tokenType code;
	int   ch, i = 0;
	ch = cin.get();
	if (isdigit(ch))
	{
		tokenBuffer[i++] = ch;
		ch = cin.get();
		while (isdigit(ch))
		{
			tokenBuffer[i++] = ch;
			ch = cin.get();
		}
		if (ch != '.') {
			cin.putback(ch);
			return INTEGER;
		}else
		{
			tokenBuffer[i++] = ch;
			ch = cin.get();
			if (isdigit(ch))
			{
				tokenBuffer[i++] = ch;
				ch = cin.get();
				while (isdigit(ch))
				{
					tokenBuffer[i++] = ch;
					ch = cin.get();
				}
				cin.putback(ch);
				code = REAL;
			}
			else
			{

				cin.putback(ch);
				code = INVALID;
			}
		}
	}
	else    // it is not a digit
	{
		cin.putback(ch);
		code = INVALID;
	}
	return(code);
}
/*
tokenType getRealInt() {
	char ch = cin.get();
	if (!isdigit(ch)) {
		cin.putback(ch);
		return INVALID;
	}
	int i = 1;
	tokenBuffer[0] = ch;
	while (cin.peek()!=EOF) {
		ch = cin.get();
		if (isdigit(ch))
			tokenBuffer[i] = ch;
		else
			break;
		++i;
	}
	if (ch == '.') {//GETTING REAL
		bool intFnd = false;
		tokenBuffer[i] = ch;
		if (cin.peek() == EOF)
			return INVALID;
		ch = cin.get();
		while (cin.peek() != EOF && isdigit(ch) && i < BUFFER_SIZE) {
			++i;
			tokenBuffer[i] = ch;
			ch = cin.get();
			intFnd = true;
		}
		if (intFnd)
			return REAL;
		else
			return INVALID;
	}
	return INTEGER;

}
*/
tokenType lexical_error() {
	char ch = cin.get();
	tokenBuffer[0] = ch;
	return INVALID;
}

tokenType scanner() {
	skipSpacesWrap(skipType);
	int character = cin.get();
	if (character == EOF) {
		std::cout << "EOF" << std::endl;//debug
		return eof;
	}
	else
	{
		cin.putback(character);
		if (isalpha(cin.peek())) {
			character = 'A';
		}
		else if (isdigit(cin.peek())) {
			character = '1';
		}
		else {
			character = cin.peek();
		}
		if (scannerMap.find(character) != scannerMap.end()) {
			tokenType code= scannerMap[character]();
			std::string s(tokenBuffer);
			std::cout <<"FOUND: "<<s  << " CODE:" <<code<<std::endl;//debug
			return code;
		}
		else {
			std::string s(tokenBuffer);
			std::cout << "INVALID: " << s << std::endl;//debug
			return lexical_error();
		}
	}

}

tokenType getNextToken(void) {
	static tokenType nextToken;
	if (needToken) {
		nextToken=scanner();
		needToken = false;
	}
	return nextToken;//no otherwise specified
}

tokenType peekToken(void) {
	return getNextToken();
}

tokenType readToken(void) {
	tokenType code=getNextToken();
	needToken = true;
	return code;
}

void match(tokenType token) {
	tokenType code = readToken();
	std::string buffer(tokenBuffer);
	if (token != code) {
		std::cout << "ERROR AT BUFFER: " << buffer << std::endl;
		syntaxError(code);
		std::cout <<"current token | expected code " << std::endl<< code<<" | "<< token <<std::endl;
	}
	else {
		std::cout << std::endl << "MATCHED: " <<" "<< buffer;
		if(code == SEMICOLON)
			std::cout << std::endl;//skip to the next line? next line of what? There multiplie streams with lines (i.e. output screen, input file)
	}
	std::cout<<std::endl << "----" << std::endl;
	clearBuffer();
}
void syntaxError(tokenType token){
	std::cout << "ERROR: Missing token at token:  " << token <<std::endl;
}

void Program(void) {
	match(BEGIN);
	StatementList();
	match(END);
}

void StatementList(void) {
	Statement();
	StatementTail();
}
void StatementTail(void) {
	tokenType nextToken = peekToken();
	switch (nextToken) {
	//statement cases	
	case ID:
	case WRITE:
	case READ:
		std::cout << "STATEMENT MATCH IN TAIL:" << std::endl;//debug
		Statement();
		StatementTail();
		break;
	case END:
		return;
	default:
		std::cout << "STATEMENT_TAIL ERR:" << std::endl;//debug
		syntaxError(nextToken);
	}
}

void Statement(void) {
	tokenType nextToken = peekToken();
	switch (nextToken) {
		case ID:
			std::cout << "ID STATEMENT: " << nextToken << std::endl;//debug
			match(ID);
			match(ASSIGNMENT);
			Expression();
			match(SEMICOLON);
			break;
		case WRITE:
			std::cout << "WRITE STATEMENT: " << std::endl;//debug
			match(WRITE);
			match(LEFTP);
			Parameter();
			match(RIGHTP);
			match(SEMICOLON);
			break;
		case READ:
			std::cout << "READ STATEMENT: " << std::endl;//debug

			match(READ);
			match(LEFTP);
			IDlist();
			match(RIGHTP);
			match(SEMICOLON);
			break;
		default:
			std::cout << "STATEMENT ERR: " << std::endl;//debug
			syntaxError(nextToken);
	}
}

void Expression(void) {
	std::cout <<std::endl<< "EXPRESSION" << std::endl;
	Factor();
	ExpressionTail();
}

void ExpressionTail(void) {
	tokenType nextToken = peekToken();
	std::cout << "EXP_TAIL: " << nextToken << std::endl;
	switch (nextToken) {
	case PLUS:
		match(PLUS);
		Factor();
		ExpressionTail();
		break;
	case RIGHTP:
	case SEMICOLON:
	case COMMA: //TEMPORARY FIX: When result is passed. The next token is always ','. <expression>-><primary><factorTail> will be chosen
	// and is will always false. A potential solution is somehow never read the the ID in the first place. OR hold the syntax error message and wait to see
	//see if it resolves itself up the tree i.e. when it completes expression and returns to <expressionListTail> to match ',' <expression><expression>.
	//other expressions should fail and the error should printed
		return;
	default:
		syntaxError(nextToken);
	}
}
void Factor(void) {
	std::cout << std::endl << "FACTOR "<< std::endl;
	Primary();
	FactorTail();
}

void FactorTail(void) {
	tokenType nextToken = peekToken();
	std::cout << std::endl << "FACTOR TAIL: "<<nextToken << std::endl;
	switch (nextToken) {
	case MULTIPLICATION:
		match(MULTIPLICATION);
		Primary();
		FactorTail();
		break;
	case DIVISION:
		match(DIVISION);
		Primary();
		FactorTail();
		break;
	case PLUS:
	case SEMICOLON:
	case RIGHTP:
	case COMMA://TEMPORARY FIX: see explaination at expressionTail
		return;
	default:
		syntaxError(nextToken);
	}
}

void Primary(void) {
	tokenType nextToken = peekToken();
	std::cout << std::endl << "PRIMARY: "<<nextToken << std::endl;
	switch (nextToken) {
	case LEFTP:
		match(LEFTP);
		Expression();
		match(RIGHTP);
		break;
	case ID:
		match(ID);
		break;
	case REAL:
		match(REAL);
		break;
	case INTEGER:
		match(INTEGER);
		break;
	default:
		syntaxError(nextToken);
	}
}

void Parameter(void) {
	tokenType nextToken = peekToken();
	std::cout << std::endl << "PARAMETER: "<< nextToken << std::endl;//debug

	switch (nextToken) {
	case STRING:
		match(STRING);
		std::cout << std::endl <<"AFTER MATCH STR: "<< std::string(tokenBuffer) << std::endl;//debug
		break;
	case ID:
	case REAL:
	case INTEGER:
		std::cout << std::endl << "AFTER MATCH EXR: " << std::string(tokenBuffer) << std::endl;//debug

		ExpressionList();
		break;
	default:
		std::cout << std::endl << "AFTER MATCH ERR: " << std::string(tokenBuffer) << std::endl;//debug
		syntaxError(nextToken);
	}
}


void ExpressionList(void) {
	Expression();
	ExpressionListTail();
}

void ExpressionListTail(void) {
	tokenType nextToken = peekToken();
	std::cout << std::endl << "EXP_LIST_TAIl TOK: " << nextToken << std::endl;
	switch (nextToken) {
	case COMMA:
		match(COMMA);
		Expression();
		ExpressionListTail();
		break;
	case RIGHTP:
		return;
	default:
		syntaxError(nextToken);
	}
}

void IDlist(void) {
	std::cout << std::endl << "IDlist" << std::endl;
	match(ID);
	IDTail();
}


void IDTail(void) {
	tokenType nextToken = peekToken();
	std::cout << "IDtail: " << nextToken << std::endl;
	switch (nextToken) {
	case COMMA:
		match(COMMA);
		match(ID);
		IDTail();
		break;
	case RIGHTP:
		return;
	default:
		syntaxError(nextToken);
	}
}
