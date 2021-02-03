#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
using namespace std;	//Play it safe kids.

enum elevatorDirection
{
	UP, DOWN
};

class Node
{
public:

	int val;
	
	Node(int val)
	{
		this->val = val;
	}
	
	Node* next = NULL;
	Node* prev = NULL;
};

class ElevatorQueue
{
public:

	Node* front = NULL;
	Node* back = NULL;

	ElevatorQueue()
	{
		front = NULL;
		back = NULL;
	}

	bool isEmpty()
	{
		if (NULL == front && NULL == back)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int peek()
	{
		if (isEmpty())
		{
			return 0;
		}
		else
		{
			return front->val;
		}
	}

	bool doesContain(int val)
	{
		Node* currentNode = back;

		while (NULL != currentNode)
		{
			if (val == currentNode->val)
			{
				return true;
			}
			currentNode = currentNode->next;
		}

		return false;
	}

	void moveUpTheQueue(Node* currentNode)
	{
		if (NULL != currentNode->next)	//Safety check
		{
			int currentVal, initiallyNextVal;
			currentVal = currentNode->val;
			initiallyNextVal = currentNode->next->val;

			currentNode->val = initiallyNextVal;
			currentNode->next->val = currentVal;
		}
	}

	void moveDownTheQueue(Node* currentNode)
	{
		if (NULL != currentNode->prev)	//Safety check
		{
			int currentVal, initiallyPreviousVal;
			currentVal = currentNode->val;
			initiallyPreviousVal = currentNode->prev->val;

			currentNode->val = initiallyPreviousVal;
			currentNode->prev->val = currentVal;
		}
	}

	bool enqueue(int val, int elevatorDirection, int currentElevatorFloor)
	{
		if (isEmpty())
		{
			Node* newNode = new Node(val);
			front = newNode;
			back = newNode;

//			front->prev = back;		//DO NOT USE this line
//			back->next = front;		//DO NOT USE this line

			front->next = NULL;	//Redundant
			back->prev = NULL;	//Redundant
		}
		else
		{
			if (!doesContain(val))
			{
				Node* newNode = new Node(val);

				newNode->next = back;
				back->prev = newNode;

				back = newNode;

				if (elevatorDirection == UP && val > currentElevatorFloor)
					{
					Node* currentNode = back;
					while (NULL != currentNode->next
					&& currentNode->val > currentNode->next->val
					//&& currentNode->val > currentElevatorFloor
					&& currentNode->next->val < currentElevatorFloor)
					{
//						cout << "Here Up 1 A" << endl;
						moveUpTheQueue(currentNode);
						currentNode = currentNode->next;
					}

					while (NULL != currentNode->next && currentNode->val < currentNode->next->val)
					{
//						cout << "Here Up 1 B" << endl;
						moveUpTheQueue(currentNode);
						currentNode = currentNode->next;
					}
				}

				if (elevatorDirection == UP && val < currentElevatorFloor)
				{
					Node* currentNode = back;
					while (NULL != currentNode->next
					&& currentNode->val > currentNode->next->val)
					{
//						cout << "Here Up 2 A" << endl;
						moveUpTheQueue(currentNode);
						currentNode = currentNode->next;
					}
				}

			//==================================================

				if (elevatorDirection == DOWN && val < currentElevatorFloor)
				{
					Node* currentNode = back;
					while (NULL != currentNode->next
					&& currentNode->val < currentNode->next->val
					&& currentNode->next->val > currentElevatorFloor)
					{
//						cout << "Here Down 1 A" << endl;
						moveUpTheQueue(currentNode);
						currentNode = currentNode->next;
					}

					while (NULL != currentNode->next && currentNode->val > currentNode->next->val)
					{
//						cout << "Here Down 1 B" << endl;
						moveUpTheQueue(currentNode);
						currentNode = currentNode->next;
					}
				}

				if (elevatorDirection == DOWN && val > currentElevatorFloor)
				{
					Node* currentNode = back;
					while (NULL != currentNode->next
					&& currentNode->val < currentNode->next->val)
					{
//						cout << "Here Down 2 A" << endl;
						moveUpTheQueue(currentNode);
						currentNode = currentNode->next;
					}
				}

				return true;
			}
			else
			{
//				cout << "Already contains the value\n";
				return false;
			}			
		}
	}

	int dequeue()
	{
		int val = 0;

		if (isEmpty())
		{
//			cout << "Empty Queue!" << endl;
			val = 0;	//Return to ground floor
		}
		else if (back == front)	//The queue contains only one element.
		{
//			cout << "Queue contains only One item" << endl;
			val = front->val;

			delete front;
			front = NULL;
			back = NULL;
		}
		else
		{
			val = front->val;

			Node* theNewFront = front->prev;
			delete front;
			front = NULL;
			front = theNewFront;
			front->next = NULL;
		}

		return val;
	}

	void print()
	{
		Node* currentNode = back;
		while (NULL != currentNode)
		{
			cout << currentNode->val << "\t";
			currentNode = currentNode->next;
		}
		cout << endl;
	}

	~ElevatorQueue()
	{
		while (!isEmpty())
		{
			dequeue();
//			cout << "Freeing : Dequeueing\n";
		}
	}
};

class Elevator
{
public:
	
