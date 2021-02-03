#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>		//For generating random numbers
#include <ctime>		//To feed srand()
#include <stdint.h>		//For things like int32_t
#include <omp.h>

#define NO_OF_LOGICAL_CORES 8

using namespace std;

class NN	//Fully connected
{
//Assuming that edges do have weights. Rather, it is the node that does not have a weight.
//Also assuming that all non-first layer nodes recieve multiple values (from the previous layer nodes)
//and all of these values are summed up, then fed to the activation function and then passed to all nodes in the next layer.
//Rinse and repeat

public:
	
	int noOfNodes = 0;
	int noOfEdges = 0;
	int noOfLayers = 0;

	int range = 0;	//10000;	//Centered around 0. So range = 100 means -100 to +100

	int* NodesValues = NULL;	//Nodes
	int* Weights = NULL;	//Edges
	int* Biases = NULL;
	
	int* ArrayOfNoOfNodesInGivenLayer = NULL;

	int* NoOfNodesInPreviousLayersCombined = NULL;
	//Array size = noOfLayers
	//Each index of this array corresponds to a layer, and for each layer, it contanis the no of elements in all of its previous
	//layers.
	//This array is to ease accessing one dimentional arrays of NodesValues, Weights and Biases.
	
	bool isOutputReady = true;
	
	NN()
	{
	}
	
	
//Do not use constructor without synchronizing it with initialize() function.	
	NN(int noOfLayers, int* AONONIGL, int range)	//AONONIGL = ArrayOfNoOfNodesInGivenLayer
	{
//Do not use constructor without synchronizing it with initialize() function.

		this->range = range;
		
		this->noOfLayers = noOfLayers;
		ArrayOfNoOfNodesInGivenLayer = new int[noOfLayers];
		NoOfNodesInPreviousLayersCombined = new int[noOfLayers];
		
		for (int i = 0; i < noOfLayers; i++)	//Setting (noOfNodes and noOfEdges) as well
		{
			ArrayOfNoOfNodesInGivenLayer[i] = AONONIGL[i];
			noOfNodes = noOfNodes + ArrayOfNoOfNodesInGivenLayer[i];

			if (i != 0)	//Ignore the very first iteration.
			{
				noOfEdges += ArrayOfNoOfNodesInGivenLayer[i - 1] * ArrayOfNoOfNodesInGivenLayer[i];
			}
		}
		
		NodesValues = new int[noOfNodes];
		Weights = new int[noOfEdges];
		Biases = new int[noOfNodes];		//New line
		
		for (int i = 0; i < noOfEdges; i++)
		{
			Weights[i] = (rand() % 2 * range) - range;	//Range = -range to +range
		}
		
		NoOfNodesInPreviousLayersCombined[0] = 0;	//First entry is always 0.	
		for (int i = 1; i < noOfLayers; i++)
		{
			NoOfNodesInPreviousLayersCombined[i] = NoOfNodesInPreviousLayersCombined[i - 1] + ArrayOfNoOfNodesInGivenLayer[i - 1];
		}
	
		for (int i = 0; i < noOfNodes; i++)
		{
			NodesValues[i] = 0;
			Biases[i] = (rand() % 2 * range) - range;
		}		
	}

	NN(const NN &netwotk2)	//Deep Copy
	{
		noOfLayers = netwotk2.noOfLayers;
		noOfEdges = netwotk2.noOfEdges;
		noOfNodes = netwotk2.noOfNodes;
		range = netwotk2.range;
		isOutputReady = netwotk2.isOutputReady;

		ArrayOfNoOfNodesInGivenLayer = new int[noOfLayers];			//1
		NoOfNodesInPreviousLayersCombined = new int[noOfLayers];	//2
		NodesValues = new int[noOfNodes];							//3
		Weights = new int[noOfEdges];								//4
		Biases = new int[noOfEdges];								//5
		
		for (int i = 0; i < noOfLayers; i++)
		{
			ArrayOfNoOfNodesInGivenLayer[i] = netwotk2.ArrayOfNoOfNodesInGivenLayer[i];					//1
			NoOfNodesInPreviousLayersCombined[i] = netwotk2.NoOfNodesInPreviousLayersCombined[i];		//2
		}

		for (int i = 0; i < noOfNodes; i++)
		{
			NodesValues[i] = netwotk2.NodesValues[i];				//3
			Biases[i] = netwotk2.Biases[i];							//5
		}
				
		for (int i = 0; i < noOfEdges; i++)
		{
			Weights[i] = netwotk2.Weights[i];					//4
		}
	
	}
	
//Same as copy constructor except for the commented part. 
	void operator = (NN const &netwotk2)	//Deep Copy
	{
//Same as copy constructor except for the commented part. 
		
		noOfLayers = netwotk2.noOfLayers;
		noOfEdges = netwotk2.noOfEdges;
		noOfNodes = netwotk2.noOfNodes;
		range = netwotk2.range;
		isOutputReady = netwotk2.isOutputReady;
/*
		ArrayOfNoOfNodesInGivenLayer = new int[noOfLayers];			//1
		NoOfNodesInPreviousLayersCombined = new int[noOfLayers];	//2
		NodesValues = new int[noOfNodes];							//3
		Weights = new int[noOfEdges];								//4
		Biases = new int[noOfEdges];								//5
*/		
		for (int i = 0; i < noOfLayers; i++)
		{
			ArrayOfNoOfNodesInGivenLayer[i] = netwotk2.ArrayOfNoOfNodesInGivenLayer[i];					//1
			NoOfNodesInPreviousLayersCombined[i] = netwotk2.NoOfNodesInPreviousLayersCombined[i];		//2
		}

		for (int i = 0; i < noOfNodes; i++)
		{
			NodesValues[i] = netwotk2.NodesValues[i];				//3
			Biases[i] = netwotk2.Biases[i];							//5
		}
				
		for (int i = 0; i < noOfEdges; i++)
		{
			Weights[i] = netwotk2.Weights[i];					//4
		}		
	}
	
