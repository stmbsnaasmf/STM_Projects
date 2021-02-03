#include <iostream>
#include <fstream>
#include <cstdlib>		//For generating random numbers
#include <ctime>		//To feed srand()
#include <stdint.h>		//For things like int32_t
using namespace std;

int getTheNumberOfIntegersInFirstLine(char* fileName)
{
	fstream myFile(fileName);
	int counter = 0, value;
	char readChar;
	
	if (myFile.is_open())
	{
		//cout << "The file has been opened." << endl;
	}
	else
	{
		cout << "Error opening file in getTheNumberOfIntegersInFirstLine()." << endl;
		return 0;
	}
	
	while (!myFile.eof())
	{
		myFile >> value;
		counter++;
		
		myFile.get(readChar);	
		
		if (readChar == '\n')
		{
			break;
		}
	}
	
	myFile.close();
	return counter;
}

class AdjacencyMatrix	//Square matrix
{
public:
	
	int* Values;	//1d representation of 2d array.
	int noOfValues;
	int noOfRows;	//Also noOfCols
	
	AdjacencyMatrix()	
	{
		
	}
	
	AdjacencyMatrix(char* fileName)
	{
		noOfRows = getTheNumberOfIntegersInFirstLine(fileName);
		noOfValues = noOfRows * noOfRows;
		Values = new int[noOfValues];
		
		fstream myFile(fileName);
		if (myFile.is_open())
		{
			for (int i = 0; i < noOfValues; i++)
			{
				myFile >> Values[i];
			}	
		}
		else
		{
			cout << "Error opening file in AdjacencyMatrix(char* fileName)" << endl;
		}
		
		myFile.close();
	}
	
	AdjacencyMatrix(const AdjacencyMatrix &m2)
	{
		noOfRows = m2.noOfRows;
		noOfValues = m2.noOfValues;
		
		Values = new int[noOfValues];
		
		for (int i = 0; i < noOfValues; i++)
		{
			Values[i] = m2.Values[i];
		}
	}
	
	int getValue(int rowNo, int colNo)
	{		
		int value;
		int targetIndex = (noOfRows * (rowNo + 1)) - (noOfRows - (colNo + 1)) - 1;
		
		value = Values[targetIndex];
		
		return value;
	}
	
	void printAdjacencyMatrix()
	{
		int index = 0;
		
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfRows; j++)
			{
				cout << Values[index] << "\t";
				index++;
			}
			cout << endl;
		}
	}
	
	~AdjacencyMatrix()
	{
		delete[] Values;
	}
};

class Tour	//Like Individual
{
public:
	
	int* Cities = NULL;	//Like int* Genes	//Also, its size is n+1 and first and last indices contain 0 i-e the first city.
	int noOfCities = 0;	//No of citites in complete circuit
	
	Tour()
	{
		
	}
	
	Tour(int actualNoOfCities)	//no of cities minus the last city in circuit.
	{
		noOfCities = actualNoOfCities + 1;
		Cities = new int[noOfCities];
		Cities[0] = 0;	//First AND home city
		Cities[noOfCities - 1] = 0;	//The last city, which is also the home.
		
		for (int i = 1; i < noOfCities - 1; i++)
		{
			Cities[i] = i;
		}
	}
	
	Tour(const Tour &tour2)	//Deep Copy
	{
		noOfCities = tour2.noOfCities;
		Cities = new int[noOfCities];
		
		for (int i = 0; i < noOfCities; i++)
		{
			Cities[i] = tour2.Cities[i]; 
		}
	}
	
	void operator = (Tour const &tour2)	//Deep Copy
	{
		if (noOfCities == tour2.noOfCities)
		{
			for (int i = 0; i < noOfCities; i++)
			{
				Cities[i] = tour2.Cities[i]; 
			}
		}
		else
		{
			noOfCities = tour2.noOfCities;
			delete[] Cities;

			Cities = new int[noOfCities];	
			
			for (int i = 0; i < noOfCities; i++)
			{
				Cities[i] = tour2.Cities[i]; 
			}
		}
	}

