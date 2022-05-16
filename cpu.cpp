#include <iostream>
#include "Scheduler.cpp"

using namespace std;

void menu(){
    cout << "**** Menu de Opcoes ****\n";
    cout << "1 - Adicionar Processo\n"; 
    cout << "2 - Rodar\n";
}

bool menu_op(Scheduler& sc){
    int op;
    cin >> op;
    if(op == 1){
        int id, burst, io_count;
        cout << "Digite um inteiro para o id do processo: ";
        cin >> id;
        cout << "Digite um inteiro para o tempo de burst do processo: ";
        cin >> burst;
        cout << "Digite um inteiro para o numero de operacoes de E/S do processo: ";
        cin >> io_count;
        Process p = Process(id, burst, io_count);
        p.set_default_burst(burst);
        p.set_default_io_burst(20);
        sc.add_process(p);
        cout << "Processo adicionado com sucesso!\n\n";
        return true;
    }
    return false;
}

void print_history(int scheduler_time, vector<Data> history){
    //print history
    for(int i = 0; i < static_cast<int>(history.size()); i++){
        if(i){
            if(history[i].start != history[i - 1].end){
                cout << "--: " << history[i - 1].end << " " << history[i].start << "\n";
            }
        }
        else{
            if(history[i].start != 0){
                cout << "--: " << 0 << " " << history[i].start << "\n";
            }
        }
        cout << "P" << history[i].id << ": " << history[i].start << " " << history[i].end << "\n";
    }
    if(history.back().end != scheduler_time){
        cout << "--: " << history.back().end << " " << scheduler_time << "\n";
    }
    cout << "\n";
}

int main(){
    //initialize cpu scheduler
    Scheduler sc;
    bool run = true;

    //menu for adding process
    while(run){
        menu();
        run = menu_op(sc);
    }

    //process
    int t = 0;
    while(sc.schedule_is_up()){
        t = sc.schedule_process(); 
        if(t == 0) t = sc.execute_io();
        sc.set_current_time(sc.get_current_time() + t);
    }
    cout << "CPU History:\n";
    print_history(sc.get_current_time(), sc.cpu_history);

    cout << "IO History:\n";
    print_history(sc.get_current_time(), sc.io_history);
    return 0;
}