	void initialize(int noOfLayers, int* AONONIGL, int range)	//AONONIGL = ArrayOfNoOfNodesInGivenLayer
	{
		this->range = range;
		
		this->noOfLayers = noOfLayers;
		ArrayOfNoOfNodesInGivenLayer = new int[noOfLayers];
		NoOfNodesInPreviousLayersCombined = new int[noOfLayers];
		
		for (int i = 0; i < noOfLayers; i++)	//Setting noOfNodes and noOfEdges
		{
			ArrayOfNoOfNodesInGivenLayer[i] = AONONIGL[i];
			noOfNodes = noOfNodes + ArrayOfNoOfNodesInGivenLayer[i];
			
			if (i != 0)	//Ignore the very first iteration.
			{
				noOfEdges += ArrayOfNoOfNodesInGivenLayer[i - 1] * ArrayOfNoOfNodesInGivenLayer[i];
			}
		}
		
		NodesValues = new int[noOfNodes];
		Weights = new int[noOfEdges];
		Biases = new int[noOfEdges];
		
		int theNumberToModTheRand_By = 10;		//For initializing weights and biases
		
		for (int i = 0; i < noOfEdges; i++)	//Initializing weights
		{
			if (rand() % 2 == 0)
			{
				Weights[i] = rand() % theNumberToModTheRand_By;
			}
			else
			{
				Weights[i] = rand() % theNumberToModTheRand_By * -1;
			}
		}
		
		for (int i = 0; i < noOfNodes; i++)	//Initializing biases
		{
			if (rand() % 2 == 0)
			{
				Biases[i] = Biases[i] = rand() % theNumberToModTheRand_By;
			}
			else
			{
				Biases[i] = rand() % theNumberToModTheRand_By * -1;
			}
			
		}
		
		NoOfNodesInPreviousLayersCombined[0] = 0;	//First entry is always 0.	
		for (int i = 1; i < noOfLayers; i++)
		{
			NoOfNodesInPreviousLayersCombined[i] = NoOfNodesInPreviousLayersCombined[i - 1] + ArrayOfNoOfNodesInGivenLayer[i - 1];
		}
	
		for (int i = 0; i < noOfNodes; i++)
		{
			NodesValues[i] = 0;
		}		
			
	}
		
	int getValue(int noOfIthLayer, int noOfIthNode)	//Seems to be working fine. Returns the value at a node.
	{
		int targetIndex = NoOfNodesInPreviousLayersCombined[noOfIthLayer] + noOfIthNode;
		return NodesValues[targetIndex];
	}	
	
//The following is the actual feed function
	void test(int* InputValues)	//Size of InputValues must conform to no of nodes in first layer.
	{
		isOutputReady = false;	//Locking
		
		//Feed the input layer
		for (int i = 0; i < ArrayOfNoOfNodesInGivenLayer[0]; i++)	//Working of this loop has NOT been tested yet.
		{
			NodesValues[NoOfNodesInPreviousLayersCombined[0] + i] = InputValues[i];
			//NoOfNodesInPreviousLayersCombined[noOfIthLayer] + noOfIthNode
		}
		
		//Feed the next layers.
		int index = 0;
		for (int i = 0; i < noOfLayers - 1; i++)	//For each layer (except the last one)
		{
			for (int j = 0; j < ArrayOfNoOfNodesInGivenLayer[i]; j++)	//For each node
			{
				int theValue = NodesValues[NoOfNodesInPreviousLayersCombined[i] + j];
				
				
				for (int k = 0; k < ArrayOfNoOfNodesInGivenLayer[i + 1]; k++)	//For each node in next layer
				{
					//Set next layer's node values to zero before summing up values from previous(which is current at this moment)
					//layer nodes.
					//This is done to all nodes of the next layer BEFORE the first node of current layer feeds its value
					//to all nodes in the next layer.
					if (0 == j)
					{
						NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k] = 0;
					}
					
//					NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k] += (theValue * Weights[index] + Biases[index]);
					//The above is the old line
					
					NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k] += (theValue * Weights[index]);
					index++;
					
//					cout << "j = " << j << endl;
//					cout << "ArrayOfNoOfNodesInGivenLayer[i] - 1 = " << ArrayOfNoOfNodesInGivenLayer[i] - 1 << endl;
					
					
					//When the last node of current layer has sent its value to the nodes of next layer, then as soon
					//as a node of next layer has receivd its last input, apply its activation function.
					if (j == ArrayOfNoOfNodesInGivenLayer[i] - 1)
					{
					//In addition to the lines written above the if() block, nodes biases are subtracted here as well.

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//The bias subtraction code begis here.		3 March 2019
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						
						NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k] -= Biases[NoOfNodesInPreviousLayersCombined[i + 1] + k];						
						
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//The bias subtraction code ends here.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!						
						
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Introducing "averaging instead of summing all values from the nodes of previous layer" here. 23 Feb 2019
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//It should probably be done in this if() block.

//Because the summing is done above at the line:

//NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k] += (theValue * Weights[index] + Biases[index]);

//Therefore, only the division is done here.

						NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k] /= ArrayOfNoOfNodesInGivenLayer[i]; 

//Seems to be working.	

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//The averaging code ends here.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Introducing ReLU here. 23 Feb 2019
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//The code for tanh() has been commented out.

						int rectifiedValue = NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k];
						if (rectifiedValue < 0)
						{
							rectifiedValue = 0;
						}
						NodesValues[NoOfNodesInPreviousLayersCombined[i + 1] + k] = rectifiedValue;
						
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//The ReLu code ends here.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					}
				}
			}
		}
		
		isOutputReady = true;		
	}
	
	void printOutput()
	{
		for (int i = 0; i < ArrayOfNoOfNodesInGivenLayer[noOfLayers - 1]; i++)
		{
			//noOfLayers - 1 = last layer
			//NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + i - 1 //Because for an array int A[10], the last index is A[9].
			int targetIndex = NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + i - 1;
			cout << NodesValues[targetIndex] << "\t";
		}
		cout << endl;
	}

	~NN()
	{
		delete[] ArrayOfNoOfNodesInGivenLayer;
		delete[] NoOfNodesInPreviousLayersCombined;
		delete[] NodesValues;
		delete[] Weights;
		delete[] Biases;
	}
};

