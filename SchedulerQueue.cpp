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

        //function executes nex process, having a time_limit to execute it
        int execute_time_limit_process(int time_limit){
            int current_burst = process_queue.front().get_burst();
            process_queue.front().set_burst(std::max(0, current_burst - time_limit));
            return std::min(current_burst, time_limit);
        }
        
        //function executes next process, if is a round robin, call its specific function
        int execute_process(int io_limit){
            if(process_queue.empty()) return 0;
            if(is_fcfs == false) {
                int quantum_left = std :: min(quantum, process_queue.front().get_current_quantum());
                if(io_limit && io_limit < quantum_left){
                    process_queue.front().set_jump(false);
                    int time_elapsed = execute_time_limit_process(io_limit);
                    process_queue.front().set_current_quantum(quantum_left - time_elapsed);
                    return time_elapsed;
                }
                else{
                    int time_elapsed = execute_time_limit_process(quantum_left);
                    if(process_queue.front().get_burst() != 0) process_queue.front().set_jump(true);
                    else process_queue.front().set_jump(false);
                    return time_elapsed;
                }
            }
            process_queue.front().set_jump(false);

            if(io_limit) return execute_time_limit_process(io_limit);

            int current_burst = process_queue.front().get_burst();
            process_queue.front().set_burst(0);
            return current_burst;
        }

};