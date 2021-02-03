#include <iostream>
#include <stdint.h>
#include <fstream>
using namespace std;

char inputFilename[] = "TESTLOGS.TXT";
char outputFilename[] = "ReadableLogs.txt";

struct Data
{
  char currentMillis;		//UINT8_T
  float ax, ay, az;
};
Data readData;

int convert(char* inputFilename, char* outputFilename)
{
	int errorStatus = 0;
	
	fstream inputFile;
	inputFile.open(inputFilename, ios::in | ios::binary);
	if (inputFile.is_open())
	{
		fstream outputFile(outputFilename);
		if (outputFile.is_open())
		{
			Data readTemporaryData;
						
			while (!inputFile.eof())
			{
				inputFile.read((char *)&readTemporaryData, sizeof(readTemporaryData));			
			
//				cout << "PeriodMillis = " << int(readTemporaryData.currentMillis) << endl;
//				cout << "AX = " << readTemporaryData.ax << endl;
//				cout << "AY = " << readTemporaryData.ay << endl;
//				cout << "AZ = " << readTemporaryData.az << endl;
				
				if (readTemporaryData.currentMillis == 0 && readTemporaryData.ax == 0 && readTemporaryData.ay == 0 && readTemporaryData.az == 0)
				{	//If so, insert the header
					outputFile << "Period, AX, AY, AZ" << endl;
				}
				else	//Log the normal record in human readable form in output file.
				{
					outputFile << int(readTemporaryData.currentMillis) << ", ";
					outputFile << readTemporaryData.ax << ", ";
					outputFile << readTemporaryData.ay << ", ";
					outputFile << readTemporaryData.az << endl;
				}
			}
		}
		else
		{
			cout << "Could not open output file for writing" << endl;
		}
		

	}
	else
	{
		cout << "Could not open input file for reading" << endl;
	}
	
	return errorStatus;

}

int main(void)
{
	convert(inputFilename, outputFilename);
	return 0;
}
