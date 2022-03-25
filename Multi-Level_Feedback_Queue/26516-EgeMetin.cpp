#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <queue>
#include <vector>

using namespace std;

struct process
{
	string id, operation;
	int curr_queue = 0, curr_operation = 0;
	vector<string> operations;
	process(string p_id = "0", int p_queue = 0)
		: id(p_id), curr_queue(p_queue)
	{}
};


int main()
{
	string foldername, word;
	int num_queue, num_process, size_S;
	cout << "Please enter the process folder name: ";
	cin >> foldername;
	cout << "When all processes are completed, you can find execution sequence in \"" << foldername << "/output.txt\".\n";
	ifstream input;
	ofstream output;
	input.open(foldername + "/configuration.txt");
	input >> num_queue >> num_process >> size_S;
	input.close();
	vector<queue<process>> Queues;		// The queue of queues

	/* Creating Queues in a vector */
	for (int b = 0; b < num_queue; b++)
	{
		queue<process> Q;
		Queues.push_back(Q);
	}

	/* Reading files one by one till end and placing this operation information to process structs */
	for (int a = 0; a < num_process; a++)
	{
		input.open(foldername + "/p" + to_string(a+1) + ".txt");
		process proc("PC" + to_string(a + 1), num_process);
		while (input >> word)
		{
			proc.operations.push_back(word);
		}
		Queues[0].push(proc);
		input.close();
	}
	int num_done = 0, S = 0, curr_queue = 0, current_queue = 0;	// num_done = Number of completed processes
	// curr_queue = current queue number of processing, current_queue = the current queue that process is placed(or stayed) in

	output.open(foldername + "/output.txt");	// Open output file to write on output.txt
	while (num_done != num_process)				// Contiue processes until all of them are completed
	{
		if (Queues[curr_queue].empty())
		{
			curr_queue++;
		}
		else
		{
			process curr_proc = Queues[curr_queue].front();					// the current process to be processed
			process temp;
			bool last_idx_check = false;
			int x = Queues[curr_queue].front().curr_operation, y = 1;		// x = idx of current operation for that process
			string operation = Queues[curr_queue].front().operations[x];	// current operation

			if (Queues[curr_queue].front().operations[x] == "1")
			{
				S++;
				Queues[curr_queue].front().curr_operation++;
				temp = Queues[curr_queue].front();
				Queues[curr_queue].pop();
				if (curr_queue+1 < num_queue)
					current_queue = curr_queue + 1;
				if (temp.operations[x + 1] == "-")
				{
					num_done++;
					last_idx_check = true;
				}
				else
					Queues[current_queue].push(temp);		// If the next elem is not "-", then we can push the 											
			}												// current process again to where it should be

			else if (Queues[curr_queue].front().operations[x] == "0")
			{
				S++;
				Queues[curr_queue].front().curr_operation++;
				temp = Queues[curr_queue].front();
				Queues[curr_queue].pop();
				current_queue = curr_queue;
				if (temp.operations[x + 1] == "-")
				{
					num_done++;
					last_idx_check = true;
				}
				else 
					Queues[current_queue].push(temp);
			}

			/*		Print accordingly - If next elem is "-", then we only print the operation as "E"	*/
			if (!last_idx_check)
				output << operation << ", " << curr_proc.id << ", Q" << Queues.size() - current_queue << endl;
			else 
				output << "E, " << curr_proc.id << ", QX" << endl;

			/*		 Move all the processes to the top most queue when S is reached		*/
			if (S == size_S)		
			{
				while (y != Queues.size())
				{
					if (!Queues[y].empty())
					{
						curr_proc = Queues[y].front();
						Queues[0].push(curr_proc);
						Queues[y].pop();
						output << "B, " << curr_proc.id << ", Q" << Queues.size() << endl;
					}
					else
						y++;
				}
				curr_queue = 0;
				S = 0;
			}
		}
	}
	output.close();

	return 0;
}