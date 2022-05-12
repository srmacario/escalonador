#include "IOScheduler.cpp"

class Scheduler{
    public:
        //Q[0] -> First priority : Round Robin
        //Q[0] -> Second priority : Round Robin
        //Q[0] -> Third priority : FCFS
        std :: vector<SchedulerQueue> queues;
        SchedulerQueue IOScheduler;
    public:
        std :: vector<std ::pair<int, std :: pair<int, int>>> cpu_history, io_history;
        Scheduler(){
            queues.push_back(SchedulerQueue(10));
            queues.push_back(SchedulerQueue(15));
            queues.push_back(SchedulerQueue());
            IOScheduler = SchedulerQueue();
        }

        void add_process(Process new_process){
            new_process.set_current_quantum(queues[0].get_quantum());
            queues[0].add_process(new_process);
        }

        void send_process_to_queue(Process current_process){
            current_process.set_burst(current_process.get_default_burst());
            add_process(current_process);
            IOScheduler.pop_process();
        }

        void send_process_to_io(int queue_index, Process current_process){
            //set process to have burst the same as default io burst and reduce io counter by
            current_process.set_burst(current_process.get_default_io_burst());
            current_process.set_io_count(current_process.get_io_count() - 1);
            IOScheduler.add_process(current_process);
        }

        //function executes next process, returning time elapsed
        int schedule_process(int current_time){
            for(int i = 0; i < 3; i++){
                if(queues[i].size()){
                    //execute process and capture time elapsed
                    int io_limit = 0;
                    if(IOScheduler.size()) io_limit = IOScheduler.peek_process().get_burst();
                    int time_elapsed = queues[i].execute_process(io_limit);

                    //save cpu burst history
                    Process current_process = queues[i].peek_process();
                    cpu_history.push_back({current_process.get_id(), {current_time, current_time + time_elapsed}});

                    //use time elapsed to execute possible io burst
                    execute_last_io(time_elapsed, current_time);

                    //manage process execute to next queue, or remove it from queue
                    if(current_process.get_jump() == true){
                        queues[i].pop_process();
                        current_process.set_current_quantum(queues[i + 1].get_quantum());
                        queues[i + 1].add_process(current_process);
                    }

                    //process finished, verify if it is needed to read from io again
                    else if(current_process.get_burst() == 0){
                        //remove process from queue
                        queues[i].pop_process();
                        //send process to io
                        if(current_process.get_io_count()) send_process_to_io(i, current_process);
                    }
                    return time_elapsed;
                }
            }
            return 0;
        }

        void execute_last_io(int time_limit, int current_time){
            while((time_limit) && (IOScheduler.size() != 0)){
                //execute io burst
                int time_elapsed = IOScheduler.execute_time_limit_process(time_limit);
                Process current_process = IOScheduler.peek_process();

                //add io executed to io history
                io_history.push_back({current_process.get_id(), {current_time, current_time + time_elapsed}});

                //if io is finished, send process to queue
                if(current_process.get_burst() == 0) send_process_to_queue(current_process);

                //calculate time spent and restart
                time_limit -= time_elapsed;
            }
        }

        int execute_io(int current_time){
            //execute io process
            int time_elapsed = IOScheduler.execute_process(0);

            //no process in queue, finish
            if(time_elapsed == 0) return 0;

            //save history and send it back to queue
            Process current_process = IOScheduler.peek_process();
            io_history.push_back({current_process.get_id(), {current_time, current_time + time_elapsed}});
            send_process_to_queue(current_process);

            return time_elapsed;
        }

        bool schedule_is_up(){
            bool ok = static_cast<bool>(IOScheduler.size());
            for(int i = 0; i < 3; i++) ok |= static_cast<bool>(queues[i].size());
            return ok;
        }
};