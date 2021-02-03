#include <iostream>
#include <fstream>
#include <stdint.h>

#define limit 0.002

#define UPPER_X_THRESHOLD limit
#define LOWER_X_THRESHOLD -limit
#define UPPER_Y_THRESHOLD limit
#define LOWER_Y_THRESHOLD -limit
#define UPPER_Z_THRESHOLD limit
#define LOWER_Z_THRESHOLD -limit

using namespace std;

class ThreeDDataInstance
{
public:
	int32_t currentMilliseconds;
	float x, y, z;

	ThreeDDataInstance()
	{
	}

	ThreeDDataInstance(int32_t cMs, float xx, float yy, float zz)
	{
		currentMilliseconds = cMs;
		x = xx;
		y = yy;
		z = zz;
	}
	
	void setParams(int32_t cMs, float xx, float yy, float zz)
	{
		currentMilliseconds = cMs;
		x = xx;
		y = yy;
		z = zz;		
	}
	
	void print()
	{
		cout << currentMilliseconds << "\t\t" << x << "\t\t" << y << "\t\t" << z << "\n";
	}
};

class AccelData
{
public:
	int noOfRecords = 0;	//Should be n-2 i-e ignore the first and the last record IN file
	ThreeDDataInstance* Records = NULL;
	
	AccelData()
	{
	}
	
	AccelData(int noOfRecs)
	{
		noOfRecords = noOfRecs;	
		Records = new ThreeDDataInstance[noOfRecords];
	}
	
	int populateRecords(char* filename)	//Creates a new file named AccelDataNonCSV.txt
	{
		fstream myFile(filename);
		if (myFile)
		{
			fstream tempFile;
			tempFile.open("AccelDataNonCSV.txt", ios::out);
			if (tempFile)
			{
				char c;
				while (!myFile.eof())
				{
					myFile.get(c);
					if (c == ',')
					{
						c = '\t';
						tempFile << c; 
					}
					else
					{
						tempFile << c;
					}
				}
				
				tempFile.close();
			}
			else
			{
				cout << "Error opening tempFile in populateRecords()\n";
			}
			
			myFile.close();
		}
		else
		{
			cout << "Error opening file in populateRecords()\n";
		}
		
		myFile.open("AccelDataNonCSV.txt");
		if (myFile)
		{
			char c = 0;
			while (c != '\n')
			{
				myFile.get(c);
			}
			for (int i = 0; i < noOfRecords; i++)
			{
				int ms;
				float ax, ay, az;
				myFile >> ms;
				myFile >> ax;
				myFile >> ay;
				myFile >> az;

				Records[i].setParams(ms, ax, ay, az);
			}
			
			myFile.clear();
		}
		else
		{
			cout << "Error opening AccelDataNonCSV.txt as myFile for reading\n";
		}
	}
	
	void print()
	{
		for (int i = 0; i < noOfRecords; i++)
		{
			Records[i].print();
		}
		cout << endl;
	}
	
	~AccelData()
	{
		delete[] Records;
	}
};

class VelocData
{
public:
	int noOfRecords = 0;
	ThreeDDataInstance* Records = NULL;
	
	VelocData()
	{
	}
	
	VelocData(int noOfRecs)
	{
		noOfRecords = noOfRecs;
		Records = new ThreeDDataInstance[noOfRecords];
	}
	
	void print()
	{
		for (int i = 0; i < noOfRecords; i++)
		{
			Records[i].print();
		}
	}

	int printToFile(char* filename)
	{
		fstream myFile(filename, ios::out);
		if (myFile)
		{
			for (int i = 0; i < noOfRecords; i++)
			{
				myFile << Records[i].currentMilliseconds << "\t";
				myFile << Records[i].x << "\t";
				myFile << Records[i].y << "\t";
				myFile << Records[i].z << "\n";
			}
			myFile.close(); 
		}
		else
		{
			cout << "Error opening file in VelocData::printToFile(char* filename)\n";
		}
	}
	
	~VelocData()
	{
		delete[] Records;
	}
};

class PosData
{
public:
	int noOfRecords = 0;
	ThreeDDataInstance* Records = NULL;
	
	PosData()
	{
	}
	