class GA	//Same structure for every individual
{
public:

//Data Section Begins
	
//Each individual is to be tested against one instance of each class for the individual's fitness calculation.	
	
	int noOfClasses;			//How many classes are there in the data?	//Also dictates the no of nodes in output layer.
	int noOfInstancesPerClass;	//Assumes equal distribution
	int noOfInputParameters;
	int totalNoOfInstances;
	
	int percentageOfInstancesToTestAgainstForEachClass = 20;	//Per 100
	
	int** InputData = NULL;		//Not DELETED yet.
//	int** ExpectedOutput = NULL;

	int lowValue = 1000;
	int highValue = 5000;

//Data Section Ends

	
	int populationSize;
	int tournamentSize;
	int mutationRate = 2500;			//Per 10,000	//25% @ 2500
	int crossoverProbability = 5000;	//Per 10,000	//50% @ 5000
	int range;
	
	int noOfLayers;
	int* ArrayOfNoOfNodesInGivenLayer = NULL;
	
	int noOfNodesInInputLayer;
	int noOfNodesInOutputLayer;
	
	NN* ArrayOfNeuralNetworks = NULL;
	
	int *Fitnesses;	//So that in functions like getFittest(), fitness does not need to be calculated multiple times.
	
	GA()
	{
	}

	GA(int noOfLayers, int* AONONIGL, int popSize, int tournamentSize, int mutationRate, int range)	//AONONIGL = ArrayOfNoOfNodesInGivenLayer
	{
		this->noOfLayers = noOfLayers;
		ArrayOfNoOfNodesInGivenLayer = new int[noOfLayers];
		for (int i = 0; i < noOfLayers; i++)
		{
			ArrayOfNoOfNodesInGivenLayer[i] = AONONIGL[i];
		}
		
		populationSize = popSize;
		this->tournamentSize = tournamentSize;
		this->mutationRate = mutationRate;
		this->range = range;
		
		noOfNodesInInputLayer = ArrayOfNoOfNodesInGivenLayer[0];
		noOfNodesInOutputLayer = ArrayOfNoOfNodesInGivenLayer[noOfLayers - 1];	//Last index
		
		ArrayOfNeuralNetworks = new NN[populationSize];
		for (int i = 0; i < populationSize; i++)
		{
			ArrayOfNeuralNetworks[i].initialize(noOfLayers, AONONIGL, range);
		}
		
		Fitnesses = new int[populationSize];
	}
	
	void fisherYatesShuffling(int *arr, int n)	//Copy and pasted with thnaks from 
												//https://www.sanfoundry.com/cpp-program-implement-fisher-yates-algorithm-array-shuffling/
	{
//    	int a[n];
//  	int ind[n];
		int* a = new int[n];
		int* ind = new int[n];

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
    	
    	delete[] a;
    	delete[] ind;
	}
	
	bool readData(char* filename, int noOfClasses, int noOfInputParameters, int totalNoOfInstances)
	//Returns false if fails to open file.
	{
/*Assumes the following file format:

inputPara1 ... inputParaN classNumber
		
*/		
		this->noOfClasses = noOfClasses;
		this->noOfInputParameters = noOfInputParameters;
		this->totalNoOfInstances = totalNoOfInstances;
		noOfInstancesPerClass = totalNoOfInstances / noOfClasses;

		InputData = new int*[totalNoOfInstances];
		for (int i = 0; i < totalNoOfInstances; i++)
		{
			InputData[i] = new int[noOfInputParameters + 1];	//Input params + classNumber
		}

		fstream myFile(filename);
		
		if (myFile.is_open())
		{
			//cout << "The file has been opened." << endl;
		}
		else
		{
			cout << "Error opening file in GA::readData()" << endl;
			return false;
		}		
		
		int readValue;
		
		int index1 = 0;	//For all instances of data
		int index2 = 0;	//For each instance of data
		while (!myFile.eof())
		{
			myFile >> readValue;
			InputData[index1][index2] = readValue;
			index2++;
			
			if (index2 == noOfInputParameters + 1)
			{
				index2 = 0;
				index1++;
			}
		}
		
		return true;
	}
	
	void printData()
	{
		for (int i = 0; i < totalNoOfInstances; i++)
		{
			for (int j = 0; j < noOfInputParameters + 1; j++)
			{
				cout << InputData[i][j] << "\t";
			}
			cout << endl;
		}
	}

