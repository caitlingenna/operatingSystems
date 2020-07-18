/*  Program uses a linked list to store user inputted program names and times. An alarm system call is used to simulate
 *  scheduling interrupts and each process gets turns to 'run' for a certain timeslice until it has finished.
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
using namespace std;

struct proc //simple structure to hold process info and place in list
{
	string name;
    int timeLeft;
	proc *next = NULL, *previous = NULL;
};
class List
{
	// Linked list class designed specifically for the needs of this assignment (no need to have an insert function, etc.)
	public:
		proc *start, *end;
		List()
		{
			start = NULL;
			end = NULL;
		}
	void newProc(string n, int t)
	{
		proc *temp = new proc;
		temp->name = n;
		temp->timeLeft = t;
		temp->next = NULL;
		if(start == NULL)
		{
			start = temp;
			end = temp;
		}
		else
		{
			end->next = temp;
			temp->previous = end;
			end = temp;
		}
	}
	void moveToEnd(proc *p)
	{
		start = p->next;
		end->next = p;
		end = p;
		p->next = NULL;
	}
	void deleteProc(proc *p)
	{
		if(p == end)
		{
			delete p;
			start = NULL;
		}
		else
		{
			start = p->next;
			start->previous = NULL;
			delete p;
		}
	}
};
List procList;
const int TIMESLICE = 3;
void sigalarm_handler(int signum)
{ // When alarm signals, if there are still processes left in list the next one's timeLeft will be reduced by the timeslice, 
  // and the process will be moved to the end of the list.
	if(procList.start != NULL)
	{
		procList.start->timeLeft -= TIMESLICE;
		cout << procList.start->name << " " << procList.start->timeLeft << endl;
		procList.moveToEnd(procList.start);
	}
}
int main()
{
	string processes, tempS = "", tempC = "", tempName;
	int tempTime;
	cout<<"Enter Process names and times (Ex: p1 15, CTRL D to end input): " << endl;
	while(getline(cin, processes))
	{ // Allows user to enter process names and times until CTRL D is entered.
	  // Assumes input is being entered in correct form.
		for(int i = 0; i < processes.length(); i++)
		{
			tempC = processes.at(i);
			if(tempC != " ")
			{ //while first word is being entered, add chars to temp string
				tempS += tempC;
			}
			else if (tempC == " " && tempName == "")
			{ //if word has ended and no name has been recorded yet, assign temp string to name
				tempName = tempS;
				tempS = "";
			}
			if(i == processes.length() -1)
			{// if end of string is reached, assign remaining characters in temp string to time.
			 // add process to list.
				tempTime = stoi(tempS);
				procList.newProc(tempName, tempTime);
				tempName = "";
				tempS = "";
			}
		}
	};

	// BEGINNING OF ROUND ROBIN
	cout << endl;
	while(procList.start != NULL)
	{
		int tLeft = procList.start->timeLeft;	
		signal(SIGALRM, sigalarm_handler);
		if(tLeft <= TIMESLICE)
		{ //if time left for process is less than alarm cycle, sleep for time left and delete process.
			sleep(tLeft);
			cout << procList.start->name << " Finished" << endl;
			procList.deleteProc(procList.start);
		}
		else
		{ //if time left for process is greater than alarm cycle, sleep until alarm interrupts, go to handling fxn.
			alarm(TIMESLICE);
			sleep(5);	
		}	
	};
}
