#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include<vector>
#include <unistd.h> // for conversion
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int PID); // process constructor
  int Pid();                               // Done: See src/process.cpp
  std::string User();                      // Done: See src/process.cpp
  std::string Command();                   // Done: See src/process.cpp
  float CpuUtilization();                  // Done: See src/process.cpp
  std::string Ram();                       // Done: See src/process.cpp
  long int UpTime();                       // Done: See src/process.cpp
  bool operator<(Process const& a) const;  // Done: See src/process.cpp
  bool operator>(Process const&a) const; // added for sorting
  float getCPUUsage() const; // getter function for comparison purpose

  // Done: Declare any necessary private members
 private:
 int PID_ {0}; // the ID of the instantiated processor
 std::string UserID_;
 std::string UserName_;
 std::string Command_;
 long RAMUsage_;
 float Uptime_; // in seconds
 long total_time_;
 float duration_;
 float CpuUsage_;
 };

#endif