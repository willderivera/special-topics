#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"

using namespace std;

//method changes given vector to next permutation with each index
//given a value from 1 to colors inclusive
bool increment(vector<int> &v, int colors) {
	//find first index to increment
	for (int i = 0; i < v.size(); i++) {
		if (v[i] < colors) {
			v[i--]++;
			//reset previous index from colors to 1
			for (i; i >= 0; i--) {
				v[i] = 1;
			}
			//returns that there are more permutations
			return true;
		}
	}
	//retunrs that v is at final permutation
	//every index has value colors
	return false;
}

//method finds coloring of graph to minimize conflicts
//returns number of conflicts
int exhaustiveColoring(graph &g, int numColors, int t) {
	//vector to hold answer with least conflicts seen so far
	//initially ever node is color 1
	vector<int> bestAnswer(g.numNodes(), 1);
	//vector to hold answer currently being tested
	//also set to all color 1
	vector<int> currentAnswer = bestAnswer;
	//int to hold number of conflicts in bestAnswer
	//initialized to max number of cnflicts for given graph
	int conflicts = g.numEdges();
	//initilize starting time
	double startTime = (double) (clock() / CLOCKS_PER_SEC);

	//while time elapsed is within given time
	while ( (double)(clock() / CLOCKS_PER_SEC) - startTime < t) {
		//change graph to have coloration of currentAnswer
		for (int i = 0; i < currentAnswer.size(); i++) {
			g.setColor(i, currentAnswer[i]);
		}
		//if current graph is asgood as or better than best graph
		//set best graph to current graph
		if (g.numConflicts() <= conflicts) {
			conflicts = g.numConflicts();
			bestAnswer = currentAnswer;
		}
		//break if all permutations of colors have been tested
		//algorithm is done
		if (!increment(currentAnswer, numColors)) {
			break;
		}
	}
	//set coloration of graph to best rsult
	for (int i = 0; i < bestAnswer.size(); i++) {
		g.setColor(i, bestAnswer[i]);
	}
	return conflicts;
}

//unmodified natural greedy algortihm
//
int naturalGreedyColoring(graph &g, int numColors, int t) {
	//vector to hold answer with least conflicts seen so far
	//initially ever node is color 0
	//can be usd in testing
	//vector<int> answer(g.numNodes(), 0);

	//iterate over all nodes
	for (int n = 0; n < g.numNodes(); n++) {
		//can be used in testing
		//answer[n] = g.getfirstAvailColor(n, numColors);
		
		//pick color with fewest conflicts and assign to node
		g.setColor(n,  g.getfirstAvailColor(n, numColors));
	}
	return g.numConflicts();
}

//method takes a graph and finds the best local optimum using 2-opt neighbors
//returns number of coflicts
int twoOptSteepestDecent(graph &g, bool repeat, int numColors) {
	double startTime = (double) (clock() / CLOCKS_PER_SEC);
	graph prevBest;

	while ( (double)(clock() / CLOCKS_PER_SEC) - startTime < 600) {
		do {
			if (repeat) {
				g.randomize(numColors);
			}
			prevBest = g;
			g = prevBest.twoOptNeighbor(600 + startTime);
			if ( (double)(clock() / CLOCKS_PER_SEC) - startTime > 600) {
				break;
			}
		}
		while (prevBest.numConflicts() > g.numConflicts());
		if (!repeat) {
			break;
		}
	}
	return g.numConflicts();
}

//method takes a graph and finds the best local optimum using 3-opt neighbors
//returns number of coflicts
int threeOptSteepestDecent(graph &g, bool repeat, int numColors) {
	double startTime = (double) (clock() / CLOCKS_PER_SEC);
	graph prevBest;
	while ( (double)(clock() / CLOCKS_PER_SEC) - startTime < 600) {
		do {
			if (repeat) {
				g.randomize(numColors);
			}
			prevBest = g;
			g = prevBest.threeOptNeighbor(600 + startTime);
			if ( (double)(clock() / CLOCKS_PER_SEC) - startTime > 600) {
				break;
			}
		}
		while (prevBest.numConflicts() > g.numConflicts());
		if (!repeat) {
			break;
		}
	}
	return g.numConflicts();
}

bool eq(graph &g1, graph &g2) {
	if (g1.numNodes() != g2.numNodes()) {
		return false;
	}
	for (int n = 0; n < g1.numNodes(); n++) {
		if(g1.getColor(n) != g2.getColor(n)) {
			return false;
		}
	}
	return true;
}

int main() {
	char x;
	ifstream fin;
	ofstream fout;
	stack<int> moves;
	string filename;
	int numColors;
	vector<int> solution;

	//Read the name of the graph from the keyboard or
	//hard code it here for testing.

	//filename = "color12-4.input";
	string filenames[] = {"color12-3", "color12-4", "color24-4", "color24-5",
		"color48-5", "color48-6", "color96-6", "color96-7",
		"color192-6", "color192-7" ,"color192-8"};

	/*
	cout << "Enter file name" << endl;
	cin >> filename;
	*/

	//fin.open(filename.c_str());
	for (int i = 6; i < 11; i++) {
		fin.open( (filenames[i] + ".input").c_str() );
		fout.open( (filenames[i] + ".output").c_str() );
		if(!fin) {
			cerr << "Cannot open " << filename << endl;
			exit(1);
		}

		try {
			cout << "Reading number of colors" << endl;
			fin >> numColors;

			cout << "Reading graph" << endl;
			graph g(fin);

			cout << "Num colors: " << numColors << endl;
			cout << g;

			//exhaustiveColoring(g, numColors, 600);
			naturalGreedyColoring(g, numColors, 600);

			graph g1 = g;
			graph g2 = g;
			graph g3 = g;
			graph g4 = g;
			int c1, c2, c3, c4;
			c1 = twoOptSteepestDecent(g1, false, numColors);
			c2 = threeOptSteepestDecent(g2, false, numColors);
			c3 = twoOptSteepestDecent(g3, true, numColors);
			c4 = threeOptSteepestDecent(g4, true, numColors);
			
			if (c1 <= c2 && c1 <= c3 && c1 <= c4) {
				fout << "greedy initial solution followed by steepest descentwith 2-Opt"<<endl;
				g1.printSolution(fout);
			}
			else if (c2 <= c3 && c2 <= c4) {
				fout << "greedy initial solution followed by steepest descentwith 3-Opt"<<endl;
				g2.printSolution(fout);
			}
			else if (c3 <= c4) {
				fout << "random initial solution followed by steepest descent with 2-Opt with multiple restarts"<<endl;
				g3.printSolution(fout);
			}
			else {
				fout << "random initial solution followed by steepest descent with 3-Opt with multiple restarts"<<endl;
				g4.printSolution(fout);
			}

		}
		catch(indexRangeError &ex) {
			cout << ex.what()<< endl; exit(1);
		}
		catch (rangeError &ex) {
			cout << ex.what()<< endl; exit(1);
		}
		fin.close();
		fout.close();
		//system("pause");
	}
	system("pause");
}