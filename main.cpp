/*
Home Project
Student Name: Vishnu Rampersaud
Professor: Pavel Shostak
Course: CS 340
Due Date: 12/14/2018
*/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

//Function to remove process from the CPU queue when it is moving into the CPU
void remove_cpu_queue_proc (vector <int>& cpu_queue, int pid)
{
    int j;
    for (int i =0; i < cpu_queue.size(); i++){
        if (pid == cpu_queue[i]){
            j = i;
        }
    }

    cpu_queue.erase(cpu_queue.begin() + j);
}

//Class to store memory and priority of new process
class new_process
{
public:

    void create_process (char A, int pr, unsigned long int mem)
    {
        priority = pr;
        memory_size = mem;
    }

    int get_priority()
    {
        return priority;
    }

    unsigned long int get_memory ()
    {
        return memory_size;
    }



private:
    int priority;
    unsigned long int memory_size;
    vector <int> pid;

};

//Class to act as a Process Control Block (PCB) for each process; Stores data for each process
class PCB
{
public:

    //Store priority of process
    int set_pri (int x)
    {
        pri =x;
    }

    //Store memory of process
    void set_mem (unsigned long int x)
    {
        mem = x;
    }

    //return memory of process
    unsigned long int get_mem ()
    {
        return mem;
    }

    //return priority of process
    int get_pri ()
    {
        return pri;
    }

    //store pid of process
    int set_pid(int x)
    {
        pid = x;
    }

    //return pid of process
    int get_pid ()
    {
        return pid;
    }

    //store file process uses
    int set_file_name (string x)
    {
        file_name = x;
    }

    //return file name process uses
    string get_file_name ()
    {
        return file_name;
    }
private:
    int pri, pid;           //pri- PRiority of process, pid- Process ID
    unsigned long int mem;  //memory of process
    string file_name;       //file name of process
};

//Function to find the process with the highest priority in the cpu queue, if any, in order to determine if CPU process needs to be preempted
int find_highest_priority (vector <PCB> PCB_vector, vector<vector<int> > disks)
{
    int pri_max = 0;
    int pid =0;

    for (int i=0; i < PCB_vector.size(); i++)
    {
        for (int j=0; j < disks.size(); j++){
            for (int k =0; k < disks[j].size(); k++){
                if (disks[j][k] == PCB_vector[i].get_pid()){
                    //PCB_vector.erase(PCB_vector.begin()+i);
                    PCB_vector[i].set_pid(-1);
                }
            }
        }
    }

    for (int i=0; i < PCB_vector.size(); i++)
    {
        if (PCB_vector[i].get_pri() > pri_max && PCB_vector[i].get_pid() != -1){
            pri_max = PCB_vector[i].get_pri();
            pid = PCB_vector[i].get_pid();
        }
    }

    return pid;
}

//Function to obtain the PCB of process with pid 'X'
int find_proc_PCB (vector <PCB> PCB_vector, int pid)
{
    int x=0;
    for (int i=0; i<PCB_vector.size(); i++){
        if (PCB_vector[i].get_pid() == pid){
            x = i;
        }
    }
    return x;
}


//Class to monitor holes/blocks in the OS memory
class block
{
public:

    //Set initial memory for OS
    void set_OS_mem (unsigned long int mem, int pid)
    {
        block_size = mem;
        block_process = pid;
        pid_begin_location = 0;
        pid_end_location = mem;
    }

    //Store pid of process in block
    void set_block_process (int x)
    {
        block_process = x;
    }

    //Set size of block needed to store process
    void set_block_size (unsigned long int x)
    {
        block_size = x;
    }

    //Retuen pid of process in block
    int get_block_process ()
    {
        return block_process;
    }

    //return size of the block
    unsigned long int get_block_size ()
    {
        return block_size;
    }

    //Check to see if there is enough memory in OS to create new process
    bool check (vector <block> blocks, unsigned long int mem)
    {
        bool create = false;

        //vector to store all blocks that can hold the new process
        vector <int> temp;
        //maximum size of memory in OS that can be used to create processes
        unsigned long int min_block= 4000000000;
        //check the block vector for all holes that are equal to or greater than new process
        for (int i =0; i < blocks.size(); i++){
            if ( (blocks[i].get_block_size() >= mem) && (blocks[i].get_block_process() == 0) ){
                //These are the holes. Add to vector
                temp.push_back(i);
            }
        }

        //check temp vector for the smallest hole that can fit the new process
        for (int i=0; i < temp.size(); i++){
            if (blocks[i].get_block_size() <= min_block){
                create = true;
                break;
            }
        }

        //if a hole was found to store new process, then return true
        return create;
    }

