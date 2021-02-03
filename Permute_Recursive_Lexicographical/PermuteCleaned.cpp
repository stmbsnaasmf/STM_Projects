#include <iostream>
using namespace std;

char* permute(char* Arr, int size, char* PrevArr, int pSize)
{
	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			//char* prevChar = new char[size - (size - (pSize + 1))];
			char* prevChar = new char[1];
			*prevChar = Arr[i];
		
			char* nArr = new char[size - 1];
			int k = 0;
			for (int j = 0; j < size; j++)
			{
				if (Arr[j] != *prevChar)
				{
					nArr[k] = Arr[j];
					k++;
				}
			}
	
			char* PrevArrToBePassed = new char[pSize + 1];
		
			if (pSize != 0)
			{
				for (int j = 0; j < pSize; j++)		//(n-1) times
				{
					PrevArrToBePassed[j] = PrevArr[j];
				}
				PrevArrToBePassed[pSize] = *prevChar;		//nth time
			}
			else	//First time
			{
				PrevArrToBePassed[0] = *prevChar;
			}
			
			permute(nArr, size - 1, PrevArrToBePassed, pSize + 1);
			
			delete prevChar;
			delete[] nArr;
			delete[] PrevArrToBePassed;
		}
	}
	else
	{
		for (int i = 0; i < pSize; i++)
		{
			//cout << int(PrevArr[i]) << "\t";
		
			//cout << PrevArr[i];
		}
		//cout << int(Arr[0]);
		//cout << Arr[0];
		
		char* Crr = new char[pSize];
		
		for (int i = 0; i < pSize; i++)
		{
			Crr[i] = PrevArr[i];
		}
		
		for (int i = 0; i < pSize; i++)
		{
			cout << Crr[i];
		}
		cout << endl;
		
		delete[] Crr;
	}
	return Arr;
}

int main(void)
{
	char c[] = {'A', 'B', 'C', 'D'};
	int cSize = 4;
	
	permute(c, cSize, c, 0);		
	
	for (int i = 0; i < cSize; i++)
	{
		cout << c[i] << "\t";
	}
	cout << endl;
		
	return 0;
}
