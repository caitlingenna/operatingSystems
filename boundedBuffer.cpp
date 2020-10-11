/* Threads and Synchronization — Bounded Buffer
 *
 * Utilizes a bounded buffer and semaphores to manage two threads (producer and consumer),
 * one of which will read lines from a file, while the other prints those lines.
 */
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <thread>
#include "pthread.h"
#include <string>
#include <semaphore.h>
using namespace std;

class BoundedBuffer
{
	public:
		BoundedBuffer(int s = 5, string f=NULL)
		{ //initialize/set semaphores and variables
			fName = f;
			size = s;
			buffer = new string[size];
			done = false;
			counter = 0;
 			sem_init(&empty, 0, s);
                	sem_init(&full, 0, 0);
                	sem_init(&mutex, 0, 1);
		};
		string* buffer;
		bool done;
		int counter, size;
		void produce();
		void consume();
	private:
		string fName;
		sem_t empty, full, mutex;

};
void BoundedBuffer::produce()
{
	ifstream file(fName);
	string line;
	int in = 0;
	while(done == false)
	{ 	sleep(1); // I put a sleep here just so it will be clear that the program is reading individual lines (rather than it seeming like the entire file is printing at once)
		sem_wait(&empty); //wait for buffer empty semaphore to increment
		getline(file, line); //read line from file
		if(file.eof()) //exit condition, reached end of file
			done = true;
		else
		{ 
			sem_wait(&mutex); //wait for mutex to enter critical section
			//write line to buffer, increment buffer 'in' placeholder and counter.
			buffer[in] = line;
			in = (in + 1)% size;
			counter++;
			sem_post(&mutex); //unlock mutex
		}
		sem_post(&full); //post full counter
	}	
};
void BoundedBuffer::consume()
{ 
	int out = 0;
	while(done == false || counter != 0) //exit condition: EOF reached and all lines in buffer have been consumed
	{      // wait on full semaphore and mutex
		sem_wait(&full);
		sem_wait(&mutex);
		if(buffer[out] != "00") // safety condition, checks that current buffer spot is not empty
		{	//print line in buffer, remove line from buffer, increment placeholder and counter
			cout << buffer[out];
			cout << endl;
			buffer[out] = "00";
			out = (out + 1)% size;
			counter--;
		}
		sem_post(&mutex); //post mutex and empty semaphore
		sem_post(&empty);
	}
};

int main(int argc, char* argv[])
{
	int bufferSize;
	string fileName;
	//checks argument number is acceptable
	if(argc == 3)
	{
		bufferSize = atoi(argv[1]);
		fileName = argv[2];
	}
	else
	{
		cout << "Incorrect Arguments." << endl;
		exit(EXIT_FAILURE);
	}
	// create buffer object and two threads, producer and consumer
	BoundedBuffer BB(bufferSize, fileName);
    thread consumer(&BoundedBuffer::consume, &BB);	
	thread producer(&BoundedBuffer::produce, &BB);

	producer.join();
	consumer.join();
	return 0;
}