	int calculateFitness(NN* n)	//New function
	//Tests against one randomly selected instance of each class and returns sum(expected - actual) for all output nodes.
	{
		int* Classes = NULL;				//Contains the number of classes
		Classes = new int[noOfClasses];
		for (int i = 0; i < noOfClasses; i++)
		{
			Classes[i] = i;
		}
		
		int noOfInstancesToTestAgainstForEachClass = 
		int (float (noOfInstancesPerClass * float ( percentageOfInstancesToTestAgainstForEachClass/ 100.0) ));
		int** RandomIndices = NULL;	//RandomIndices[classNumber][instanceNumber]
		RandomIndices = new int*[noOfClasses];
		
//		cout << "noOfInstancesToTestAgainstForEachClass = " << noOfInstancesToTestAgainstForEachClass << endl;

		for (int i = 0; i < noOfClasses; i++)
		{
			RandomIndices[i] = new int[noOfInstancesToTestAgainstForEachClass];
		}
		
		
		
		fisherYatesShuffling(Classes, noOfClasses);	//Shuffle it.	//This is to select classes randomly so that not only the 
																	//index from each class is chosen randomly, the order of
																	//classes is random as well.
		
		int* InputValues = new int[ArrayOfNoOfNodesInGivenLayer[0]];	//To feed input parameters.
		
		int highNode;	//Which output node should be high? This node is equal to the class number since each output 
						//node denotes a class(by means of class numbers).
		
		int sumOfDifferences = 0;
		int fitness = 0;
		int difference;
		int expectedOutputNodeValue;

		for (int i = 0; i < noOfClasses; i++)	//For each class,
		{
			int lowerNumber = Classes[i] * noOfInstancesPerClass;	//Inclusive
			
//			int randomIndex = lowerNumber + (rand() % noOfInstancesPerClass);
	
			for (int j = 0; j < noOfInstancesToTestAgainstForEachClass; j++)	//get multiple random indices
			{
				RandomIndices[i][j] = lowerNumber + (rand() % noOfInstancesPerClass);

			}
		}
		
		for (int i = 0; i < noOfClasses; i++)	//For each class,
		{
			for (int j = 0; j < noOfInstancesToTestAgainstForEachClass; j++)	//test against its multiple random indices.
			{
				//For each test, fill the fed array:
				for (int k = 0; k < noOfInputParameters; k++)	//For InputValues (to feed the input layer),
				{
					InputValues[k] = InputData[RandomIndices[i][j]][k];
				}
				highNode = InputData[RandomIndices[i][j]][noOfInputParameters];	//and for highNode
			
				n->test(InputValues);
				
				
				//Calculate fitness, difference etc below:
				for (int k = 0; k < ArrayOfNoOfNodesInGivenLayer[noOfLayers - 1]; k++)	//For each output node
				{
					//NoOfNodesInPreviousLayersCombined[noOfIthLayer] + noOfIthNode;	//Comment for indexing below.
					int actualValue = (n->NodesValues[n->NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + k]);
			
					if (k == highNode)	//In this case, value at kth output node should be high.
					{
						difference = highValue - actualValue;	//Expected - Actual
					
						expectedOutputNodeValue = highValue;
					}
					else	//In this case, value at kth output node should be low.
					{
						difference = lowValue - actualValue;	//Expected - Actual					
					
						expectedOutputNodeValue = lowValue;
					}

					if (difference < 0)	//If defference is negative, make it positive.
					{
						difference *= -1;	
					}

					fitness += difference;

//					cout << "EO: " << expectedOutputNodeValue << "            \t\t";
//					cout << "AO: " << actualValue << "            \t\t";				
//					cout << "D: " << difference << endl;

					sumOfDifferences += difference;

				}
			}	
		}

//		cout << endl << "Sum of Differences: " << sumOfDifferences << endl << endl;	

					
//		for (int i = 0; i < noOfClasses; i++)
//		{
//			for (int j = 0; j < noOfInstancesToTestAgainstForEachClass; j++)
//			{
//				cout << RandomIndices[i][j] << "\t";
//			}
//			cout << endl;
//		}
		
		fitness = (INT_MAX / 10) - fitness;
		
		delete[] Classes;
		delete[] InputValues;
		
		if (NULL != RandomIndices)
		{
			for (int i = 0; i < noOfClasses; i++)
			{
				delete[] RandomIndices[i];
				RandomIndices[i] = NULL;
			}
			
			delete[] RandomIndices;
		}
		
		return fitness;
	}

	int calculateFitnessAndPrint(NN* n)	//New function
	//Tests against one randomly selected instance of each class and returns sum(expected - actual) for all output nodes.
	{
		int* Classes = NULL;				//Contains the number of classes
		Classes = new int[noOfClasses];
		for (int i = 0; i < noOfClasses; i++)
		{
			Classes[i] = i;
		}

		int noOfInstancesToTestAgainstForEachClass = 
		int (float (noOfInstancesPerClass * float ( percentageOfInstancesToTestAgainstForEachClass/ 100.0) ));
		int** RandomIndices = NULL;	//RandomIndices[classNumber][instanceNumber]
		RandomIndices = new int*[noOfClasses];
		
//		cout << "noOfInstancesToTestAgainstForEachClass = " << noOfInstancesToTestAgainstForEachClass << endl;

		for (int i = 0; i < noOfClasses; i++)
		{
			RandomIndices[i] = new int[noOfInstancesToTestAgainstForEachClass];
		}
		
		
		
		fisherYatesShuffling(Classes, noOfClasses);	//Shuffle it.	//This is to select classes randomly so that not only the 
																	//index from each class is chosen randomly, the order of
																	//classes is random as well.
		
		int* InputValues = new int[ArrayOfNoOfNodesInGivenLayer[0]];	//To feed input parameters.
		
		int highNode;	//Which output node should be high? This node is equal to the class number since each output 
						//node denotes a class(by means of class numbers).
		
		int sumOfDifferences = 0;
		int fitness = 0;
		int difference;
		int expectedOutputNodeValue;

		for (int i = 0; i < noOfClasses; i++)	//For each class,
		{
			int lowerNumber = Classes[i] * noOfInstancesPerClass;	//Inclusive
			
//			int randomIndex = lowerNumber + (rand() % noOfInstancesPerClass);
	
//			if (0 == i)
//			{
//				cout << "This time the first class is: " << Classes[0] << endl;
//			}



			for (int j = 0; j < noOfInstancesToTestAgainstForEachClass; j++)	//get multiple random indices
			{
				RandomIndices[i][j] = lowerNumber + (rand() % noOfInstancesPerClass);

			}
		}
		
		for (int i = 0; i < noOfClasses; i++)	//For each class,
		{
			for (int j = 0; j < noOfInstancesToTestAgainstForEachClass; j++)	//test against its multiple random indices.
			{
				//For each test, fill the fed array:
				for (int k = 0; k < noOfInputParameters; k++)	//For InputValues (to feed the input layer),
				{
					InputValues[k] = InputData[RandomIndices[i][j]][k];
				}
				highNode = InputData[RandomIndices[i][j]][noOfInputParameters];	//and for highNode
			
				n->test(InputValues);
				
				
				//Calculate fitness, difference etc below:
				cout << endl;
				for (int k = 0; k < ArrayOfNoOfNodesInGivenLayer[noOfLayers - 1]; k++)	//For each output node
				{
					//NoOfNodesInPreviousLayersCombined[noOfIthLayer] + noOfIthNode;	//Comment for indexing below.
					int actualValue = (n->NodesValues[n->NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + k]);
			
					if (k == highNode)	//In this case, value at kth output node should be high.
					{
						difference = highValue - actualValue;	//Expected - Actual
					
						expectedOutputNodeValue = highValue;
					}
					else	//In this case, value at kth output node should be low.
					{
						difference = lowValue - actualValue;	//Expected - Actual					
					
						expectedOutputNodeValue = lowValue;
					}

					if (difference < 0)	//If defference is negative, make it positive.
					{
						difference *= -1;	
					}

					fitness += difference;

					cout << "EO: " << expectedOutputNodeValue << "            \t\t";
					cout << "AO: " << actualValue << "            \t\t";				
					cout << "D: " << difference << endl;

					sumOfDifferences += difference;

				}
			}	
		}

		cout << endl << "Sum of Differences: " << sumOfDifferences << endl << endl;	
					
//		for (int i = 0; i < noOfClasses; i++)
//		{
//			for (int j = 0; j < noOfInstancesToTestAgainstForEachClass; j++)
//			{
//				cout << RandomIndices[i][j] << "\t";
//			}
//			cout << endl;
//		}
		
		fitness = (INT_MAX / 10) - fitness;
		
		delete[] Classes;
		delete[] InputValues;
		
		if (NULL != RandomIndices)
		{
			for (int i = 0; i < noOfClasses; i++)
			{
				delete[] RandomIndices[i];
				RandomIndices[i] = NULL;
			}
			
			delete[] RandomIndices;
		}
		
		return fitness;
	}

