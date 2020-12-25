#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 int execution_counter {0}; // init with 0;
 float utilization_ {-1};
 long currentIdle_time_; // since division fractions representation is needed for 0.x
 long currentNoneIdle_time_;
 long prevIdle_time_;
 long prevNoneIdle_time_;
 long currentTotal_;
 long prevTotal_;
 long totaldifference_;
 long idledifference_;
 long noneidledifference_;
 long user_;
 long nice_;
 long system_;
 long idle_;
 long iowait_;
 long irq_;
 long softirq_;
 long steal_;
 long guest_;
 long guest_nice_;
};

#endif