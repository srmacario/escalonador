class Process{
    private:
        int id, burst, io_count, default_burst, default_io_burst, current_quantum;

        //change_queue variable will define if process needs to change queue
        bool change_queue = false;
    public:
        Process(int new_id = 0, int new_burst = 0, int new_io_count = 0) : 
            id(new_id), burst(new_burst), io_count(new_io_count), current_quantum(0), change_queue(false){}
        
        void set_default_burst(int new_burst){
            this->default_burst = new_burst;
        }

        int get_default_burst(){
            return this->default_burst;
        }     

        void set_default_io_burst(int new_burst){
            this->default_io_burst = new_burst;
        }

        int get_default_io_burst(){
            return this->default_io_burst;
        }

        void set_burst(int new_burst){
            this -> burst = new_burst;
        }

        void set_io_count(int new_io_count){
            this -> io_count = new_io_count;
        }

        void set_id(int new_id){
            this->id = new_id;
        }

        int get_burst() const{
            return this -> burst;
        }

        int get_io_count() const{
            return this -> io_count;
        }

        int get_id(){
            return this->id;
        }

        void set_change_queue(bool new_change_queue){
            this->change_queue = new_change_queue;
        }

        bool get_change_queue(){
            return this->change_queue;
        }

        int get_current_quantum(){
            return this->current_quantum;
        }       
        
        void set_current_quantum(int new_quantum){
            this->current_quantum = new_quantum;
        }
};