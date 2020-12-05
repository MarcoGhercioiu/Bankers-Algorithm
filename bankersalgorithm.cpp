/*
File: main.cpp
Author: Vlad
Procedures:
	main 				- opens the text file, reads in the number of processes and resources, gets the resource quantity array,
				calls function to make claims and allocation matrices, calls function to calculate the needs matrix, and
				calls function to determine the state of the system.
	readLine 			- returns line from input file stream.
	splitString 		- tokenize a line and create array of integers found on line.
	getResources 		- get next line in text file and return an array created by splitString
	makeProcessVector 	- creates 2d vector from values placed into an array using split string.
	calcNeed 			- returns a 2d vector, which is the summation of the claims 2d vector minus the allocation 2d vector.
	safe 				- checks if system is safe, if unsafe then prints message, if safe then prints message and safe sequence
				of execution for processes
*/



#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>


using namespace std;



/*
* string readLine(ifstream* text)
* Author: Vlad
* Date: 16 October 2020
* Description: Returns the next line from input file stream.
*
* Parameters:
* text 			I/P 	ifstream* 			pointer to text file stream
* readLine 		O/P 	string 				Status code (not currently used)
*/

string readLine(ifstream* text){
	string line;
// pull line from text file
	getline(*text, line);
	return line;
}

/*
* int * splitString(string line, string splitter, int numTokens)
* Author: Vlad
* Date: 16 October 2020
* Description: Returns an array that contains all the integer values on the string passed in,
* 				gets all the integers by finding the position a delimiter.
*
* Parameters:
* line 			I/P 	string 			a string taken from the text file
* splitter		I/P		string			the delimiter taken to find the integer values on the line.
* numTokens		I/P		int				the number of integers in the string
* splitString 	O/P 	int* 			Status code (not currently used)
*/

int * splitString(string line, string splitter, int numTokens) {
// create token array
	int * tokens = new int[numTokens];

	int index = 0;
	int position = 0;
	string token = "";
// loop through integers within string
	while(index < numTokens) {
		position = line.find(splitter);
		tokens[index] = stoi(line.substr(0, position));
		line.erase(0, position + splitter.length());
		index++;
	}

	return tokens;
}

/*
* int * getResources(ifstream* text, int numR)
* Author: Vlad
* Date: 16 October 2020
* Description: Reads line from text file and returns the resulting resource array
* 				from splitString function.
*
* Parameters:
* text 			I/P 	ifstream* 		pointer to text file stream
* numR			I/P		int				number of resources
* getResources 	O/P 	int* 			Status code (not currently used)
*/

int * getResources(ifstream* text, int numR){
	string resourceVal = readLine(text);
	return splitString(resourceVal, "\t", numR);
}

/*
* vector<vector<int>> makeProcessVector(ifstream * text, int numP, int numR)
* Author: Vlad
* Date: 16 October 2020
* Description: Returns a 2d vector filled with values taken from the text file,
* 				values are loaded into array through the splitString function
* 				and distributed in the 2d vector
*
* Parameters:
* text 					I/P 	ifstream * 					pointer to text file stream
* numP					I/P		int							the number of processes
* numR					I/P		int							the number of resources
* makeProcessVector 	O/P 	vector<vector<int>>			Status code (not currently used)
*/

vector<vector<int>> makeProcessVector(ifstream * text, int numP, int numR) {
	vector<vector<int>> processVector;


// add process totals to process vector
	for(int i = 0; i < numP; i++){
		string processVals = readLine(text);

		int * arr = splitString(processVals, "\t", numR);

		vector<int> tempVect;

		for(int j = 0; j < numR; j++){
		tempVect.push_back(arr[j]);
		}

		processVector.push_back(tempVect);

	}

	return processVector;
}

/*
* vector<vector<int>> calcNeed(vector<vector<int>> claims, vector<vector<int>> allocation, int numP, int numR)
* Author: Vlad
* Date: 16 October 2020
* Description: calculate the 2d vector for the resources needed by subtracting the claims from the allocation
*
* Parameters:
* claims				I/P 	vector<vector<int>> 		2d vector of process claims
* allocation			I/P		vector<vector<int>>			2d vector of process allocation
* numR					I/P		int							the number of resources
* numP					I/P		int							the number of processes
* calcNeed			 	O/P 	vector<vector<int>>			Status code (not currently used)
*/


