#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

enum ErrorType
{
	UNEXPECTED_CHAR,		//0
	NON_TERMINATED_STRING,	//1
};

enum TokenType
{
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,	//0, 1, 2, 3,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,	//4, 5, 6, 7, 8, 9, 10,

	// One or two character tokens.                  
	BANG, BANG_EQUAL,                                	//11, 12,
	EQUAL, EQUAL_EQUAL,                              	//13, 14,
	GREATER, GREATER_EQUAL,                          	//15, 16,
	LESS, LESS_EQUAL,                                	//17, 18,
	
	// Literals.
	IDENTIFIER, STRING, NUMBER,							//19, 20, 21,

	// Keywords.                                     
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,  	//22, 23, 24, 25, 26, 27, 28, 29,30,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,		//31, 32, 33, 34, 35, 36, 37,

	//EOF
};

class Token
{
public:
	char* lexemeString = NULL;
	int lexemeLength = 0;
	int lexemeLine;
	TokenType type;

	Token* next = NULL;
	Token* previous = NULL;

	Token(char* lexemeString, int lexemeLength, int lexemeLineNumber, TokenType type)
	{
		this->lexemeLength = lexemeLength;
		this->lexemeLine = lexemeLineNumber;
		this->type = type;
		
		this->lexemeString = new char[this->lexemeLength];
		for (int i = 0; i < this->lexemeLength; i++)
		{
			this->lexemeString[i] = lexemeString[i];
		}
	}
	
	Token(const Token &token2)	//Copy constructor
	{
		lexemeLine = token2.lexemeLine;
		lexemeLength = token2.lexemeLength;
		type = token2.type;
		
		lexemeString = new char[lexemeLength];
		for (int i = 0; i < lexemeLength; i++)
		{
			lexemeString[i] = token2.lexemeString[i];
		}
	}
	
	void operator = (Token const & token2)	//Deep copy
	{
		delete[] lexemeString;
		lexemeString = NULL;
		
		lexemeLine = token2.lexemeLine;
		lexemeLength = token2.lexemeLength;
		type = token2.type;
		
		lexemeString = new char[lexemeLength];
		for (int i = 0; i < lexemeLength; i++)
		{
			lexemeString[i] = token2.lexemeString[i];
		}
	}
	
	void printToken()
	{
		cout << "Lexeme string:\t\"";
		
		for (int i = 0; i < lexemeLength; i++)
		{
			cout << lexemeString[i];
		}
		
		cout << "\"\n" << "Lexeme length:\t" << lexemeLength << "\n" << "Line number:\t" << lexemeLine << "\nToken type:\t" << type << endl;
	}
	
	~Token()
	{
		delete[] lexemeString;
	}
};

class Error
{
public:
	int lineNumber;
	char* errorMessage = NULL;
	int errorMessageLength = 0;
	
//	Error(int lineNumber, char* errorMessage, int errorMessageLength)
	Error(char* errorMessage, int errorMessageLength, int lineNumber)
	{
		this->lineNumber = lineNumber;
		this->errorMessageLength = errorMessageLength;
		
		this->errorMessage = new char[this->errorMessageLength];

		for (int i = 0; i < this->errorMessageLength; i++)
		{
			this->errorMessage[i] = errorMessage[i];
		}
	}
	
	Error(const Error &error2)	//Copy Constructor
	{
		this->lineNumber = error2.lineNumber;
		this->errorMessageLength = error2.errorMessageLength;
		
		this->errorMessage = new char[this->errorMessageLength];
		
		for (int i = 0; i < this->errorMessageLength; i++)
		{
			this->errorMessage[i] = error2.errorMessage[i];
		}		
	}
	
	void operator = (Error const &error2)	//Deep copy
	{
		delete[] errorMessage;
		errorMessage = NULL;
		
		this->lineNumber = error2.lineNumber;
		this->errorMessageLength = error2.errorMessageLength;
		
		this->errorMessage = new char[this->errorMessageLength];
		
		for (int i = 0; i < this->errorMessageLength; i++)
		{
			this->errorMessage[i] = error2.errorMessage[i];
		}		
	}
	
