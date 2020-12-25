#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"



using std::string;
using std::to_string;
using std::vector;

//constructo
Process::Process(int PID) : PID_(PID) {
    // values set at init and updated at call
    this->User();
    this->Ram();
    this->UpTime();
    this->CpuUtilization();

}

// Done: Return this process's ID
int Process::Pid() { return this->PID_; } 

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    this->data_ = LinuxParser::ProcessUtilization(this->PID_);
    this->utime_ = this->data_[0];
    this->stime_ = this->data_[1];
    this->cutime_ = this->data_[2];
    this->cstime_=this->data_[3];
    this->starttime_ =this->data_[4];
    // calculation could be also factorized to not use seconds
    // as % is unitless
    // child processes also considered 
    this->total_time_ = (this->utime_ + this->stime_+ this->cstime_)/sysconf(_SC_CLK_TCK);
    // possible imporvement to use parent class variable access?
    this -> duration_ =  LinuxParser::UpTime() - this->UpTime();  // to use latest values
    this->CpuUsage_ = this->total_time_ / this->duration_;

    return this->CpuUsage_; }

// Done: Return the command that generated this process
string Process::Command() { 
    this->Command_ = LinuxParser::Command(this->PID_);
    return this->Command_; }

// Done: Return this process's memory utilization
string Process::Ram() { 
    this->RAMUsage_ = LinuxParser::Ram(this->PID_);
    return to_string(this->RAMUsage_); }

// Done: Return the user (name) that generated this process
string Process::User() { 
    this->UserID_ = LinuxParser::Uid(this->PID_);
    this->UserName_ = LinuxParser::User(this->UserID_);
    return this->UserName_; }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() {
    this ->Uptime_ = LinuxParser::UpTime(this->PID_);
    this ->Uptime_ = this->Uptime_ / sysconf(_SC_CLK_TCK); // devided by clock ticks
     return this->Uptime_; }

float Process::getCPUUsage() const{ // getter function for comparison purpose
    return this->CpuUsage_; }
// Done: Overload the "less than" comparison operator for Process objects
// why access to private members was possible here?
bool Process::operator<(Process const& a) const { 

    if (this->CpuUsage_ < a.getCPUUsage())  return true;
    else return false;
     }
bool Process::operator>(Process const& a) const {  

    if (this->CpuUsage_ > a.getCPUUsage())  return true;
    else return false;
     }