	PosData(int noOfRecs)
	{
		noOfRecords = noOfRecs;
		Records = new ThreeDDataInstance[noOfRecords];
	}
	
	void print()
	{
		for (int i = 0; i < noOfRecords; i++)
		{
			Records[i].print();
		}
	}

	int printToFile(char* filename)
	{
		fstream myFile(filename, ios::out);
		if (myFile)
		{
			for (int i = 0; i < noOfRecords; i++)
			{
				myFile << Records[i].currentMilliseconds << "\t";
				myFile << Records[i].x << "\t";
				myFile << Records[i].y << "\t";
				myFile << Records[i].z << "\n";
			}
			myFile.close(); 
		}
		else
		{
			cout << "Error opening file in PosData::printToFile(char* filename)\n";
		}
	}
	
	~PosData()
	{
		delete[] Records;
	}
};

class Algorithm
{
public:
	AccelData* adr = NULL;
	VelocData* vdr = NULL;
	PosData* pdr = NULL;
	
	int noOfRecords;
	
	Algorithm()
	{
	}
	
	Algorithm(char* filename)
	{
		noOfRecords = Algorithm::getTheNumberOfLinesInFile(filename) - 2;
		adr = new AccelData(noOfRecords);
		vdr = new VelocData(noOfRecords);
		pdr = new PosData(noOfRecords);
	
		adr->populateRecords(filename);
		//adr->print();
		calculateVelocities();
	}
	
	void calculateVelocities()
	{
		float vfx = 0, vfy = 0, vfz = 0;
		vdr->Records[0].setParams(0, 0, 0, 0);
		
		for (int i = 1; i < noOfRecords; i++)
		{
			float time = adr->Records[i].currentMilliseconds - adr->Records[i - 1].currentMilliseconds;	//In milliSeconds			
			time = time / 1000;	//In seconds now
			
			vfx = vdr->Records[i - 1].x + adr->Records[i].x * time;
			vfy = vdr->Records[i - 1].y + adr->Records[i].y * time;
			vfz = vdr->Records[i - 1].z + adr->Records[i].z * time;
			
			vdr->Records[i].setParams(adr->Records[i].currentMilliseconds, vfx, vfy, vfz);
			//vdr->Records[i].print();
		}
	}
	
	void calculateVelocitiesBasedOnSimpleGaitAssumption()
	{
		float vfx = 0, vfy = 0, vfz = 0;
		vdr->Records[0].setParams(0, 0, 0, 0);
		
		for (int i = 1; i < noOfRecords; i++)
		{
			float time = adr->Records[i].currentMilliseconds - adr->Records[i - 1].currentMilliseconds;	//In milliSeconds			
			time = time / 1000;	//In seconds now

			if (adr->Records[i].x < UPPER_X_THRESHOLD && adr->Records[i].x > LOWER_X_THRESHOLD)
			{
				vfx = 0;
			}
			else
			{
				vfx = vdr->Records[i - 1].x + adr->Records[i].x * time;
			}
			
			if (adr->Records[i].y < UPPER_Y_THRESHOLD && adr->Records[i].y > LOWER_Y_THRESHOLD)
			{
				vfy = 0;
			}
			else
			{
				vfy = vdr->Records[i - 1].y + adr->Records[i].y * time;
			}

			if (adr->Records[i].z < UPPER_Z_THRESHOLD && adr->Records[i].z > LOWER_Z_THRESHOLD)
			{
				vfz = 0;
			}
			else
			{
				vfz = vdr->Records[i - 1].z + adr->Records[i].z * time;
			}
			
			vdr->Records[i].setParams(adr->Records[i].currentMilliseconds, vfx, vfy, vfz);
		}
	}
	
	void calculateDisplacements()
	{
		float dfx = 0, dfy = 0, dfz = 0;
		pdr->Records[0].setParams(0, 0, 0, 0);
		
		for (int i = 1; i < noOfRecords; i++)
		{
			float time = vdr->Records[i].currentMilliseconds - vdr->Records[i - 1].currentMilliseconds;	//In milliSeconds
			time = time / 1000;	//In seconds now
			
			dfx = pdr->Records[i - 1].x + vdr->Records[i].x * time;
			dfy = pdr->Records[i - 1].y + vdr->Records[i].y * time;
			dfz = pdr->Records[i - 1].z + vdr->Records[i].z * time;
			
			pdr->Records[i].setParams(vdr->Records[i].currentMilliseconds, dfx, dfy, dfz);
		}
	}
	