	void fisherYatesShuffling(int *arr, int n)	//Copy and pasted with thnaks from 
												//https://www.sanfoundry.com/cpp-program-implement-fisher-yates-algorithm-array-shuffling/
	{
    	int a[n];
    	int ind[n];
    	for (int i = 0; i < n; i++)
        ind[i] = 0;
    	int index;
 
    	for (int i = 0; i < n; i++)
    	{
        	do
        	{
            	index = rand() % n;
        	}
        	while (ind[index] != 0);
 
	        ind[index] = 1;
    	    a[i] = *(arr + index);
    	}
    	for (int i = 0; i < n; i++)
    	{
    		arr[i] = a[i];
       		//cout << a[i] << " ";
    	}
	}

	void shuffleCitiesRandomly()
	{
		int Arr[noOfCities - 1];
				
		for (int i = 0; i < noOfCities - 2; i++)	//Assigning
		{
			Arr[i] = Cities[i + 1];
		}
		
		fisherYatesShuffling(Arr, noOfCities - 2);
		
		for (int i = 0; i < noOfCities - 2; i++)
		{
			Cities[i + 1] = Arr[i];
		}
	}
	
	
	bool doesContainCity(int city)	//Except the home city at first and last index.
	{
		bool doesContainCity = false;
		
		for (int i = 1; i < noOfCities - 1; i++)
		{
			if (city == Cities[i])
			{
				doesContainCity = true;
			}
		}
		
		return doesContainCity;
	}
	
	void printTour()
	{
		for (int i = 0; i < noOfCities; i++)
		{
			cout << Cities[i] << "\t";
		}
		cout << endl;
	}
	
	void printTourInFile(char* fileName)
	{
		fstream myFile(fileName, std::ios_base::app);
		
		if (myFile.is_open())
		{
//			cout << "Working" << endl;
		}
		else
		{
			cout << "Error opening file in printTourInFile(char* fileName)" << endl;
		}
		
		for (int i = 0; i < noOfCities; i++)
		{
			myFile << Cities[i];
			if (i != (noOfCities - 1))
			{
				myFile << "\t";
			}
		}
		myFile << endl;
		myFile.close();
	}
		
	~Tour()
	{
		delete[] Cities;
		Cities = NULL;
	}
};

class Population
{
public:
	
	Tour* ArrTours = NULL;
	int populationSize = 0;
	int noOfCities = 0;	//No of cities in complete circuit
	int mutationRate = 0;	//Per 10,000
	int uniformRate = 0;
	
	Population()
	{
	}
	
	Population(int popSize, int noOfCities, int mutationRate)
	{
		this->mutationRate = mutationRate;
		populationSize = popSize;
		this->noOfCities = noOfCities;
		ArrTours = new Tour[populationSize];
		
		Tour t(noOfCities - 1);
		for (int i = 0; i < populationSize; i++)
		{
			ArrTours[i] = t;
			ArrTours[i].shuffleCitiesRandomly();
		}
	}
	
	Population (const Population &pop2)
	{
		populationSize = pop2.populationSize;
		noOfCities = pop2.noOfCities;
		ArrTours = new Tour[populationSize];
		
		for (int i = 0; i < populationSize; i++)
		{
			ArrTours[i] = pop2.ArrTours[i];
		}
		
		mutationRate = pop2.mutationRate;
		uniformRate = pop2.uniformRate;
	}
	
	void operator = (Population const &pop2)	//Deep Copy
	{
		noOfCities = pop2.noOfCities;
		
		if (populationSize == pop2.populationSize)
		{
			for (int i = 0; i < populationSize; i++)
			{
				ArrTours[i] = pop2.ArrTours[i];
			}
		}
		else
		{
			populationSize = pop2.populationSize;
			delete[] ArrTours;
			
			ArrTours = new Tour[populationSize];
			
			for (int i = 0; i < populationSize; i++)
			{
				ArrTours[i] = pop2.ArrTours[i];
			}
		}

		mutationRate = pop2.mutationRate;
		uniformRate = pop2.uniformRate;
	}
	
	int getCircuitLength(Tour t, AdjacencyMatrix adj)
	{
		int length = 0;	//Total circuit length
		
		for (int i = 0; i < noOfCities - 1; i++)
		{
			length += adj.getValue(t.Cities[i], t.Cities[i + 1]);	//Distance between two cities
		}

		return length;
	}
	
