#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm> // for sorting

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;



System::System() { // defining values in constructor. for initial values

// since a change of OS or Kernel will imply a restart of software
//no need to waste computation time each 1second calculating kernel and OS again
    this->Kernel_ = LinuxParser::Kernel();
    this -> OS_ = LinuxParser::OperatingSystem();
    // done at init and updated at calls
    this->processes_ = this->Processes();
}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }
// private methods
bool System::lookup(int pid) {
    for (unsigned int i =0; i < (this->processes_.size()); i++) {
        if (this->processes_[i].Pid() == pid) { 
            return true;
        }
    }
    return false;
}
bool Compare(Process a, Process b) {
    if (a< b) { return true;}
    else return false;
}
void System::sorting() {
    std::sort(this->processes_.begin(),this->processes_.end(), std::greater<Process>());
    /*
    Process* exchange;
    for (unsigned int i = 0;  i < this->processes_.size(); i++) {
        if (this->processes_[i] < this->processes_[i+1]) {
            exchange = &(this->processes_[i]);
            this->processes_[i] = this->processes_[i+1];
            this->processes_[i+1] = (*exchange);
        }
    } */
}
// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    //if (this->sync == false) {
    vector<int> Processors_List_Ids = LinuxParser::Pids();
    //this -> processes_.clear(); // clear vector, faster than lookup for  limited number of processes
    for (int &pid : Processors_List_Ids) {
        if (lookup(pid) == false) {
        this->processes_.push_back(Process(pid));
        }
    }
    this->sorting();
    //this ->RunningPIDs_ = this->processes_.size(); // to ensure the invariance 
    //this ->sync = true ; // values updated based on same parsing
    //}
    return this->processes_; // may lead to access issues due to private. to double check the display functions
     }

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() { return this->Kernel_;}

// DONE : Return the system's memory utilization
float System::MemoryUtilization() { 
    this->MemUtilization_ = LinuxParser::MemoryUtilization();
    return this->MemUtilization_; }

// Done: Return the operating system name
std::string System::OperatingSystem() { return this->OS_;} // string(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    /*if (this->sync == false ) {
        this->Processes();
        }
    this->sync = false; // to allow for a refresh after 1 second update
    */
   this->RunningPIDs_ = LinuxParser::RunningProcesses();
    return (this-> RunningPIDs_); } 

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    this->totalPID_ = LinuxParser::TotalProcesses();
    return (this-> totalPID_); }

// TODO: Return the number of seconds since the system started running
// somehow not displayed although value is available? unit?
long int System::UpTime() { 
    this->uptime_ =  (long int) LinuxParser::UpTime();
    return this->uptime_ ; }