	void invertAccelerationsX()
	{
		for (int i = 0; i < noOfRecords; i++)
		{
			adr->Records[i].x = adr->Records[i].x * -1;
		}
	}
	
	void invertPositionsX()
	{
		for (int i = 0; i < noOfRecords; i++)
		{
			pdr->Records[i].x = pdr->Records[i].x * -1;
		}
	}
	
	void invertVelocitiesX()
	{
		for (int i = 0; i < noOfRecords; i++)
		{
			vdr->Records[i].x = vdr->Records[i].x * -1;
		}
	}


	bool printVelocitiesToFile(char* filename)	//scale = 100 for cm, 10 for dm, 1 for m
	{
		fstream myFile(filename, ios::out);
		if (myFile)
		{
			float scale = 1;
			for (int i = 0; i < noOfRecords; i++)
			{
				myFile << vdr->Records[i].currentMilliseconds << ", ";
				myFile << vdr->Records[i].x * scale << ", ";
				myFile << vdr->Records[i].y * scale << ", ";
				myFile << vdr->Records[i].z * scale << "\n";
			}
			myFile.close();
		}
		else
		{
			cout << "Error opening file in Algorithm::printVelocitiesToFile(char* filename)\n";
			return false;
		}
		return true;
	}
	
	bool printPositionsToFile(char* filename, float scale)	//scale = 100 for cm, 10 for dm, 1 for m
	{
		fstream myFile(filename, ios::out);
		if (myFile)
		{
			for (int i = 0; i < noOfRecords; i++)
			{
				myFile << pdr->Records[i].currentMilliseconds << ", ";
				myFile << pdr->Records[i].x * scale << ", ";
				myFile << pdr->Records[i].y * scale << ", ";
				myFile << pdr->Records[i].z * scale << "\n";
			}
			myFile.close();
		}
		else
		{
			cout << "Error opening file in Algorithm::printPositionsToFile(char* filename, int scale)\n";
			return false;
		}
		return true;
	}
	
	bool printPositionsToFileInCM(char* filename)
	{
		return printPositionsToFile(filename, 100);
	}
	
	int getTheNumberOfLinesInFile(char* filename)
	{
		int noOfLines = 0;
		
		fstream myFile(filename);		
		if (myFile)
		{
			char c;
			while (!myFile.eof())
			{
				myFile.get(c);
				if ('\n' == c)
				{
					noOfLines++;
				}
			}
			myFile.close();		
		}
		else
		{
			cout << "Error opening file in getTheNumberOfLinesInFile\n";
		}
		
		return noOfLines;
	}
	
	~Algorithm()
	{
		delete adr;
		delete vdr;
		delete pdr;
	}

};

int getTheNumberOfLinesInFile(char* filename)
{
	fstream myFile(filename);
	int noOfLines = 0;
	
	if (myFile)
	{
		char c;
		while (!myFile.eof())
		{
			myFile.get(c);
			if ('\n' == c)
			{
				noOfLines++;
			}
		}
		myFile.close();		
	}
	else
	{
		cout << "Error opening file in getTheNumberOfLinesInFile\n";
	}
	
	return noOfLines;
}

int main(void)
{
	cout << getTheNumberOfLinesInFile("AccelData.txt") << endl;
	
	Algorithm a("AccelData.txt");
	a.invertAccelerationsX();										//X acceleration inversion
	//a.calculateVelocities();
	a.calculateVelocitiesBasedOnSimpleGaitAssumption();
	//a.invertVelocitiesX();										//X velocity inversion
	a.calculateDisplacements();
	a.printVelocitiesToFile("VelocData.txt");
	//a.invertPositionsX();
	//a.printPositionsToFileInCM("C://Users/Administrative/Desktop/Fall 2019/Human Computer Interaction/Project/2D T_Plotter/data.txt");
	a.printPositionsToFileInCM("Data.txt");
	
	return 0;
}
