// Project 5: Knapsack
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>
#include <time.h>

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"
#include "knapsack.h"

using namespace std;

void neighborhood2Opt(knapsack &k, int maxTime)
//Neighborhood: One object's included status flipped
{
	knapsack current(k);

//	int bestValue = 0;
	int prevBestValue = 0;

	bool newSolution = true;

	clock_t start = clock()/CLOCKS_PER_SEC;
	clock_t finish = start + maxTime;

	while(newSolution && clock()/CLOCKS_PER_SEC < finish)
	{
		newSolution = false;

		for(int i = 0; i < k.getNumObjects() - 1; i++)
		{
			current = k;
			if(current.isSelected(i))
			{
				current.unSelect(i);
			}
			else
			{
				current.select(i);
			}

			if(current.getValue() <= current.getCostLimit() && current.getValue() > k.getValue())
			{
				k = current;
				newSolution = true;
			}
		}
	}
}

void neighborhood3Opt(knapsack &k, int maxTime)
// Neighborhood: two object's included status changed
{
	knapsack current(k);

//	int bestValue = 0;
	int prevBestValue = 0;

	bool newSolution = false;

	clock_t start = clock()/CLOCKS_PER_SEC;
	clock_t finish = start + maxTime;

	while(newSolution && clock()/CLOCKS_PER_SEC < finish)
	{
		newSolution = false;

		for(int i = 0; i < k.getNumObjects() - 1; i++)
		{
			current = k;
			if(current.isSelected(i))
			{
				current.unSelect(i);
			}
			else
			{
				current.select(i);
			}
			for(int j = 0; j < k.getNumObjects() - 1; j++)
			{
				if(current.isSelected(j))
				{
					current.unSelect(j);
				}
				else
				{
					current.select(j);
				}

				if(current.getValue() <= current.getCostLimit() && current.getValue() > k.getValue())
				{
					k = current;
					newSolution = true;
				}

				if(current.isSelected(j))
				{
					current.unSelect(j);
				}
				else
				{
					current.select(j);
				}
			}
		}
	}
}


void randomKnapsack(knapsack &k, int maxTime)
{
//	int bestValue = 0;

	clock_t start = clock()/CLOCKS_PER_SEC;
	clock_t finish = start + maxTime;

	while(clock()/CLOCKS_PER_SEC < maxTime)
	{
		knapsack current(k);

		for(int i = 0; i < current.getNumObjects() - 1; i++)
		{
			srand(time(NULL));

			if(rand() % 2)
			{
				current.select(i);
			}
			else
			{
				current.unSelect(i);
			}

			if(current.getCost() > current.getCostLimit())
			{
				current.unSelect(i);
			}
		}

		if (current.getValue() > k.getValue())
		{
			k = current;
		}
	}
}

void greedyKnapsack(knapsack &k)
//greedy algorithm to solve knapsack
//uses the ratio of value/cost to select objects
{
	int numObjects = k.getNumObjects();

	vector<double> r;
	vector<int> order;

	//Loop through each object to find it's ratio of value/cost
	for(int i = 0; i < numObjects; i++)
	{
		double ratio =  ((double)k.getValue(i)/(double)k.getCost(i));
		r.push_back(ratio);
	}

	//Find the order of indices by largest ratio, and place them
	//in a vector
	for(int i = 0; i < numObjects; i++)
	{
		int bestIndex = 0;
		for(int j = 0; j < numObjects; j++)
		{
			if(r[j] >= r[bestIndex])
			{
				bestIndex = j;
			}
		}
		order.push_back(bestIndex);
		r.at(bestIndex) = -1;
	}

	//add each object in order of ratio
	//if cost exceeds cost limit, remove the object
	for(int i = 0; i < numObjects; i++)
	{
		k.select(order[i]);
		
		if(k.getCost() > k.getCostLimit())
		{
			k.unSelect(order[i]);
		}
	}
}

int main()
{
   char x;
   ifstream fin;
   ofstream fout;
   stack <int> moves;
   string fileName, outputFileName;
   vector<bool> solution;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack512.input";
//   outputFileName = "knapsack8.output";

   fout.open(outputFileName.c_str());

   //   cout << "Enter filename" << endl;
   //   cin >> fileName;
   
   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      cout << "Reading knapsack instance" << endl;
      knapsack k(fin);
	  cout << k;

      //greedyKnapsack(k);
	  randomKnapsack(k, 60);

	  neighborhood2Opt(k, 600);
	  //neighborhood3Opt(k, 600);

	  k.printSolution();
   }

   catch (indexRangeError &ex) 
   { 
      cout << ex.what() << endl; //exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; //exit(1);
   }

   system("PAUSE");
}