	void calcalculateSubPopulationFitnesses(int lowerIndex, int upperIndex)
	{
		for (int i = lowerIndex; i < upperIndex; i++)
		{
			int value = calculateFitness(&ArrayOfNeuralNetworks[i]);
			Fitnesses[i] = value;
		}
		
	}

	void calculateAllFitnesses()
	{
		int subPopulationSize = populationSize / NO_OF_LOGICAL_CORES;
		int threadNumber = 0;
		int lowerIndex = 0;									//Inclusive
		int upperIndex = lowerIndex + subPopulationSize;	//Exclusive
				
		#pragma omp parallel num_threads(NO_OF_LOGICAL_CORES)
		{
			if (upperIndex >= populationSize)	//So that if the division by number of threads created leaves a remainder, it 
			{									//does not cause problems.
				upperIndex = populationSize;
			}

			srand(rand() + rand() * omp_get_thread_num());	//Feed srand() with a different number for each thread.
															//Do not move this line.

			calcalculateSubPopulationFitnesses(lowerIndex, upperIndex);

			#pragma omp critical
			{
//				cout << "Thread Id: " << omp_get_thread_num() << " of Total " << omp_get_num_threads() << " Threads" << endl;
//				cout << "Lower Index: " << lowerIndex << endl;
//				cout << "Upper Index: " << upperIndex << endl;
//				cout << endl;

				lowerIndex = upperIndex;
				upperIndex = upperIndex + subPopulationSize;				
			}
			
		}
		
//		for (int i = 0; i < populationSize; i++)
//		{
//			int value = calculateFitness(&ArrayOfNeuralNetworks[i]);
//			Fitnesses[i] = value;
//		}	
	}
	
	int calculateFitness(NN* n, int** Inputs, int noOfInputs, int noOfInputValues, int** Outputs, int noOfOutputValues)	
	//Test against all test cases and return sumForAllCases(expected - actual)
	{
		int fitness = 0;
		
		int *InputValues = new int[noOfInputValues];
		
		for (int i = 0; i < noOfInputs; i++)
		{
			for (int j = 0; j < noOfInputValues; j++)
			{
				InputValues[j] = Inputs[i][j];
			}

//			for (int j = 0; j < noOfInputValues; j++)
//			{
//				cout << InputValues[j] << "\t";
//			}
//			cout<< endl;

			n->test(InputValues);
			
//			cout << "Tested times " << i << endl;
	
//			cout << "Inputs:\t";
			for (int m = 0; m < noOfInputValues; m++)
			{
//				cout << InputValues[m] << "\t";
			}
//			cout << endl;

			int expectedOutputNodeValue;
				
			for (int k = 0; k < noOfOutputValues; k++)
			{
				
//				for (int m = 0; m < noOfOutputValues; m++)
				{
					expectedOutputNodeValue = Outputs[i][k];
//					cout << "Expected output:\t" << expectedOutputNodeValue << "\t";
//					cout << "Actual output:\t" << n.NodesValues[n.NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + k] << endl;
//					cout << "fitness: " << fitness << endl;

				//NoOfNodesInPreviousLayersCombined[noOfIthLayer] + noOfIthNode;	//Comment for indexing below.
				int difference = expectedOutputNodeValue - (n->NodesValues[n->NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + k]);

//				int worstPossibleDifference = 0;
				
				if (difference < 0)
				{
					difference *= -1;	//So that the error is always positive.
				}
				
//				cout << "Difference squared / range = " << (difference * difference) / range << endl;
				
				fitness += difference;// * difference;		//Squaring produces overflows i-e- values > INT_MAX
				
				
				if (0 == fitness)	//Just in case, to avoid division by 0.
				{
					fitness += 1;
				}

				}		
			}		
		}

		fitness = (INT_MAX / 10) - fitness;	
		
		delete[] InputValues;
		return fitness;
	}
	
