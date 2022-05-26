#pragma once
#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;


struct DLLnode {
	DLLnode() {
		cnt++;
		prev = NULL;
		next = NULL;
		ele = NULL;
		airportName = "";
		down = NULL;
	}
	~DLLnode() { cnt--; }
	int ele;
	string airportName;
	static int cnt;
	DLLnode* prev, * next, * down;
};
int DLLnode::cnt = 0;


struct HashNode {
	HashNode() {
		cnt++;
		down = NULL;
		right = NULL;
		ele = NULL;
		countryName = "";

	}

	~HashNode() { ele--; }

	int ele;
	static int cnt;
	string countryName;
	HashNode* down;
	DLLnode* right;
};
int HashNode::cnt = 0;


HashNode* newHashNode(int ele);
DLLnode* newDLLnode(int ele);
void insertEnd(int ele, DLLnode*& dllHead);
DLLnode** locateThread(int ele);
void initHashNodes();
void display();
void deleteOne(int node);
void copyMap();
void addDestination();
bool authenticated();
void displayMap();


HashNode* hashHead = NULL;
DLLnode* dllHead = NULL;
bool isAuthenticated = false;
string password = "enter";
map<int, string> Airport_Codes;
map<string, int> Airport_Names;
int maxConnections = 10;
int connections = 100;



void copyMap() {

	for (map<int, string>::iterator iter = Airport_Codes.begin(); iter != Airport_Codes.end(); ++iter)
	{		Airport_Names.insert(pair<string, int>(iter->second, iter->first));}
}

void initNodes() {

	HashNode* hTemp = NULL;
	HashNode* hp = NULL;
	DLLnode* dTemp = NULL;
	DLLnode* dp = NULL;
	DLLnode* dpNextDown = NULL;
	HashNode* hpDown = NULL;

	for (int i = 1; i < maxConnections; i++) {//1-10
		if (i < maxConnections) {
			if (Airport_Codes.find(i) != Airport_Codes.end()) {

				hTemp = newHashNode(i);
				if (hashHead == NULL) {
					hashHead = hTemp;
					hpDown = hashHead;
				}
				else {
					hp = hashHead;
					for (; hp->down != NULL; hp = hp->down);
					hp->down = hTemp;
					hpDown = hp->down;//London, Berlin, Stockholm
				}


				for (int j = i * maxConnections + 1; j < (i + 1) * maxConnections; j++) {//11-20//21-30...
					if (Airport_Codes.find(j) != Airport_Codes.end()) {
						dTemp = newDLLnode(j);
						if (hpDown->right == NULL) {
							hpDown->right = dTemp;
							dpNextDown = hpDown->right;
						}
						else {
							dp = hpDown->right;
							for (; dp->next != NULL; dp = dp->next);
							dp->next = dTemp;
							dpNextDown = dp->next;//Paris, Rio, Mexico City
						}


						for (int x = j * maxConnections; x < (j + 1) * maxConnections; x++) {//110 - 120/ 120-130
							if (Airport_Codes.find(x) != Airport_Codes.end()) {
								DLLnode* dpDownTemp = newDLLnode(x);
								if (dpNextDown->down == NULL) {
									dpNextDown->down = dpDownTemp;
								}
								else {

									DLLnode* dpDown = dpNextDown;
									for (; dpDown->down != NULL; dpDown = dpDown->down);
									dpDown->down = dpDownTemp;//Helsinki, Edinburgh, Milan
								}

							}

						}

					}
				}
			}
		}
	}
}

void traceJourney() {
	string currentLocatin, destination;
	int currLocCode, destCode;


	cout << "\nWhere you journey will start from?\n: ";
	cin.ignore(1, '\n');
	getline(cin, currentLocatin);
	cout << "\nWhere is your final destination?\n: ";
	getline(cin, destination);


	if (Airport_Names.find(currentLocatin) != Airport_Names.end()) {
		currLocCode = Airport_Names.at(currentLocatin);	
	}
	else {
		cout << "\n\nWe don't travel from " << currentLocatin << endl;
		return;
	}
	if (Airport_Names.find(destination) != Airport_Names.end()) {
		destCode = Airport_Names.at(destination);
	}
	else {
		cout << "\n\nWe don't travel to " << destination << endl;
		return;
	}

	//check if there is link between them and reply to user if there is no such
	int trackTracer = destCode;
	bool onSameTrack = false;

	//223/10 23/10 2 == currLocCode
	while (trackTracer > 0) {
		trackTracer /= 10;
		if (trackTracer == currLocCode)
			onSameTrack = true;
	}

	if (onSameTrack)
		cout << "\nWe have available filights to your destination\n";
	else {
		cout << "\nSorry but there are no such flights\n";
		return;
	}

	int mainNode, subNode, subSub;
	//London 1 -> Rio 12 -> Lima 122

	if (destCode < 100) {
		subNode = destCode % 10;
		subSub = 0;
	}
	else {
		subNode = (destCode / 10) % 10;
		subSub = destCode % 10;//2
	}
	mainNode = currLocCode;//1


	HashNode* hp = hashHead;
	for (; mainNode > 1; mainNode--) {
		hp = hp->down;
	}
	cout << "\n----->" << hp->countryName << " -> ";


	DLLnode* dp = hp->right;
	for (; subNode > 1; subNode--) {
		dp = dp->next;
	}
	cout << dp->airportName << " -> ";

	if (subSub > 0) {
		for (; subSub >= 1; subSub--) {
			dp = dp->down;
		}

		cout << dp->airportName << endl;
	}
}


