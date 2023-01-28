#include <iostream>
#include <fstream>
using namespace std;

class State {

private://State class designed to store states

	string name;

public:

	State(string n)
	{
		name = n;
	}

	string getName()
	{
		return name;
	}
};

class FSM {

private://private members required for FSM to function

	State* presentState;
	State** stateList;
	string* Inputs;
	int noOfStates;
	int noOfInputs;

public:

	FSM()
	{
		Inputs = NULL;
		presentState = NULL;
		stateList = NULL;
		noOfStates = 0;
		noOfInputs = 0;
	}

	void LoadStates()
	{//loads all states and inputs from text file and stores them in class members
		int i;
		State** templist = new State* [50];
		string* tempinputs = new string [50];
		string temp;
		ifstream fin("statedata.txt");
		if (fin.is_open())
		{
			while (!fin.eof())
			{
				fin >> temp;
				if (stateCheck(templist, temp) == false)
				{
					templist[noOfStates] = new State(temp);
					noOfStates++;
				}
				fin >> temp;
				if (fin.eof())
				{
					break;
				}
				if (inputCheck(tempinputs, temp) == false)
				{
					tempinputs[noOfInputs] = temp;
					noOfInputs++;
				}
				fin >> temp;
				if (stateCheck(templist, temp) == false)
				{
					templist[noOfStates] = new State(temp);
					noOfStates++;
				}

			}
			stateList = new State * [noOfStates];
			Inputs = new string[noOfInputs];
			for (i = 0; i < noOfStates; i++)
				stateList[i] = templist[i];
			for (i = 0; i < noOfInputs; i++)
				Inputs[i] = tempinputs[i];
			delete[] templist;
			delete[] tempinputs;
			presentState = stateList[0];
		}
		else 
		{
			cout << "Data file could not be opened.\n";
		}
		fin.close();
	}

	bool stateCheck(State** list, string temp)
	{//checks if state is already present(as to ensure no duplicate state is stored)
		int i;
		bool check = false;
		string name;
		for (i = 0; i < noOfStates; i++)
		{
			name = list[i]->getName();
			if (list[i]->getName() == temp)
			{
				check = true;
			}
		}
		return check;
	}

	bool inputCheck(string* list, string temp)
	{//checks if input is already present(as to ensure no duplicate input is stored)
		int i;
		bool check = false;
		for (i = 0; i < noOfStates; i++)
		{
			if (list[i] == temp)
			{
				check = true;
			}
		}
		return check;
	}

	bool stateTransition(string inp,string* states, string* inputs,int& count)
	{//checks if transition of states is possible depending on current state and input
		int i;
		string tempstate, tempinput;
		ifstream fin("statedata.txt");
		while (!fin.eof())
		{
			fin >> tempstate;
			fin >> tempinput;
			if (fin.eof())
			{
				break;
			}
			if (tempstate == presentState->getName() && tempinput == inp)
			{
				inputs[count] = tempinput;
				states[count] = tempstate;
				count++;
				cout << "\nString is accepted by the FSM.\n";
				fin >> tempstate;
				cout << "State transitioned from: " << presentState->getName() << " to " << tempstate <<"\n";
				for (i = 0; i < noOfStates; i++)
				{
					if (tempstate == stateList[i]->getName())
					{
						presentState = stateList[i];
						states[count] = tempstate;
						return true;
					}
				}
			}
			else 
			{
				fin >> tempstate;
			}
		}
		cout << "\nString is rejected by the FSM.\n";
		cout << "State does not transition\n";
		return false;

	}

	void displayFSM()
	{//displays current state as to make FSM more uservfriendly
		cout << "\nPresent State: \n\n\t" << presentState->getName() << "\n\n";
	}

	void typeCheck()
	{//checks type of FSM( either DFA or NFA)
		int i, j, count = 0;
		string tempstate, tempinput;
		for (i = 0; i < noOfStates; i++)
		{
			for (j = 0; j < noOfInputs; j++)
			{

				ifstream fin("statedata.txt");
				while (!fin.eof())
				{
					fin >> tempstate;
					fin >> tempinput;
					if (fin.eof())
					{
						break;
					}
					if (stateList[i]->getName() == tempstate && Inputs[j] == tempinput)
					{
						count++;
					}
					fin >> tempstate;
				}
				if (count > 1)
				{
					cout << "Given FSM type is NFA.\n\n";
					return;
				}
				count = 0;
			}
		}
		cout << "Given FSM type is DFA.\n\n";
	}

	void Run()
	{//runs FSM until user exits
		typeCheck();
		int i, count = 0;
		char decision = 'Y';
		string* totalinputs = new string[50];
		string* totalstates = new string[50];
		while (decision == 'Y')
		{
			string inp;
			displayFSM();
			cout << "Enter FSM input: ";
			cin >> inp;
			stateTransition(inp,totalstates,totalinputs,count);
			cout << "Would you like to enter another string? Y/N: ";
			cin >> decision;
			while (decision != 'Y' && decision != 'N')
			{
				cout << "Would you like to enter another string? Y/N: ";
				cin >> decision;
			}
		}
		cout << "\n\nInput string: ";
		for (i = 0; i < count; i++)
		{
			cout << totalinputs[i];
		}
		cout << "\n\nTotal state transitions: ";
		for (i = 0; i <= count; i++)
		{
			if (i == count)
			{
				cout << totalstates[i];
				break;
			}
			cout << totalstates[i] << " > ";
			
		}
		delete[] totalinputs;
		delete[] totalstates;
		cout << "\n\nExiting FSM.\n";
		
	}
};

int main()
{
	FSM* test = new FSM();
	test->LoadStates();
	test->Run();
}