	int weight;
	int currentFloor = 0;
	int direction;
	int destination = 0;
	
	int id;
	
	ElevatorQueue q;

	bool isBoardingAllowed = false;
	
	Elevator()
	{
		direction = UP;
		currentFloor = 0;
//		cout << "Elevator built\n";
	}
	
	void set(int id)
	{
		this->id = id;
		direction = UP;
		currentFloor = 0;
		cout << "Elevator " << id << " built.\n";
	}
	
	bool order(int floor)
	{
		return q.enqueue(floor, direction, currentFloor);
	}

	void insertTabs()
	{
		for (int i = 0; i < id; i++)
		{
			cout << "";
		}
	}

	void tick()
	{
		if (currentFloor != destination)
		{	
			isBoardingAllowed = false;
			insertTabs();
			cout << "Elevator " << id << " departing floor " << currentFloor << "";
			cout << " and going to " << destination << "\n";

			if (destination > currentFloor)
			{
				direction = UP;
				currentFloor ++;
			}
			else
			{
				direction = DOWN;
				currentFloor --;
			}
		}
		else
		{
			isBoardingAllowed = true;
			insertTabs();
			cout << "!!!\tElevator " << id << " free at floor " << currentFloor << "\t!!!\n";
			destination = q.dequeue();
		}
	}

	void addWeight(int personWeight)
	{
		weight += personWeight;
	}

	void subtractWeight(int personWeight)
	{
		weight -= personWeight;
	}

};

class ElevatorController
{
public:

	int noOfFloors;
	int noOfElevators;

	Elevator* BuildingElevators = NULL;

	std::queue<int> requestQueue;

	ElevatorController(int noOfElevators , int noOfFloors)
	{
		this->noOfFloors = noOfFloors;
		this->noOfElevators = noOfElevators;

		BuildingElevators = new Elevator[noOfElevators];

		for (int i = 0; i < noOfElevators; i++)
		{
			BuildingElevators[i].set(i);
		}
	}

	void request(int floor)
	{
		requestQueue.push(floor);
	}

	bool order()
	{
		if (requestQueue.empty())
		{
			return false;
		}

		int floor = requestQueue.front();
		requestQueue.pop();
		bool hasTheFloorBeenOrdered = false;

		for (int i = 0; i < noOfElevators; i++)
		{
			if (true == BuildingElevators[i].isBoardingAllowed)	//This is a standing elevator. Order it to service this floor.
			{
				//BuildingElevators[i].order(floor);
				
				//hasTheFloorBeenOrdered = true;
				//break;
				
				cout << "A standing elevator has been ordered to floor " << floor << ". Elevator no. " << i << "\n";
				return BuildingElevators[i].order(floor);;
			}
		}
		
		if (false == hasTheFloorBeenOrdered)
		{
			for (int i = 0; i < noOfElevators; i++)
			{
				if (BuildingElevators[i].currentFloor < floor && BuildingElevators[i].destination >= floor)	//This elevator
				//is moving towards this floor.
				{
					cout << "An elevator moving towards floor " << floor << " has been ordered a stop." << ". Elevator no. " << i << "\n";
					return BuildingElevators[i].order(floor);
				}
			}
		}

		if (false == hasTheFloorBeenOrdered)
		{
			int indexOfTheElevatorWithTheLowestLoad = 0;
			
			for (int i = 0; i < noOfElevators - 1; i++)
			{
				if (BuildingElevators[i].weight < BuildingElevators[i + 1].weight)
				{
					indexOfTheElevatorWithTheLowestLoad = i;
				}
			}

			cout << "The elevator with the lowest load has been ordered to floor " << floor << ". Elevator no. "
			<< indexOfTheElevatorWithTheLowestLoad << "\n";
			return BuildingElevators[indexOfTheElevatorWithTheLowestLoad].order(floor);
		}
	}

	void tick()
	{
		order();
		
		for (int i = 0; i < noOfElevators; i++)
		{
			BuildingElevators[i].tick();
		}
	}

	~ElevatorController()
	{
		delete[] BuildingElevators;
	}

};

int main(void)
{
	srand (time(NULL));

	int noOfElevators = 3;
	int noOfFloors = 100;
	ElevatorController e(noOfElevators, noOfFloors);

	while (1)
	{
		if (rand() % 1000 + 1 < 80)
		{
			int floor = rand() % 100 + 1;
			cout << "Requesting floor " << floor << "\n";
			e.request(floor);
		}

		e.tick();
		cout << "\n\n\n";
	}

	return 0;
}