	void printError()
	{
		cout << "Error at line " << lineNumber << ":\n";
		for (int i = 0; i < errorMessageLength; i++)
		{
			cout << errorMessage[i];
		}
		cout << endl;
	}
	
	void reportError()
	{
		//To be implemennted.
	}
	
	~Error()
	{
		delete[] errorMessage;
	}
};

class Scanner
{
public:

	vector<Token> tokenList;
	int noOfTokens = 0;	//For indexing above.
	
	vector<Error> errorList;
	int noOfErrors = 0;
	bool hasErrors = false;
	
	Scanner()
	{
		noOfTokens = 0;
		noOfErrors = 0;
		hasErrors = false;
	}
	
	void addToken(char* lexemeString, int lexemeStringLength, int lineNumber, TokenType type)	//Adds a new token to token list and increments the counter.
	{
		Token t(lexemeString, lexemeStringLength, lineNumber, type);
		tokenList.push_back(t);
		noOfTokens++;
	}
	
	void addError(ErrorType type, char c, int lineNumber)
	{
		char* errorMessage = NULL;
		int errorMessageLength = 0;
		
		if (type == UNEXPECTED_CHAR)
		{
			errorMessageLength = 22 + 1 + 1;	// = 24
			char* errorMessage = new char[errorMessageLength];

			errorMessage[0] = 'U';
			errorMessage[1] = 'n';
			errorMessage[2] = 'e';
			errorMessage[3] = 'x';
			errorMessage[4] = 'p';
			errorMessage[5] = 'e';
			errorMessage[6] = 'c';
			errorMessage[7] = 't';
			errorMessage[8] = 'e';
			errorMessage[9] = 'd';
			errorMessage[10] = ' ';
			errorMessage[11] = 'c';
			errorMessage[12] = 'h';
			errorMessage[13] = 'a';
			errorMessage[14] = 'r';
			errorMessage[15] = 'a';
			errorMessage[16] = 'c';
			errorMessage[17] = 't';
			errorMessage[18] = 'e';
			errorMessage[19] = 'r';
			errorMessage[20] = ' ';
			errorMessage[21] = '\'';
			errorMessage[22] = 'c';
			errorMessage[23] = '\'';
			
			errorMessage[22] = c;

			Error e(errorMessage, errorMessageLength, lineNumber);	//Error(int lineNumber, char* errorMessage, int errorMessageLength)
			errorList.push_back(e);
			noOfErrors++;			
		}
		
		delete[] errorMessage;
	}
	
	void addError(ErrorType type, int lineNumber)
	{
		char* errorMessage = NULL;
		int errorMessageLength = 0;
		
		if (type == NON_TERMINATED_STRING)
		{
			errorMessageLength = 21;
			errorMessage = new char[errorMessageLength];
			
			errorMessage[0] = 'N';
			errorMessage[1] = 'o';
			errorMessage[2] = 'n';
			errorMessage[3] = ' ';
			errorMessage[4] = 't';
			errorMessage[5] = 'e';
			errorMessage[6] = 'r';
			errorMessage[7] = 'm';
			errorMessage[8] = 'i';
			errorMessage[9] = 'n';
			errorMessage[10] = 'a';
			errorMessage[11] = 't';
			errorMessage[12] = 'e';
			errorMessage[13] = 'd';
			errorMessage[14] = ' ';
			errorMessage[15] = 's';
			errorMessage[16] = 't';
			errorMessage[17] = 'r';
			errorMessage[18] = 'i';
			errorMessage[19] = 'n';
			errorMessage[20] = 'g';
			
			Error e(errorMessage, errorMessageLength, lineNumber);
			errorList.push_back(e);
			noOfErrors++;			
		}
		
		delete[] errorMessage;
	}

	
	char getNextChar(int currentPosition, char* sourceString, int sourceStringSize)	//Returns NULL '\0' if there is no next character.
	{
		if (currentPosition + 1 < sourceStringSize)
		{
			return sourceString[currentPosition + 1];
		}
		else
		{
			return '\0';
		}
	}
	