HashNode* newHashNode(int ele) {
	HashNode* temp = new HashNode();
	temp->ele = ele;
	temp->countryName = Airport_Codes.at(ele);
	temp->down = NULL;
	temp->right = NULL;
	return temp;
}

DLLnode* newDLLnode(int ele) {
	DLLnode* temp = new DLLnode();
	temp->ele = ele;
	temp->airportName = Airport_Codes.at(ele);
	temp->next = NULL;
	temp->prev = NULL;
	return temp;
}


void insertEnd(int ele, DLLnode*& dllHead) {
	DLLnode* temp = newDLLnode(ele);
	if (dllHead == NULL) { dllHead = temp; }
	else {
		DLLnode* p = dllHead;
		for (; p->next != NULL; p = p->next);
		p->next = temp;
		temp->prev = p;
	}
}


void display() {
	HashNode* hashHead = (::hashHead != NULL) ? ::hashHead : newHashNode(0);
	DLLnode* dllHead;
	for (; ; hashHead = hashHead->down) {
		if (hashHead == NULL)break;
		cout << hashHead->countryName << " : ";
		for (dllHead = hashHead->right; ; dllHead = dllHead->next) {
			if (dllHead == NULL)break;
			cout << dllHead->airportName << " => ";
		}
		cout << endl;
	}
}

void displayMap()
{
	for (auto& airports : Airport_Codes)
	{
		std::cout << airports.first << " : " << airports.second << std::endl;
	}

	for (auto& airports : Airport_Names)
	{
		std::cout << airports.first << " : " << airports.second << std::endl;
	}

}
bool authenticated()
{
	string password = "enter";
	string passwordEntered;
	std::cout << "Please enter your admin password: \n" << std::endl;
	std::cin >> passwordEntered;
	if (passwordEntered == password) {
		cout << "\nYou are now logged in\n";
		return true;
	}
	else {
		cout << "\nWrong password\n";
		return false;
	}
}


void writeFile() {
	ofstream flights;
	flights.open("flights.txt");

	for (auto& airports : Airport_Codes)
	{
		flights << airports.first << " " << airports.second << "\n";
	}

	flights.close();
}

void updateFile(string airport, int flightCode) {
	std::ofstream log("flights.txt", std::ios_base::app | std::ios_base::out);

	log << flightCode << " " << airport << "\n";
}

void readFile() {
	fstream flights;
	flights.open("flights.txt", ios::in);
	if (flights.is_open()) { 
		string airport;
		string line;
		int flightNumber;
		while (getline(flights, line)) { 
			airport = line.substr(line.find(" ") + 1);
			flightNumber = stoi(line.substr(0, line.find(" ")));
			Airport_Codes.insert(pair<int, string>(flightNumber, airport));
			Airport_Names.insert(pair<string, int>(airport, flightNumber));

		}
		flights.close(); 

	}

}



void addDestination()
{
	if (isAuthenticated) {
		int code;
		string name;
		cout << "Please enter the code of the destination you'd like to add: " << std::endl;
		cin >> code;
		std::cout << "Please enter the name of the destination you'd like to add: " << std::endl;
		cin >> name;
		if (Airport_Codes.find(code) != Airport_Codes.end()) {
			cout << "Key Exists!" << endl;
		}
		else {
			Airport_Codes.insert({ code, name });
			updateFile(name, code);
		}
	}
	else {
		std::cout << "\nauthentication failed\n";
		isAuthenticated = authenticated();
	}
}



void menu() {
	int userInput;
	readFile();
	initNodes();

	do {
		cout << "\n\n==================================";
		cout << "\n#1 Search Journey:\n";
		cout << "\n#2 Login\n";
		cout << "\n#3 Update Travel Destinations\n";
		cout << "\n===================================\n\n";



		cin >> userInput;
		switch (userInput)
		{

			case 1: {
				traceJourney();
				break;
			}
			case 2: {
				isAuthenticated = authenticated();
				break;
			}
			case 3: {
				addDestination();
				cout << "\nNew travel destination added\n";
				break;
			}
			default: {
				cout << "\nNo such option";
				break;

			}
		}

	} while (userInput != 0);


}