	int getFitness(Tour t, AdjacencyMatrix adj)
	{
		int fitness = 0;	//Total circuit length
		
		for (int i = 0; i < noOfCities - 1; i++)
		{
			fitness += adj.getValue(t.Cities[i], t.Cities[i + 1]);	//Distance between two cities
		}

		return INT_MAX / fitness;	//Actual fitness
	}
	
	Tour getFittest(AdjacencyMatrix adj)	//Does not leak memory
	{
		Tour fittest = ArrTours[0];
		int fitnessOfTheFittest = getFitness(fittest, adj);
		int fitnessOfTheCandidate;
		
		for (int i = 1; i < populationSize; i++)
		{			
			fitnessOfTheCandidate = getFitness(ArrTours[i], adj);
			
			if (fitnessOfTheCandidate > fitnessOfTheFittest)
			{
				fittest = ArrTours[i];
				fitnessOfTheFittest = fitnessOfTheCandidate;
			}
		}
		
		return fittest;
	}
	
	Tour tournament(int tournamentSize, AdjacencyMatrix adj)	//Does not leak memory
	{
		int someRandomNumber;
		
		Population p(tournamentSize, noOfCities, mutationRate);
		
		for (int i = 0; i < tournamentSize; i++)
		{
			int someRandomNumber = rand() % populationSize;
			p.ArrTours[i] = ArrTours[someRandomNumber];
		}
	
		Tour localFittest = p.getFittest(adj);
		
		return localFittest;
	}
	
	Tour crossover(Tour p1, Tour p2, char* crossoverType)	//Does not leak memory
	{
		Tour child = p1;
		
		int lowerIndex = rand() % (p1.noOfCities - 2) + 1;	//Must be > 0 @ [0] and < 0 @ [noOfCities - 1] 
		int upperIndex = rand() % (p1.noOfCities - 1) + 1;
		
		if (lowerIndex > upperIndex)	//Swap
		{
			int temp = lowerIndex;
			lowerIndex = upperIndex;
			upperIndex = temp;
		}
		
		for (int i = 0; i < child.noOfCities; i++)	//Needed for this crossover
		{
			child.Cities[i] = 0;	
		}
		
		for (int i = lowerIndex; i <= upperIndex; i++)
		{	
			child.Cities[i] = p1.Cities[i];		
		}
		
		for (int i = 1; i < p2.noOfCities - 1; i++)
		{
			int city = p2.Cities[i];
	
			if (!child.doesContainCity(city))
			{
				for (int j = 1; j < child.noOfCities - 1; j++)
				{
					if (child.Cities[j] == 0)
					{
						child.Cities[j] = city;
						break;
					}
				}
			}	
		}
		
		return child;
	}
	
	void mutate(Tour &t)	//Does not leak memory
	{
		for (int i = 1; i < t.noOfCities - 1; i++)
		{
			if (rand() % 10000 < mutationRate)
			{
				int theOtherCity = rand() % (t.noOfCities - 3) + 1;
				
				int temp = t.Cities[i];
				t.Cities[i] = t.Cities[theOtherCity];
				t.Cities[theOtherCity] = temp;
			}
		}
	}
	
	Population* evolvePopulation(Population* prevPop, AdjacencyMatrix adj, int tournamentSize, char* crossoverType)
	{
		Population* newPop = new Population(populationSize, noOfCities, mutationRate);
		
		newPop->ArrTours[0] = prevPop->getFittest(adj);	//Elitism
		
		for (int i = 1; i < populationSize; i++)
		{
			newPop->ArrTours[i] = prevPop->crossover(prevPop->tournament(tournamentSize, adj), prevPop->tournament(tournamentSize, adj), crossoverType);
			mutate(newPop->ArrTours[i]);
		}

		delete this;	//Commits suicide.
		return newPop;
	}
	
	void printPopulation()
	{
		for (int i = 0; i < populationSize; i++)
		{
			for (int j = 0; j < noOfCities; j++)
			{
				cout << ArrTours[i].Cities[j] << "\t";
			}
			cout << endl;
		}
	}
	
