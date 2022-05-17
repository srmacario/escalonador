#include <queue>
#include "Process.cpp"

class SchedulerQueue{
    private:
        int quantum;
        bool is_fcfs; //determines if this queue is a fcfs, not a round robin
        std::queue <Process> process_queue;

    public:
        SchedulerQueue(int new_quantum) : quantum(new_quantum), is_fcfs(false){}

        SchedulerQueue() : quantum(0), is_fcfs(true){}

        int get_quantum(){
            return this->quantum;
        }

        void set_quantum(int new_quantum){
            this->quantum = new_quantum;
        }
        
        int size(){
            return this->process_queue.size();
        }

        Process peek_process(){
            Process current = this->process_queue.front();
            return current;
        }

        void add_process (Process new_process){
            this->process_queue.push(new_process);
        }

        void pop_process(){
            this->process_queue.pop();
        }

        //function executes next process, having a time_limit to execute it
        int execute_time_limit_process(int time_limit){
            int current_burst = process_queue.front().get_burst();
            process_queue.front().set_burst(std::max(0, current_burst - time_limit));
            return std::min(current_burst, time_limit);
        }
        
        //function executes next process, if is a round robin, call its specific function
        int execute_process(int io_limit){
            //degenerate case
            if(process_queue.empty()) return 0;

            int time_elapsed;
            
            //round robin queue
            if(is_fcfs == false) {
                //quantum left for that process to execute
                int time_limit = process_queue.front().get_current_quantum();

                //check if there is an IO limit bound and update time limit  
                if(io_limit) time_limit = std :: min(time_limit, io_limit);

                //execute process with time limit bounded 
                time_elapsed = execute_time_limit_process(time_limit);

                //update quantum from process
                process_queue.front().set_current_quantum(std::max(0, process_queue.front().get_current_quantum() - time_elapsed));

                //if process is not finished and quantum has finished, it should change queue, otherwise it should not
                if(process_queue.front().get_burst() != 0 && process_queue.front().get_current_quantum() == 0) process_queue.front().set_change_queue(true);
                else process_queue.front().set_change_queue(false);
  
                return time_elapsed;
            }

            //normal queue is the last one, process should never change queue
            process_queue.front().set_change_queue(false);

            //execute queue bounded by io
            if(io_limit) return execute_time_limit_process(io_limit);

            //time elapsed will be equal to current_burst
            //process executes: set its remaining burst to 0

            time_elapsed = process_queue.front().get_burst();
            process_queue.front().set_burst(0);
            return time_elapsed;
        }

};

class FCFSQueue : public SchedulerQueue{};

class RoundRobinQueue : public SchedulerQueue{};