#include <iostream>
#include "Scheduler.cpp"
// #include "Process.cpp"

using namespace std;

int main(){
    //cpu time and cpu scheduler
    int current_time = 0;
    Scheduler sc;
    
    

    //add process
    Process p = Process(0, 17, 3);
    p.set_default_burst(17);
    p.set_default_io_burst(20);
    sc.add_process(p);
    
    p = Process(1, 29, 2);
    p.set_default_burst(29);
    p.set_default_io_burst(20);
    sc.add_process(p);

    //process
    int t = 0;
    cout << current_time << "\n";
    while(sc.schedule_is_up()){
        t = sc.schedule_process(current_time); 
        if(t == 0) t = sc.execute_io(current_time);
        current_time += t;
    }
    
    //print history
    for(int i = 0; i < static_cast<int>(sc.cpu_history.size()); i++){
        if(i){
            if(sc.cpu_history[i].second.first != sc.cpu_history[i - 1].second.second){
                cout << "--: " << sc.cpu_history[i - 1].second.second << " " << sc.cpu_history[i].second.first << "\n";
            }
        }
        cout << "P" << sc.cpu_history[i].first << ": " << sc.cpu_history[i].second.first << " " << sc.cpu_history[i].second.second << "\n";
    }
    return 0;
}