	int calculateFitnessAndPrint(NN n, int** Inputs, int noOfInputs, int noOfInputValues, int** Outputs, int noOfOutputValues)	
	//Test against all test cases and return sumForAllCases(expected - actual)
	{
		int fitness = 0;
		
		int *InputValues = new int[noOfInputValues];
		
		for (int i = 0; i < noOfInputs; i++)
		{
			for (int j = 0; j < noOfInputValues; j++)
			{
				InputValues[j] = Inputs[i][j];
			}
			
//			for (int j = 0; j < noOfInputValues; j++)
//			{
//				cout << InputValues[j] << "\t";
//			}
//			cout<< endl;

			n.test(InputValues);
			
//			cout << "Tested times " << i << endl;
	
//			cout << "Inputs:\t";
			for (int m = 0; m < noOfInputValues; m++)
			{
//				cout << InputValues[m] << "\t";
			}
//			cout << endl;

			int expectedOutputNodeValue;
				
			for (int k = 0; k < noOfOutputValues; k++)
			{
				
//				for (int m = 0; m < noOfOutputValues; m++)
				{
					expectedOutputNodeValue = Outputs[i][k];
					cout << "EO: " << expectedOutputNodeValue << "\t\t";
					cout << "AO: " << n.NodesValues[n.NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + k] << "    \t\t";
//					cout << "fitness: " << fitness << endl;

				//NoOfNodesInPreviousLayersCombined[noOfIthLayer] + noOfIthNode;	//Comment for indexing below.
				int difference = expectedOutputNodeValue - (n.NodesValues[n.NoOfNodesInPreviousLayersCombined[noOfLayers - 1] + k]);
				if (difference < 0)
				{
					difference *= -1;	//So that the error is always positive.
				}
				
				cout << "D: " << difference << endl;
				
//				cout << "Difference squared / range = " << (difference * difference) / range << endl;
				
				fitness += difference;// * difference;		//Squaring produces overflows i-e- values > INT_MAX
											
				if (0 == fitness)	//Just in case, to avoid division by 0.
				{
					fitness += 1;
				}

				}		
			}		
		}

		fitness = (INT_MAX / 10) - fitness;	

		delete[] InputValues;
		return fitness;
	}	
	
	void calculateAllFitnesses(int** Inputs, int noOfInputs, int noOfInputValues, int** Outputs, int noOfOutputValues)
	{
		for (int i = 0; i < populationSize; i++)
		{
			int value = calculateFitness(&ArrayOfNeuralNetworks[i], Inputs, noOfInputs, noOfInputValues, Outputs, noOfOutputValues);
			Fitnesses[i] = value;
		}	
	}

	int getLocalFittest(int *SubPopulation, int sizeOfTournament)
	{
		//SubPopulation array contains indices from ArrayOfNeuralNetworks.
		
		int fitnessOfTheFittest = Fitnesses[SubPopulation[0]];
		int theFittest = SubPopulation[0];
		
		for (int i = 1; i < sizeOfTournament; i++)
		{
			if (Fitnesses[SubPopulation[i]] > fitnessOfTheFittest)
			{
				fitnessOfTheFittest = Fitnesses[SubPopulation[i]];
				theFittest = SubPopulation[i];
			}
		}

		return theFittest;	//Returns the fittest index from ArrayOfNeuralNetworks among the SubPopulation.
	}

	int tournament()	//Returns an index of ArrayOfNeuralNetworks.
	{
		int* SubPopulation = new int[tournamentSize];	//After the for loop below, it will contain indeices from ArrayOfNeuralNetworks.
		
		for (int i = 0; i < tournamentSize; i++)
		{
			SubPopulation[i] = rand() % populationSize;
		}
		
		int theFittest = getLocalFittest(SubPopulation, tournamentSize);
		
		delete[] SubPopulation;
		return theFittest;
	}
	
	void crossover(NN* p1, NN* p2, NN &child, int crossoverType)
	{
		if (1 == crossoverType)	//One edge from parent 1; another from parent 2.
		{
			for (int i = 0; i < child.noOfEdges; i++)	//For weights
			{
				if (i % 2 == 0)
				{
					child.Weights[i] = p1->Weights[i];
				}
				else
				{
					child.Weights[i] = p2->Weights[i];
				}
			}
			
			for (int i = 0; i < child.noOfNodes; i++)	//For biases
			{
				if (i % 2 == 0)
				{
					child.Biases[i] = p1->Biases[i];
				}
				else
				{
					child.Biases[i] = p2->Biases[i];
				}
			}
		
		}
		
	}
	
	void mutate(NN &ind)
	{	
		int theValueToModThe_randByForWeights = 1;
		int theValueToModThe_randByForBiases = 10;
				
		for (int i = 0; i < ind.noOfEdges; i++)	//For mutating weights
		{	
			if (rand() % 10000 < mutationRate)
			{
				if (rand() % 2 == 0)
				{
					ind.Weights[i] += rand() % theValueToModThe_randByForWeights;
				}
				else
				{
					ind.Weights[i] -= rand() % theValueToModThe_randByForWeights;
				}
				
			}
		}
		
		for (int i = 0; i < ind.noOfNodes; i++)	//For mutating biases
		{
			if (rand() % 10000 < mutationRate)
			{
				if (rand() % 2 == 0)
				{
					ind.Biases[i] += rand() % theValueToModThe_randByForBiases;
				}
				else
				{				
					ind.Biases[i] -= rand() % theValueToModThe_randByForBiases;
				}				
			}		
		}
	}
	
