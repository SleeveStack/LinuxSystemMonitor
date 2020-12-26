#include "processor.h"
#include "linux_parser.h"
#include <vector>
// Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<long> ProcessorsData = LinuxParser::CpuUtilization();
    this->user_ = ProcessorsData[0];
    this->nice_= ProcessorsData[1];
    this->system_=ProcessorsData[2];
    this->idle_=ProcessorsData[3];
    this->iowait_ = ProcessorsData[4];
    this->irq_ = ProcessorsData[5];
    this->softirq_ = ProcessorsData[6];
    this->steal_ = ProcessorsData[7];
    this->guest_=ProcessorsData[8];
    this->guest_nice_ = ProcessorsData[9];
    // total utilization calculation
    this->currentIdle_time_ = this->idle_ + this->iowait_;
    this->currentNoneIdle_time_ = this->user_ + this->nice_ + this->system_ + this->irq_ + this->softirq_ + this-> steal_ + this->guest_ + this->guest_nice_;
    if (this->execution_counter > 0) {
     this->currentTotal_ = this->currentNoneIdle_time_ + this-> currentIdle_time_;
     this->prevTotal_ = this->prevNoneIdle_time_ + this->prevIdle_time_;
     this->totaldifference_ = this->currentTotal_ - this->prevTotal_;
     this->idledifference_ = (this->currentIdle_time_ -this->prevIdle_time_);
     this->noneidledifference_ = this->currentNoneIdle_time_ - this->prevNoneIdle_time_; // not used currently, alternative way of calculation
     this->utilization_ =  ((float)(this->idledifference_) /(float)this->totaldifference_); // error fix of utilization always = 0 by casting
    }
    if (this->execution_counter == 0) {
        this->utilization_ = -1; // not knowen yet
         this->execution_counter++;
    }
    this->prevIdle_time_ = this->currentIdle_time_;
    this ->prevNoneIdle_time_ = this->currentNoneIdle_time_;
    return this->utilization_; }