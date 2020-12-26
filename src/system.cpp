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


// constructor
System::System() { // defining values in constructor. for initial values

// since a change of OS or Kernel will imply a restart of software
//no need to waste computation time each 1second calculating kernel and OS again
    this->Kernel_ = LinuxParser::Kernel();
    this -> OS_ = LinuxParser::OperatingSystem();
    // done at init and updated at calls
    this->processes_ = this->Processes();
}
// Return the system's CPU
Processor& System::Cpu() { return cpu_; }
// private method
bool System::lookup(int pid) {
    for (unsigned int i =0; i < (this->processes_.size()); i++) {
        if (this->processes_[i].Pid() == pid) { 
            return true;
        }
    }
    return false;
}
// private method
void System::sorting() {
    std::sort(this->processes_.begin(),this->processes_.end(), std::greater<Process>()); // sorting in descending order
}
// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> Processors_List_Ids = LinuxParser::Pids();
    for (int &pid : Processors_List_Ids) {
        if (lookup(pid) == false) {
        this->processes_.push_back(Process(pid));
        }
    }
    this->sorting();
    return this->processes_; 
     }

// Return the system's kernel identifier (string)
std::string System::Kernel() { return this->Kernel_;}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    this->MemUtilization_ = LinuxParser::MemoryUtilization();
    return this->MemUtilization_; }

//  Return the operating system name
std::string System::OperatingSystem() { return this->OS_;} // string(); }

//  Return the number of processes actively running on the system
int System::RunningProcesses() { 
   this->RunningPIDs_ = LinuxParser::RunningProcesses();
    return (this-> RunningPIDs_); } 

//  Return the total number of processes on the system
int System::TotalProcesses() { 
    this->totalPID_ = LinuxParser::TotalProcesses();
    return (this-> totalPID_); }

//  Return the number of seconds since the system started running
long int System::UpTime() { 
    this->uptime_ =  static_cast<long int>(LinuxParser::UpTime());
    return this->uptime_ ; }