	void evolve(int crossoverProbability, int** Inputs, int noOfInputs, int noOfInputValues, int** Outputs, int noOfOutputValues)
	{		
		NN* NewPop = new NN[populationSize];
		for (int i = 0; i < populationSize; i++)
		{
			NewPop[i].initialize(noOfLayers, ArrayOfNoOfNodesInGivenLayer, range);
		}
		
		//Following code is for elitism.
		int* Pop = new int[populationSize];
		for (int j = 0; j < populationSize; j++)
		{
			Pop[j] = j;
		}
		int theFittest = getLocalFittest(Pop, populationSize);
		NewPop[0] = ArrayOfNeuralNetworks[theFittest];
		delete[] Pop;
		
		for (int i = 1; i < populationSize; i++)
		{
			int p1, p2;
			p1 = tournament();
			p2 = tournament();
					
			if (rand() % 10000 < crossoverProbability)
			{
				crossover(&ArrayOfNeuralNetworks[p1], &ArrayOfNeuralNetworks[p2], NewPop[i], 1);	//1 is crossover type.
			}
			else
			{
				NewPop[i] = ArrayOfNeuralNetworks[i];
			}
			
			mutate(NewPop[i]);
		}
		
		for (int i = 0; i < populationSize; i++)
		{
			ArrayOfNeuralNetworks[i] = NewPop[i];
		}
		
		delete[] NewPop;
	}

	void evolveSubPopulation(int lowerIndex, int upperIndex, NN* NewPop)	//Should be called after elitism code
	{
		for (int i = lowerIndex; i < upperIndex; i++)
		{
			int p1, p2;
			p1 = tournament();
			p2 = tournament();
					
			if (rand() % 10000 < crossoverProbability)
			{
				crossover(&ArrayOfNeuralNetworks[p1], &ArrayOfNeuralNetworks[p2], NewPop[i], 1);	//1 is crossover type.
			}
			else
			{
				NewPop[i] = ArrayOfNeuralNetworks[i];
			}
			
			mutate(NewPop[i]);
		}
		
		for (int i = lowerIndex; i < upperIndex; i++)
		{
			ArrayOfNeuralNetworks[i] = NewPop[i];
		}

	}
	
	void evolve()
	{
		NN* NewPop = new NN[populationSize];
		for (int i = 0; i < populationSize; i++)
		{
			NewPop[i].initialize(noOfLayers, ArrayOfNoOfNodesInGivenLayer, range);
		}
		
		//Following code is for elitism.
		int* Pop = new int[populationSize];
		for (int j = 0; j < populationSize; j++)
		{
			Pop[j] = j;
		}
		int theFittest = getLocalFittest(Pop, populationSize);
		NewPop[0] = ArrayOfNeuralNetworks[theFittest];
		delete[] Pop;
		
		int subPopulationSize = populationSize / NO_OF_LOGICAL_CORES;
		int threadNumber = 0;
		int lowerIndex = 0;
		int upperIndex = lowerIndex + subPopulationSize;

		#pragma omp parallel num_threads(NO_OF_LOGICAL_CORES)
		{
			if (upperIndex >= populationSize)	//So that if the division by number of threads created leaves a remainder, it 
			{									//does not cause problems.
				upperIndex = populationSize;
			}
			if (0 == lowerIndex)	//Elitism
			{
				lowerIndex = 1;
			}

			evolveSubPopulation(lowerIndex, upperIndex, NewPop);

			#pragma omp critical
			{
//				cout << "Thread Id: " << omp_get_thread_num() << " of Total " << omp_get_num_threads() << " Threads" << endl;
//				cout << "Lower Index: " << lowerIndex << endl;
//				cout << "Upper Index: " << upperIndex << endl;
//				cout << endl;

				lowerIndex = upperIndex;
				upperIndex = upperIndex + subPopulationSize;				
			}
			
		}

//		for (int i = 1; i < populationSize; i++)
//		{
//			int p1, p2;
//			p1 = tournament();
//			p2 = tournament();
//					
//			if (rand() % 10000 < crossoverProbability)
//			{
//				crossover(ArrayOfNeuralNetworks[p1], ArrayOfNeuralNetworks[p2], NewPop[i], 1);	//1 is crossover type.
//			}
//			else
//			{
//				NewPop[i] = ArrayOfNeuralNetworks[i];
//			}
//			
//			mutate(NewPop[i]);
//		}
//		
//		for (int i = 0; i < populationSize; i++)
//		{
//			ArrayOfNeuralNetworks[i] = NewPop[i];
//		}
		
		delete[] NewPop;		
	}

	~GA()
	{
		if (NULL != InputData)
		{
			for (int i = 0; i < totalNoOfInstances; i++)
			{
				delete[] InputData[i];
				InputData[i] = NULL;
			}
			
			delete[] InputData;
			InputData = NULL;
		}
		
		delete[] ArrayOfNoOfNodesInGivenLayer;
		delete[] Fitnesses;
		delete[] ArrayOfNeuralNetworks;
	}
};