	void printCircuitLengthInFile(char* fileName, Tour t, AdjacencyMatrix adj)
	{
		fstream myFile(fileName, std::ios_base::app);
			
		if (myFile.is_open())
		{
//			cout << "Working" << endl;
		}
		else
		{
			cout << "Error opening file in printCircuitLengthInFile(char* fileName, Tour t, AdjacencyMatrix adj)" << endl;
		}
		
		int circuitLength = getCircuitLength(t, adj);
		myFile << circuitLength << endl;
		cout << "Wrote" << endl;
		
		myFile.close();
	}

	
	~Population()
	{
		delete[] ArrTours;
		ArrTours = NULL;
	}
};

int main(void)
{
//	srand(time(NULL));
	
//	char fileName[] = "gr17.txt";	//Name of adjacency matrix file
//	char secFileName[] = "gr17_output.txt";	//Name of output file

//	char fileName[] = "gr21.txt";	//Name of adjacency matrix file
//	char secFileName[] = "gr21_output.txt";	//Name of output file
	
//	char fileName[] = "fri26.txt";	//Name of adjacency matrix file
//	char secFileName[] = "fri26_output.txt";	//Name of output file

//	char fileName[] = "ftv33.txt";	//Name of adjacency matrix file
//	char secFileName[] = "ftv33_output.txt";	//Name of output file
	
//	char fileName[] = "dantzig42.txt";	//Name of adjacency matrix file
//	char secFileName[] = "dantzig42_output.txt";	//Name of output file	
	
//	char fileName[] = "kro124p.txt";	//Name of adjacency matrix file
//	char secFileName[] = "kro124p_output.txt";	//Name of output file	
	
	char fileName[] = "rbg443.txt";	//Name of adjacency matrix file
	char secFileName[] = "rbg443_output.txt";	//Name of output file	
	
	
	
	AdjacencyMatrix* adj = new AdjacencyMatrix(fileName);
	adj->printAdjacencyMatrix();
	
	int noOfRuns = 5;
	int noOfGeneratoinsToRunFor = 500;
	
	for (int i = 0; i < noOfRuns; i++)
	{
		srand(getTheNumberOfIntegersInFirstLine(fileName) * i + getTheNumberOfIntegersInFirstLine(fileName));			
	
		int noOfCities = getTheNumberOfIntegersInFirstLine(fileName) + 1;
		int populationSize = 500;
		int mutationRate = 1500;	//Per 10,000
		int tournamentSize = 50;
		
		Population* pop = new Population(populationSize, noOfCities, mutationRate);
		
		int noOfGenerations = 0;
				
		for (int j = 0; j < noOfGeneratoinsToRunFor; j++)
		//while(1)
		{
			cout << "Generation No: " << noOfGenerations << endl;
			
			pop = pop->evolvePopulation(pop, *adj, tournamentSize, fileName);
			
			Tour t1 = pop->getFittest(*adj);
			t1.printTour();
			
			//t1.printTourInFile(secFileName);
			if (j == noOfGeneratoinsToRunFor - 1)
			{
				pop->printCircuitLengthInFile(secFileName, t1, *adj);					
			}
			
			
			
			cout << "Fitness of the fittest = " << pop->getFitness(t1, *adj) << endl;
			cout << "Circuit length of the fittest = " << pop->getCircuitLength(t1, *adj) << endl;
			
			noOfGenerations++;
		}
	}
	
	return 0;
}



/*
Notes:

1. The function evolve population should never be called from a stack instance of class Population. The reason is that this function
   has a line:
   delete this;
   and deleting this pointer requires that the instance pointed to by this pointer be a newed instance. Doing so with a stack instance may 
   cause runtime crashes.
   
2. It is okay to do this:
	Tour t1 = someFunctionThatUsesNewAndReturnsTheNewedTour();
	Tour t2 = t1;	//Deep copy	//They both are two different and separate but identical entities.
	
   But it is NOT okay to do this:
	Tour t1 = someFunctionThatUsesNewAndReturnsTheNewedTour();
	Tour t2 = someFunctionThatUsesNewAndReturnsTheNewedTour();
	t1 = t2;	//There is a possibilty of memory leaks.	//However, it remains untested.

*/
