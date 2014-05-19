// Project 2a: Solving knapsack using greedy algorithm
//
// Brian Rose rose.bri@husky.neu.edu
// Will de Rivera derivera.w@husky.neu.edu
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <cmath>
#include <map>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"
#include "knapsack.h"


void exhaustiveKnapsack(knapsack &k, int t)
// Exhaustive algorithm to solve knapsack problem
{
	clock_t start, end, curr;
	start = clock()/CLOCKS_PER_SEC;	// initialize start time
	end = start + t;	// calculate end time

	int currBest = 0;

	int numObjects = k.getNumObjects();
	int costLimit = k.getCostLimit();

	int select = 1;	// a possible solution, start at first possible solution

	curr = clock()/CLOCKS_PER_SEC;
	while(curr < end && (select <= pow(2.0, numObjects)))
	{
		
		int currVal = 0;
		int currCost = 0;

		// find current possible solution's cost and value
		for(int i = 0; i < numObjects; i++)
		{
			if((select & ( 1 << i )) >> i)	// logic to determine which objects
											// to select
			{
				currVal = currVal + k.getValue(i);
				currCost = currCost + k.getCost(i);
			}
		}

		// if this solution's value is greater than the current best solution's
		// value, and it is a legal solution (cost less than limit)
		// select the correct values with the knapsack
		if((currVal > currBest) && (currCost <= costLimit))
		{
			for(int i = 0; i < numObjects; i++)
			{
				k.unSelect(i);
			}
			for(int i = 0; i < numObjects; i++)
			{
				if((select & ( 1 << i )) >> i)	// logic to determine which
												// objects to select
				{
					k.select(i);
				}
			}
			currBest = k.getValue();
		}
		select++;	// increment possible solution
		curr = clock()/CLOCKS_PER_SEC;	// get current time
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

//void greedyKnapsack2(knapsack &k)
////greedy algorithm to solve knapsack
////uses the highest legal value left to select objects
//{
//	int numObjects = k.getNumObjects();
//
//	vector<double> r;
//	vector<int> order;
//
//	for(int i = 0; i < numObjects; i++)
//	{
////		double ratio =  ((double)k.getValue(i)/(double)k.getCost(i));
//		r.push_back(k.getValue(i));
//	}
//	for(int i = 0; i < numObjects; i++)
//	{
//		int bestIndex = 0;
//		for(int j = 0; j < numObjects; j++)
//		{
//			if(r[j] >= r[bestIndex])
//			{
//				bestIndex = j;
//			}
//		}
//		order.push_back(bestIndex);
//		r.at(bestIndex) = -1;
//	}
//
//	for(int i = 0; i < numObjects; i++)
//	{
//		k.select(order[i]);
//		
//		if(k.getCost() > k.getCostLimit())
//		{
//			k.unSelect(order[i]);
//		}
//	}
//}

int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
    fileName = "knapsack1024.input";

 //  cout << "Enter filename" << endl;
 //  cin >> fileName;
   
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
//	  knapsack l(k);

//      exhaustiveKnapsack(k, 600);
	  greedyKnapsack(k);
//	  greedyKnapsack2(l);

      cout << endl << "Best solution" << endl;
      k.printSolution();
	  cout << endl;
//	  l.printSolution();
      
   }    

   catch (indexRangeError &ex) 
   { 
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }

   system("PAUSE");
}