vector<vector<int>> calcNeed(vector<vector<int>> claims, vector<vector<int>> allocation, int numP, int numR){

	vector<vector<int>> need;
	vector<int> tempVect;
// iterate through claims and allocation 2d vectors and calculate the needs
	for(int i = 0; i < numP; i++){
		for(int j = 0; j < numR; j++){
			tempVect.push_back((claims[i][j] - allocation[i][j]));
		}
		need.push_back(tempVect);
		tempVect.clear();
	}
	return need;
}

/*
* bool safe(int numP, int numR, int * resources, vector<vector<int>> claims, vector<vector<int>> allocation, vector<vector<int>> need)
* Author: Vlad
* Date: 16 October 2020
* Description: computes whether the system is in a safe or unsafe state by determining if there is enough resources to allocate to
* 				a process
*
* Parameters:
* numR					I/P		int							the number of resources
* numP					I/P		int							the number of processes
* resources				I/P		int *						pointer to array of resources
* claims				I/P 	vector<vector<int>> 		2d vector of process claims
* allocation			I/P		vector<vector<int>>			2d vector of process allocation
* need					I/P 	vector<vector<int>> 		2d vector of needs
* safe				 	O/P 	bool						Status code (not currently used)
*/

bool safe(int numP, int numR, int * resources, vector<vector<int>> claims, vector<vector<int>> allocation, vector<vector<int>> need){

	bool finished[numP] = {false};

	int safeSeq[numP];

	int work[numR];
// make a copy of resource matrix
	for(int i = 0; i < numR; i++)
		work[i] = resources[i];

// remove allocated resources from available resources
	int availableResources[numR];
	for(int i = 0; i < numR; i++) {
		availableResources[i] = work[i];
		for(int p = 0; p < numP; p++) {
			availableResources[i] -= allocation[p][i];
		}
	}



	int count = 0;

// loop through all processes
	while(count < numP){

		bool found = false;

// loop through processes again
		for(int p = 0; p < numP; p++){

// if a processes are not finished check if resources are available
			if(finished[p] == false){
				bool resourcesAvailable = true;
				for(int j = 0; j < numR; j++){
					if(need[p][j] > availableResources[j]) {
						resourcesAvailable = false;
						break;
					}
				}

// if there are enough resources to satisfy process p the add the resources back to pool of  available resources
				if(resourcesAvailable){
					for(int k = 0; k < numR; k++){
						availableResources[k] += allocation[p][k];
					}

					safeSeq[count] = p;
					count++;

					finished[p] = true;

					found = true;
					break;
				}
			}
		}

		if(found == false){
			cout << "System is not in safe state" << endl;
			return false;
		}


	}

	cout << "System is in safe state" << endl;
	cout << "process sequence is: ";
	for(int i = 0; i < numP; i++)
		cout << safeSeq[i] << " ";

	return true;
}

/*
* int main(int argc, char ** argv)
* Author: Vlad
* Date: 16 October 2020
* Description: opens the text file, generates the resource array, claims 2d vector, allocation 2d vector, and needs 2d vector,
* 				then calls the safe function
*
* Parameters:
*  argc		I/P		int		number of arguments passed
*  argv		I/P		char **		pointer array, pointing to each argument passed
*  main		O/P		int		Status code (not currently used)
*/

int main(int argc, char ** argv){

	ifstream text;
	string line;
	string filename = "Input.txt";

	if(argc == 2)
		filename = argv[1];

	text.open(filename);

	if(text.fail() == true){
		cout << "file was unable to open" << endl;
		return 1;
	}

// get number of resources
	int numR = stoi(readLine(&text));
// create resource array
	int * resources = getResources(&text, numR);
// get number of processes
	int numP = stoi(readLine(&text));
// create 2d vectors
	vector<vector<int>>  claims = makeProcessVector(&text, numP, numR);
	vector<vector<int>> allocation = makeProcessVector(&text, numP, numR);
	vector<vector<int>> need = calcNeed(claims, allocation, numP, numR);

// check if system is in safe or unsafe state
	safe(numP, numR, resources, claims, allocation, need);
	cout << endl;
	text.close();
}