	bool isDigit(char c)
	{
		if (c >= '0' && c <= '9')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool isAlpha(char c)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool areCharStringsSame(char* str1, char* str2, int strLength)
	{
		bool areSame = true;
		
		for (int i = 0; i < strLength; i++)
		{
			if (str1[i] != str2[i])
			{
				areSame = false;
				break;
			}
		}
		
		return areSame;
	}
	
	void scanTokens(char* sourceString, int sourceStringSize)	
	{
		int currentPosition = 0;
		int stringBeginningIndex = 0;
		int stringBeginningLine = -1;
		char currentChar = sourceString[currentPosition];
		char nextChar;
		int currentLine = 1;
		
		while (currentPosition < sourceStringSize)
		{
			currentChar = sourceString[currentPosition];
			
			switch(currentChar)
			{
				case '(':
					addToken("(", 1, currentLine, LEFT_PAREN);
					break;
    			
				case ')':
					addToken(")", 1, currentLine, RIGHT_PAREN);
					break;
				
				case '{':
					addToken("{", 1, currentLine, LEFT_BRACE);
					break;
				
				case '}':
					addToken("}", 1, currentLine, RIGHT_BRACE);
					break;
				
				case ',':
					addToken(",", 1, currentLine, COMMA);
					break;
				
				case '.':
					addToken(".", 1, currentLine, DOT);
					break;
				
				case '+':
					addToken("+", 1, currentLine, PLUS);
					break;
				
				case '-':
					addToken("-", 1, currentLine, MINUS);
					break;
				
				case '*':
					addToken("*", 1, currentLine, STAR);
					break;
					
				case ';':
					addToken(";", 1, currentLine, SEMICOLON);
					break;
					
				case ' ':	//space (whitespace)
					break;
					
				case '\n':
					currentLine++;
					break;

				case '\r':
					break;

				case '\t':
					break;
				
				case '!':
				{
//					nextChar = sourceString[currentPosition + 1];
					nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
					if (nextChar == '=')
					{
						addToken("!=", 2, currentLine, BANG_EQUAL);
						currentPosition = currentPosition + 1;		//Because this next char also has been consumed by the token.
					}
					else
					{
						addToken("!", 1, currentLine, BANG);
					}
					
					break;
				}
			
				case '=':
				{
					nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
					if (nextChar == '=')
					{
						addToken("==", 2, currentLine, EQUAL_EQUAL);
						currentPosition++;
					}
					else
					{
						addToken("=", 1, currentLine, EQUAL);
					}
					
					break;
				}
				
				case '<':
				{
					nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
					if (nextChar == '=')
					{
						addToken("<=", 2, currentLine, LESS_EQUAL);
						currentPosition++;
					}
					else
					{
						addToken("<", 1, currentLine, LESS);
					}
					
					break;
				}

				case '>':
				{
					nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
					if (nextChar == '=')
					{
						addToken(">=", 2, currentLine, GREATER_EQUAL);
						currentPosition++;
					}
					else
					{
						addToken(">", 1, currentLine, GREATER);
					}
					
					break;
				}

				case '/':
				{
					nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
					if (nextChar == '/')	//It is a comment.
					{
						while (nextChar != '\n' && nextChar != '\0')	//\0 means end of source string
						{
							currentPosition++;
							nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
						}
					}
					else	//It is NOT a comment.
					{
						addToken("/", 1, currentLine, SLASH);
					}
					
					break;
				}

				case '\"':
					
					nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
					
					if (nextChar != '\0')
					{
						stringBeginningIndex = currentPosition + 1;	//Because the char at current position is " at the moment.
						stringBeginningLine = currentLine;
						bool hasTheStringEnded = false;
					
						
						while (nextChar != '\0')
						{
							currentChar = sourceString[currentPosition];
							
							if (currentChar == '\n')
							{
								currentLine++;
							}
							
							if (nextChar == '\"')	//The string may have ended or may contain a " within it.
							{	
								if (currentChar == '\\')	//The string contains a " within it.
								{
									currentPosition++;
									nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
								}
								else	//The string has ended.
								{
									hasTheStringEnded = true;
									
									int stringLength = currentPosition - stringBeginningIndex + 1;
									char* str = new char[stringLength];
									
									for (int i = 0; i < stringLength; i++)
									{
										str[i] = sourceString[i + stringBeginningIndex];
									}
									
									addToken(str, stringLength, stringBeginningLine, STRING);
									
									delete[] str;
									
									currentPosition++;	//So the string ending " is not read again after switch case statements.
									break;	//Break the while loop.
								}
							}
							else	//The string has not ended.
							{
								currentPosition++;
								nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
							}
						}
						
						if (!hasTheStringEnded)
						{
							addError(NON_TERMINATED_STRING, stringBeginningLine);	//Error: non terminated string
							hasErrors = true;
						}

					}
					else
					{
						addError(NON_TERMINATED_STRING, currentLine);	//Error: The file has ended immediately after the beginning qoutation mark.
						hasErrors = true;
					}
					
					break;
					
				default:
					
					if (isDigit(currentChar))
					{
						bool hasFractionalPart = false;
						
						nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
						stringBeginningIndex = currentPosition;
						stringBeginningLine = currentLine;
						
						while (isDigit(nextChar))
						{
							currentChar = nextChar;
							currentPosition++;
							nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
						}
						
						if (nextChar == '.' && isDigit(sourceString[currentPosition + 2]))	//Decimal detected
						{
							currentChar = nextChar;
							currentPosition++;
							nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);							
							
							while (isDigit(nextChar))
							{
								currentChar = nextChar;
								currentPosition++;
								nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
							}								
						}
						
						int strLength = currentPosition - stringBeginningIndex + 1;
						char* str = new char[strLength];
						
						for (int i = 0; i < strLength; i++)
						{
							str[i] = sourceString[i + stringBeginningIndex];
						}
						
						addToken(str, strLength, stringBeginningLine, NUMBER);
						
						delete[] str;
					}
					else if (isAlpha(currentChar))
					{
						stringBeginningIndex = currentPosition;
						stringBeginningLine = currentLine;
						
						nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
						currentPosition++;
																			
						while (nextChar != '\0' && (isDigit(nextChar) || isAlpha(nextChar)))
						{
							currentChar = nextChar;
							nextChar = getNextChar(currentPosition, sourceString, sourceStringSize);
							currentPosition++;
						}
						
						int strLength = currentPosition - stringBeginningIndex;
						char* str = new char[strLength];
						
						for (int i = 0; i < strLength; i++)
						{
							str[i] = sourceString[i + stringBeginningIndex];
						}
						
						//Here						//POTENTIAL TROUBLE: areCharStringsSame needs to be modified.
						
						if (areCharStringsSame(str, "and", strLength))
						{
							addToken(str, strLength, stringBeginningLine, AND);
						}
						
						else if (areCharStringsSame(str, "class", strLength))
						{
							addToken(str, strLength, stringBeginningLine, CLASS);
						}
						
						else if (areCharStringsSame(str, "else", strLength))
						{
							addToken(str, strLength, stringBeginningLine, ELSE);
						}
						
						else if (areCharStringsSame(str, "false", strLength))
						{
							addToken(str, strLength, stringBeginningLine, FALSE);
						}
						
						else if (areCharStringsSame(str, "fun", strLength))
						{
							addToken(str, strLength, stringBeginningLine, FUN);
						}
						
						else if (areCharStringsSame(str, "for", strLength))
						{
							addToken(str, strLength, stringBeginningLine, FOR);
						}
						
						else if (areCharStringsSame(str, "if", strLength))
						{
							addToken(str, strLength, stringBeginningLine, IF);
						}
						
						else if (areCharStringsSame(str, "nil", strLength))
						{
							addToken(str, strLength, stringBeginningLine, NIL);
						}
						
						else if (areCharStringsSame(str, "or", strLength))
						{
							addToken(str, strLength, stringBeginningLine, OR);
						}

						else if (areCharStringsSame(str, "print", strLength))
						{
							addToken(str, strLength, stringBeginningLine, PRINT);
						}

						else if (areCharStringsSame(str, "return", strLength))
						{
							addToken(str, strLength, stringBeginningLine, RETURN);
						}

						else if (areCharStringsSame(str, "super", strLength))
						{
							addToken(str, strLength, stringBeginningLine, SUPER);
						}

						else if (areCharStringsSame(str, "this", strLength))
						{
							addToken(str, strLength, stringBeginningLine, THIS);
						}

						else if (areCharStringsSame(str, "true", strLength))
						{
							addToken(str, strLength, stringBeginningLine, TRUE);
						}

						else if (areCharStringsSame(str, "var", strLength))
						{
							addToken(str, strLength, stringBeginningLine, VAR);
						}

						else if (areCharStringsSame(str, "while", strLength))
						{
							addToken(str, strLength, stringBeginningLine, WHILE);
						}
						
						else
						{
							addToken(str, strLength, stringBeginningLine, IDENTIFIER);						
						}
						
					}
					else	
					{
						addError(UNEXPECTED_CHAR, currentChar, currentLine);
						hasErrors = true;
					}

					break;
			}
			currentPosition++;
		}
		
		if (true == hasErrors)
		{
			cout << "Total errors found = " << noOfErrors << endl << endl;
			
			for (int i = 0; i < noOfErrors; i++)
			{
				errorList[i].printError();
				cout << endl;
			}
		}
	}
	
