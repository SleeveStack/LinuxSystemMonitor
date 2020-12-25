#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      //line.append("blabla "); // testing the linestream>>key>>value in debug
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
// Done: process IDs
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  string line;
  string key;
  long value;
  long MemTotal = -1;
  long MemFree = -1;
  long MemAvailable = -1;
  long Buffers = -1;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if ( filestream.is_open() ) {
    while (std::getline(filestream, line)) {
      replace(line.begin(),line.end(), '\t', ' ');
      replace(line.begin(),line.end(), ':', ' ');
      //replace(line.begin(),line.end(), 'kB', ' '); // to not waste execution time to allocate kb to key
      // another function is needed
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
      if (key == "MemTotal") {
        MemTotal =  value;
            }
      if (key == "MemAvailable") {
        MemAvailable = value;
      }
      if (key == "Buffers") {
        Buffers = value;
      }
    }
    }
    return (float) (MemTotal - MemFree - MemAvailable + Buffers) / (float) MemTotal;
  }
  return -1; //incase of errors
   }

// Done : Read and return the system uptime
long LinuxParser::UpTime() { 
  float SystemUptime;
  float SystemIdletime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> SystemUptime >> SystemIdletime;
        return SystemUptime;
      }  
       }
  
  return -1; // value to catch errors
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return -1;} // value to catch errors}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
vector<long> LinuxParser::ProcessUtilization(int pid) { 
  string line;
  string value;
  short pos_utime = 14 -1; // amount of time scheduled in user mode in clock ticks
  short pos_stime = 15 - 1; //amount of time schedule in kernel mode in clock ticks
  short pos_cutime =16 - 1; // amount of time waited for children in used mode
  short pos_cstime = 17 - 1; // amount of time waited for childrnen in kernel mode
  short pos_starttime = 22 - 1; // time processor started after system boot in jiffies or clock ticks depending on linux version
  std::vector<string> content;
  string pidpath = "/" + to_string(pid) + "/stat";  
  // file documentation : man7.org/linux/man-pages/man5/proc5.html
  std::ifstream filestream(LinuxParser::kProcDirectory + pidpath);
  std::stringstream fullFile; // for both read and write
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
    fullFile << line; // attach all lines in one string stream; only 2 lines here so no huge memory consumption
    }
      for (int i=0; i<= pos_starttime;i++) {
       string value;
       fullFile >> value;
       content.push_back(value);
       }
    return vector<long> {stoi(content[pos_utime]),stoi(content[pos_stime]), stoi(content[pos_cutime]), stoi(content[pos_cstime]),
     stoi(content[pos_starttime])};
  
  }
return vector<long> {-1}; } // to catch errors  return 0; }


long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0;}
  

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Done: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  string key;
  string line;
  

  // variables meaning: kernel.org/doc/Documentation/filesystems/proc.txt
  long user; // normal processes executed in user mode
  long nice; // niced processes executing in user mode
  long system; // processes executing in kernel mode
  long idle; // twiddling thumbs
  long iowait; //waiting for I/O to compelete
  long irq; // servicing interrupts
  long softirq; // serficing softirqs
  long steal;  // involuntary wait
  long guest;  // running a normal guest
  long guest_nice; // running a niced guest
  //long uptime = -1; // init to negative values to catch errors
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      //replace(line.begin(),line.end(), ':', ' ');
      std::istringstream linestream(line);
        while (linestream >> key >> user >> nice >> system >>
         idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
          if (key == "cpu") {
            vector<long> TotalCPUsData = {user, nice, system, idle, iowait, irq, softirq, steal
            ,guest, guest_nice};
            return TotalCPUsData;
       /* long idle_time = idle + iowait;
        long noneIdle_time = user + nice + system + irq + softirq + steal;
        uptime = idle_time / (idle_time + noneIdle_time);
        return uptime;
        */
      }  
       }
    }
  }
  return vector<long> {-1}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string Output = "";
  string line;
  string pidpath = "/" + to_string(pid) + kCmdlineFilename;
  // file documentation : man7.org/linux/man-pages/man5/proc5.html
  std::ifstream filestream(kProcDirectory + pidpath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      Output = Output + line;
    }
  }
  return Output; }

// DONE: Read and return the memory used by a process
// return changed from string to long to long
long LinuxParser::Ram(int pid) { 
  string line;
  string key;
  long VmSize_KB;
  string pidpath = "/" + to_string(pid) + "/status";
  std::ifstream filestream(kProcDirectory + pidpath);
  if (filestream.is_open()) {
    while(getline(filestream, line)) {
      replace(line.begin(),line.end(), '\t', ' ');
      replace(line.begin(),line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> VmSize_KB) {
        if (key == "VmSize") {
          return VmSize_KB / 1000; // conversion to Megabytes
        }
 
       }
    }
  }

  
  return -1; } // to catch errors}

// Done: Read and return the user ID associated with a process
std::string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string pidpath = "/" + to_string(pid) + "/status";
  string UserID;
  std::ifstream filestream(kProcDirectory + pidpath);
  if ( filestream.is_open() ) {
    while (std::getline(filestream, line)) {
      replace(line.begin(),line.end(), '\t', ' ');
      replace(line.begin(),line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> UserID) {
      if (key == "Uid") {
        return UserID;
      }
    }
    }
  }
  return "error"; }

// Done: Read and return the user associated with a process
string LinuxParser::User(string Uid) {
  string userID;
  string line;
  string user;
  string x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      replace(line.begin(),line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> userID;
      if (userID == Uid) {
        return user;
      }  
       }
  }
  return "corresponding user error  ";}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  /* unused
  string executable_name;
  char process_state; //R Runnun // S Sleeping, etc.
  int ppid; // parent process PID
  int pgrp // process group ID of the process
  int session // session ID; 
  */
  short UptimeFile_Position = 22 ; 
  long Uptime;
  std::vector<string> content;


  string pidpath = "/" + to_string(pid) + "/stat";
  // file documentation : man7.org/linux/man-pages/man5/proc5.html
  std::ifstream filestream(kProcDirectory + pidpath);
  std::stringstream fullFile; // for both read and write
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
    fullFile << line; // attach all lines in one string stream; only 2 lines here so no huge memory consumption
    }
      for (int i=1; i<= UptimeFile_Position;i++) {
       string value;
       fullFile >> value;
       content.push_back(value);
       }
      if (! (content[UptimeFile_Position].empty())) { // not empty test
        Uptime = std::stoi(content[UptimeFile_Position-1]); // string to int conversion // position -1 since content start at 0
        return Uptime; 
      }
  }
return -1; } // to catch errors 