    //Store new process in memory block/holes
    void allocate_proc_mem (vector <block> &blocks, int pid, unsigned long int mem)
    {
        //vector to store all blocks that can hold the new process
        vector <int> temp;
        //maximum size of memory in OS that can be used to create processes
        unsigned long int min_block = 4000000000;

        //check the block vector for all holes that are equal to or greater than new process
        for (int i =0; i < blocks.size(); i++){
            if (blocks[i].get_block_size() >= mem && blocks[i].get_block_process() == 0){
                //These are the holes. Add to vector
                temp.push_back(i);
            }
        }

        //check temp vector for the smallest hole that can fit the new process
        for (int i=0; i < temp.size(); i++){
            int temp_num = temp[i];
            if (blocks[temp_num].get_block_size() <= min_block){
                //store smallest hole location that can fit process in min_block
                min_block = temp_num;
            }
        }

        //Set location of block used for process
        block_used = min_block;
        //Set block size of new process
        block_size = mem;
        //Set pid of process in block
        block_process = pid;
        //Calculate the new size of block used after storing new process.
        blocks[min_block].set_block_size (blocks[min_block].get_block_size() - block_size);
    }

    //Set beginning and end location for each process in memory
    void set_proc_mem_range (vector <block> &blocks)
    {
        //first block always starts at 0
        blocks[0].set_begin(0);
        //End location of process is the memory size
        blocks[0].set_end (blocks[0].get_block_size()-1);

        //find location of processes other than the first block
        for (int i=1; i < blocks.size(); i++)
        {
            //Beginning of block i is equal to the end location of the process before it
            blocks[i].set_begin(blocks[i-1].get_end()+1);
            // End of block i is equal to the beginning location + memory size of process
            blocks[i].set_end(blocks[i].get_begin() + blocks[i].get_block_size()-1);
        }
    }

    //Function to combine adjacent holes in memory
    void combine (vector <block> &blocks)
    {
        unsigned long int total = 0;
        for (int i=0; i < blocks.size()-1; i++)
        {
            if (blocks[i].get_block_process() ==0 && blocks[i+1].get_block_process() ==0)
            {
                //Store total memory of adjacent holes in 'total'
                total = blocks[i].get_block_size() + blocks[i+1].get_block_size();
                //Set the first hole memory size equal to 'total'
                blocks[i].set_block_size(total);
                //remove adjacent (2nd) hole
                blocks.erase(blocks.begin() + (i+1));
                //update the location of all processes in memory
                set_proc_mem_range(blocks);
                i--;
            }
        }
    }

    //set beginning location of block
    void set_begin(unsigned long int x)
    {
        pid_begin_location = x;
    }

    //set ending location of block
    void set_end (unsigned long int x)
    {
        pid_end_location = x;
    }

    //return beginning location of block
    unsigned long int get_begin ()
    {
        return pid_begin_location;
    }

    //return ending location of block
    unsigned long int get_end ()
    {
        return pid_end_location;
    }

    //return index of block
    int get_block_used ()
    {
        return block_used;
    }


private:
    unsigned long int block_size, pid_begin_location, pid_end_location;
    int block_process, block_used;

};