	void printTokens()
	{
		for (int i = 0; i < noOfTokens; i++)
		{
			tokenList[i].printToken();
			cout << "\n";
		}
		cout << endl;
	}

};

class Lox
{
public:
	
	char* sourceString = NULL;
	int sourceStringSize = 0;
	bool* hasError = false;
	
	int getTheNumberOfCharactersInFile(char* filename)	//Working
	{
		int noOfCharacters = 0;
		ifstream myFile(filename);
		char c;
		
		if (myFile.is_open())
		{
			while(!myFile.eof())
			{
				myFile.get(c);
				noOfCharacters++;
			}
			
			myFile.close();
		}
		else
		{
			cout << "Error opening file in function getTheNumberOfCharactersInFile(char* filename)\nFilename: " << filename << endl;
		}
		
		noOfCharacters--;	//Because off by 1 for some reason.	//Maybe it is EOF? Not sure.
		return noOfCharacters;
	}
	
	int readFile(char* filename)		//This file is Jlox code's source file.
	{
		//First find the no of characters in source file, then new the sourceString, and then read the file.
		
		sourceStringSize = getTheNumberOfCharactersInFile(filename);
		sourceString = new char[sourceStringSize];
		
		ifstream myFile(filename);
		if (myFile.is_open())
		{
			for (int i = 0; i < sourceStringSize; i++)
			{
				myFile.get(sourceString[i]);
			}
			
			myFile.close(); 
		}
		else
		{
			cout << "Error opening file in function readFile(char* filename)\nFilename: " << filename << endl;
			return 0;
		}
		
		return 1;
	}
	
	int printSourceString()
	{
		for (int i = 0; i < sourceStringSize; i++)
		{
			cout << sourceString[i];
		}
		
		return sourceStringSize;
	}
	
	~Lox()
	{
		delete[] sourceString;
	}
	
};

int main(void)
{
	Lox l;
	l.readFile("jlox_code.txt");
	cout << l.getTheNumberOfCharactersInFile("jlox_code.txt") << endl;
	l.printSourceString();
	
	cout << endl << "=====" << endl;

	Scanner s;
	s.scanTokens(l.sourceString, l.sourceStringSize);
	s.printTokens();
	
	return 0;
}
