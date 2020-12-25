#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  //TODO function to lookup if the found process already in the list
  // goal is to add only unique processes in the next executions
  // might be more calculation expensive than to create new list
  // decision to be made
  bool lookup(std::vector<Process>& Ps, int PID); 
  Processor cpu_ = {};
  long int uptime_;
  std::vector<Process> processes_ = {};
  std::string Kernel_ {"error"};
  std::string OS_ {"error"};
  float MemUtilization_ {-1}; // init with error value
  int totalPID_{-1}; // in case not updated;
  bool sync {false}; // since user can call total processes before calling running processes or vice-versa;
};

#endif