int main()
{
    //initialize variables
    int hd =0, cpu =0, pri_max = 0, pid_max, priority, number, number2, pid=0;
    unsigned long int memory_size, RAM;
    string file_name, c, b;
    char a;

    //CPU ready queue
    vector <int> cpu_queue;
    //Vector to hold all PCBs for each process
    vector <PCB> PCB_vector;
    //vector of vectors to hold all disks and I/O queue for each disk
    vector <vector <int> > disks;
    //vector to hold all blocks and holes processes are using
    vector <block> blocks;

    //User input
    cout << "How much RAM memory is there on the simulated computer? ";
    cin >> RAM;
    while (RAM > 4000000000){
        cout << "RAM memory too high, please enter lower RAM memory for simulated computer: ";
        cin.clear();
        cin >> RAM;
    }

    cout << "how many hard disks does the simulated computer have? ";
    cin >> hd;

    //Set first block w all RAM memory, this will be the max memory the OS can use
    block setOS;
    setOS.set_OS_mem (RAM, 0);
    blocks.push_back(setOS);
    setOS.set_proc_mem_range(blocks);

    //initialize vector with how many disks the OS will have according to user input
    for (int i=0; i < hd; i++){
        vector <int> j;
        disks.push_back(j);
    }

    //run this process non-stop in order to continuously take user input
    while(1){
        cin >> a;

        //if user enters termination command, do the following
        if (a == 't')
        {
            if (cpu == 0)
            {
                cout << "No process in CPU" << endl;
                continue;
            }
            int j =0;
            int k =0;

            //find the process and PCB you want to terminate
            for (int i =0; i < PCB_vector.size(); i++){
                if (cpu == PCB_vector[i].get_pid()){
                    j = i;
                }
            }
            //erase process PCB for PCB vector
            PCB_vector.erase(PCB_vector.begin() + j);

            //find memory used by this process
            for (int i =0; i < blocks.size(); i++){
                if (cpu == blocks[i].get_block_process()){
                    k = i;
                }
            }

            //release memory used by this process
            blocks[k].set_block_process(0);
            //update process locations in memory
            blocks[k].set_proc_mem_range(blocks);
            //Combine all adjacent holes
            blocks[k].combine(blocks);

            //empty cpu
            cpu = 0;

            //find pid with the highest priority to go into the CPU
            pid_max = find_highest_priority(PCB_vector, disks);

            //if the cpu queue is not empty, then remove highest priority process from cpu queue
            if (cpu_queue.size()>0)
                {remove_cpu_queue_proc(cpu_queue, pid_max);}
            //place pid with highest priority into CPU; preemption
            cpu = pid_max;
        }

        //if user enters this command, then remove current process using disk
        else if (a == 'D')
        {
            //take in second input
            cin >> b;

            //change input from string type to int type
            stringstream ss(b);
            ss >> number;

	    if (number >= hd){
                cout << "Disk number does not exist. Please try again" << endl;
                continue;
            }

            if (disks[number].empty())
            {
                cout << "Disk is empty, cannot complete operation. Please enter another command" << endl;
                continue;
            }

            

            int j =0, k=0;
            //Find PCB of process currently using disk
            for (int i =0; i < PCB_vector.size(); i++){
                if (disks[number][0] == PCB_vector[i].get_pid()){
                    j = i;
                }
            }

            //Find PCB of process currently using CPU
            for (int i =0; i < PCB_vector.size(); i++){
                if (cpu == PCB_vector[i].get_pid()){
                    k = i;
                }
            }

            //if cpu is idle, then insert process that is finished with the disk into the CPU
            if (cpu ==0){
                cpu = disks[number][0];
            }
            //Compare priority of cpu process and disk process to see if preemption will should occur.
            //If priority of process leaving dosk is higher than process in cpu, preempt
            else if (PCB_vector[j].get_pri() > PCB_vector[k].get_pri()){
                cpu_queue.push_back(cpu);
                cpu = disks[number][0];
            }
            //If priority of process leaving dosk is lower than process in cpu, do not preempt. Add disk process to cpu queue
            else
                cpu_queue.push_back(disks[number][0]);

            //remove process from using disk, and put next process in I/O queue to use disk
            if (disks[number].size() == 1){
                disks[number].clear();
            }
            else
                disks[number].erase(disks[number].begin());
        }

        //if user enters this command, then create new process
        else if (a == 'A')
        {
            int x=0;
            cin >> b >> c;

            //change input from string type to int type
            stringstream ss(b);
            ss >> priority;

            //change input from string type to int type
            stringstream sx(c);
            sx >> memory_size;

            //Create new block for new process and test to see if process can fit in memory
            block new_block;
            //If process can fit in memory, then create process
            if (new_block.check(blocks, memory_size) == true)
            {
                //Increase pid for every new process that is created
                pid += 1;
                //find and store process in best-fit memory location
                new_block.allocate_proc_mem(blocks, pid, memory_size);
                //get block index of where process can fit
                int block_used = new_block.get_block_used();
                //insert new block of process in the space it can fit.
                blocks.insert(blocks.begin()+ block_used, new_block);
                //Combine adjacent holes
                new_block.combine(blocks);
                //update range of all processes in memory
                new_block.set_proc_mem_range(blocks);

                //Create new process that holds priority and memory size
                new_process new_proc;
                new_proc.create_process(a, priority, memory_size);

                //Create PCB for this new process
                PCB new_pcb;
                //Store priority in PCB
                new_pcb.set_pri(new_proc.get_priority());
                //Store memory in PCB
                new_pcb.set_mem(new_proc.get_memory());
                //Store pid in PCB
                new_pcb.set_pid(pid);

                //Add pcb for new process into PCB vector
                PCB_vector.push_back(new_pcb);

                //Find PCB of process currently in CPU
                for (int i=0; i<PCB_vector.size(); i++){
                    if (PCB_vector[i].get_pid() == cpu){
                        x = i;
                    }
                }

                //if cpu is idle, then add new process into CPU
                if (cpu == 0){
                    cpu = pid;
                }
                //Compare priority of new process to priority of CPU process
                //If priority of new process is higher, then preempt
                else if (new_pcb.get_pri() > PCB_vector[x].get_pri())
                {
                    cpu_queue.push_back(cpu);
                    cpu = pid;
                }

                //If priority of new process is lower, then do not preempt.
                //Store new process in cpu queue
                else
                    cpu_queue.push_back(pid);
            }

            //if there is not enough memory to create process, tell user to try again
            else if (new_block.check(blocks, memory_size) == false)
            {
                cout << "Not enough memory, please try again" << endl;
            }
        }

        //If user enters this command, then add cpu process to disk or disk I/O queue
        else if (a == 'd') //change disks to array or vector or class b/c there is more than one disk.
        {
            cin >> b >> c;

            //change input from string type to int type
            stringstream ss(b);
            ss >> number2;

            //store file name
            file_name = c;

            if (cpu == 0)
            {
                cout << "No process in CPU, cannot complete operation. Please enter another command" << endl;
                continue;
            }

            if (number2 >= hd){
                cout << "Disk number does not exist. Please try again" << endl;
                continue;
            }

            //set file name in PCB
            for (int i=0; i < PCB_vector.size(); i++){
                if (cpu == PCB_vector[i].get_pid()){
                    PCB_vector[i].set_file_name(file_name);
                }
            }

            //Add CPU process to I/O queue
            disks[number2].push_back(cpu);

            // look for next high priority process not in I/O Queue
            pid_max = find_highest_priority(PCB_vector, disks);
            //remove new process from cpu queue
            if (cpu_queue.size()>0)
                {remove_cpu_queue_proc(cpu_queue, pid_max);}
            //New process goes into CPU
            cpu = pid_max;
        }

        //if user enters this command, then display snapshots
        else if (a == 'S')
        {
            cin >> b;

            //display snapshot of CPU/ CPU queue
            if (b[0] == 'r'){
                //if CPU is idle, then output it to user
                if (cpu == 0){
                    cout << "CPU: IDLE" << endl;
                }
                else {
                    //find PCB of CPU process
                    int y = find_proc_PCB(PCB_vector, cpu);
                    //display CPU current process pid and priority
                    cout << "CPU: " << "PID " << cpu << ", Priority " << PCB_vector[y].get_pri() << endl;
                    //display ready queue if cpu queue has anything inside of it
                    if (cpu_queue.size() > 0){
			cout << endl;
                        cout << "Ready Queue: " << endl;
                    }
                    //display ready queue
                    for (int i =0; i < cpu_queue.size(); i++){
                        int q = cpu_queue[i];
                        int p = find_proc_PCB(PCB_vector, q);
                        cout << "PID " << cpu_queue[i] << ", Priority " << PCB_vector[p].get_pri() << endl;
                    }
                }
            }
            else if (b[0] == 'i'){
                //Check all disks in OS. If disk is idle, display idle
                for (int i=0; i < disks.size(); i++){
                    if (disks[i].size() < 1){
                            cout << "Disk " << i << ": Idle" << endl;
                    }
                    for (int j=0; j < disks[i].size(); j++){
                        //Display processes using all disks
                        if (disks[i].size() >= 1 && j == 0){
                            int p = find_proc_PCB(PCB_vector, disks[i][0]);
                            cout << "Disk " << i << ": " << "PID " << disks[i][0] << ", Priority: " << PCB_vector[p].get_pri() << ", file: " << PCB_vector[p].get_file_name() << endl;
                            //if there is an I/O Queue, display it
                            if (disks[i].size() > 1){
                                cout << "I/O Queue: " << endl;
                            }
                        }

                        else {
                            //display I/O queue
                            if (disks[i].size() > 1){
                                int p = find_proc_PCB(PCB_vector, disks[i][j]);
                                cout << "Disk " << i << ": " << "PID " << disks[i][j] << ", Priority: " << PCB_vector[p].get_pri() << ", file: " << PCB_vector[p].get_file_name() << endl;
                            }
                        }
                    }
                    cout << endl;
                }
            }
            else if (b[0]=='m'){
                //Display memory range for all processes and holes in OS
		cout << "All memory is measured in Bytes" << endl;
                for (int i=0; i < blocks.size(); i++){
                    if (blocks[i].get_block_process() == 0){
                        cout << "Hole: " << " Begin: " << blocks[i].get_begin() << " End: " << blocks[i].get_end() << endl;
                    }
                    else
                        cout << "PID: "  << blocks[i].get_block_process() << " Begin: " << blocks[i].get_begin() << " End: " << blocks[i].get_end() << endl;
                }
            }

            else if (b[0] != 'r' || b[0] != 'i' || b[0] != 'm')
            {
                cout << "Command not recognized, please try again" << endl;
                continue;
            }
        }

        else
        {
            cout << "Command not recognized, please try again" << endl;
            continue;
        }

        //clear all user inputs
        cin.clear();
    }

    return 0;
}