int main(void)
{
	srand(time(NULL));
	cout << "INT_MAX = " << INT_MAX << endl;

	unsigned long previousMillis = time(NULL);
//	cout << "Now: " << previousMillis << endl;

	int popSize = 20000;
	int tournamentSize = popSize / 1000;
	int mutationRate = 4000;
	int noOfGenerations = 100000;

	int range = 100000;	//1,00,000	//2147483647
	int noOfLayers = 5;
	int* ArrayOfNoOfNodesInGivenLayer = new int[noOfLayers];
	
	ArrayOfNoOfNodesInGivenLayer[0] = 4;				//First layer
	ArrayOfNoOfNodesInGivenLayer[1] = 8;
	ArrayOfNoOfNodesInGivenLayer[2] = 8;
	ArrayOfNoOfNodesInGivenLayer[3] = 8;
//	ArrayOfNoOfNodesInGivenLayer[4] = 6;
//	ArrayOfNoOfNodesInGivenLayer[5] = 7;
//	ArrayOfNoOfNodesInGivenLayer[6] = 7;
	ArrayOfNoOfNodesInGivenLayer[noOfLayers - 1] = 3;	//Last layer					

	char filename[] = "irisReady.txt";
	int noOfClasses = 3;
	int noOfInputParameters = 4;
	int totalNoOfInstances = 150;

	GA* ga = new GA(noOfLayers, ArrayOfNoOfNodesInGivenLayer, popSize, tournamentSize, mutationRate, range);

	ga->readData(filename, noOfClasses, noOfInputParameters, totalNoOfInstances);
//	ga->printData();
//	ga->calculateFitness(&ga->ArrayOfNeuralNetworks[0]);
	

	

	int* Pop = new int[popSize];
	for (int j = 0; j < popSize; j++)
	{
		Pop[j] = j;
	}

	ga->calculateAllFitnesses();	//This line is must because evolve() requires that all fitnesses be calculate before it is called.
	
	for (int i = 0; i < noOfGenerations; i++)
	{
//		cout << "Generation no:\t" << i << endl;


//		int theFittest = ga->getLocalFittest(Pop, popSize);
//		cout << "The fittest is individual number\t" << theFittest << endl; 
//		cout << "Fitness of the fittest =\t" << ga->Fitnesses[theFittest] << endl;
//		ga->calculateFitnessAndPrint(&ga->ArrayOfNeuralNetworks[theFittest]);

		if (i % 10 == 0)
		{
		
			cout << "Generation no:\t" << i << endl;			

//			int theFittest = ga->getLocalFittest(Pop, popSize);
//			cout << "The fittest is individual number\t" << theFittest << endl; 
//			cout << "Fitness of the fittest =\t" << ga->Fitnesses[theFittest] << endl;
//			ga->calculateFitnessAndPrint(&ga->ArrayOfNeuralNetworks[theFittest]);	
		}


		//for (int i = 0; i < ga->ArrayOfNeuralNetworks[ga->getLocalFittest(Pop, popSize)].noOfEdges; i++)
		if (i % 100 == 0)
		{
			cout << "Time for 100 generations: " << (time(NULL) - previousMillis) << " seconds" << endl;
			previousMillis = time(NULL);
			
			int tf = ga->getLocalFittest(Pop, popSize);
			NN asd = ga->ArrayOfNeuralNetworks[tf];
			
			for (int i = 0; i < asd.noOfEdges; i++)
			{	
				cout << "Weight at edge no\t" << i << " =\t" << asd.Weights[i] << endl;
			}
			
			for (int i = 0; i < asd.noOfNodes; i++)
			{
				cout << "Bias at node no\t\t" << i << " =\t" << asd.Biases[i] << endl;
			}
			
			int theFittest = ga->getLocalFittest(Pop, popSize);
			cout << "The fittest is individual number\t" << theFittest << endl;
			cout << "Fitness of the fittest =\t" << ga->Fitnesses[theFittest] << endl;
			ga->calculateFitnessAndPrint(&ga->ArrayOfNeuralNetworks[theFittest]);
			
			
//			int theFittest = ga->getLocalFittest(Pop, popSize);
//			cout << "The fittest is individual number\t" << theFittest << endl; 
//			cout << "Fitness of the fittest =\t" << ga->Fitnesses[theFittest] << endl;
//			ga->calculateFitnessAndPrint(&ga->ArrayOfNeuralNetworks[theFittest]);
//
//			cout << "Generation no:\t" << i << endl;
		}

		ga->evolve();
		ga->calculateAllFitnesses();
	}


	system ("pause");

	return 0;
}

/*										
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
															Change Log (Stack)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

7. 2 March 2019: A lot of comments have been cleaned.


6. 2 March 2019: Biases are now at nodes instead of at being edges.

This change is also done in NN constructor.

Mutation values for weights are rand() % 10 and for biases are rand() % 5000.
For biases, values from rand() % 10 to rand() % 5000 work somehow for the followimg parameters.

Range: 1,00,000
Structure: 2 x 4 x 4 x 2
Pop size: 2000
Tournament size = 1%
Mutation rate = 1800 / 10,000 (18%)
Random initialization of weights and biases: -100 to +100
Mutation values:
-10 to +10 for weights
-5000 to +5000 for biases

HIGH = 5000
LOW = 1000
XOR Gate


5. Range Reduced 		~24 Feb 2019		Log writing forgotten, this change added later at 3 March 2019

Still works fine.

Main reason to reduce range is unconfirmed but highly suspected problems caused by integer overflow.

Range: 100000


4. 24 Feb 2019:	Random Initialization and some other things

Values of weights and biases initialized with -100 to +100.
NOTE: The above code is only in initialize() function. NN constructor has not been modified yet. ALSO note that the NN constructor is
	  not used in GA constructor where initialization takes place and does so through NN class' initialize() function. Therefore, 
	  not modifying the NN constructor does not have any negative effects as of now.  

Mutation values are: +- rand() % 20.

Some other changes such as:

	1. Calculating fitness by subtracting from INT_MAX / 10 instead of dividing.
	2. Function to print actual output against the expected one has been modified and now also shows the difference.
	3. Maybe there are some other changes as well.

Following parameters have been found to be working:

Range: 99999999
Structure: 2 x 4 x 4 x 2
Pop size: 2000
Tournament size = 1%
Mutation rate = 1800 / 10,000 (18%)
Random initialization of weights and biases: -100 to +100
Mutation values: -20 to +20
HIGH = 5000
LOW = 1000
XOR Gate


3. 23 Feb 2019:	ReLu added.

All negative values become 0 while positive values remain the same.

HOWEVER, Random Initialization not done yet.


2. From Initializing_With_Zero - Copy:				23 Feb 2019

The following parameters work:

Range = 99999999
Structure: 2 x 4 x 4 x 2
theValueToDivideTheRangeBy = 10000000;	//Produces these values for mutation: 0, -9
Pop size: 2000
Tournament size = 10%
Mutation rate = 2000 / 10,000 (20%)
HIGH = 5000
LOW = 1000
XOR Gate


1.From Initializing_With_Zero - Copy:				23 Feb 2019

Averaging instead of Summing: All values recieved from the nodes of previous layers are averaged instead of being summed together.


*/
