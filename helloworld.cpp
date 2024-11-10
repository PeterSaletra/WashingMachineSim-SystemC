#include <systemc>
using namespace sc_core;

#define MAXINPUT 5

class COMMON_IF : public sc_interface {
public:
  virtual void write(int data) = 0;
  virtual int read() = 0;
  virtual bool is_fifo_not_empty() = 0;
  virtual void clear_fifo() = 0;
  virtual void errorHandle() = 0;
  virtual sc_event& getEvent() = 0;
private:
  virtual bool getLock() = 0;
  virtual bool freeLock() = 0;
};

class PRIMITIVE_CH : public sc_prim_channel, public COMMON_IF {
  public:

  PRIMITIVE_CH(sc_module_name name) : sc_prim_channel(name){}

  void write(int data) override {
      //getLock();
      fifo.write(data);
      //freeLock();
  }

  int read() override {
      int value = 0;
      //getLock();
      value = fifo.read();
      //freeLock();
      return value;
  }

  sc_event& getEvent() override {
      return nextProcessor;
  }

  bool is_fifo_not_empty() override {
      return fifo.num_available() > 0;
  }

  void clear_fifo() override {
      while (fifo.num_available() > 0) {
          fifo.read();
      }
  }

  void errorHandle() override {
    if(is_fifo_not_empty()){
      unsigned int value = fifo.read();
      if(!(value == 0 or value == 1 or value == 2 or value == 4 or value == 8 or value == 16 or value == 32 or value == 64 or value == 65 or value == 66 or value == 68 or value == 72 or value == 80 or value == 96)){
        std::cout << "Error" << std::endl;
        clear_fifo();
      }
    }
  }

private:
  sc_fifo<unsigned int> fifo;
  sc_mutex mutex;
  sc_event nextProcessor;

  bool getLock() override {
      if (mutex.trylock() == -1) {
        std::cout << sc_time_stamp() << ": obtained resource by lock()" << std::endl;
        return mutex.lock();
      } else {
        std::cout << sc_time_stamp() << ": obtained resource by trylock()" << std::endl;
        return mutex.trylock();
      }
  }

  bool freeLock() override {
    return mutex.unlock();
  }

};

SC_MODULE(PROCESSOR_1){

  sc_port<COMMON_IF> port;
  sc_event e1, e2, e3, e4, e5, e6;

  SC_CTOR(PROCESSOR_1){
    SC_THREAD(user_input)
    SC_THREAD(programme1);
    SC_THREAD(programme2);
    SC_THREAD(programme3);
    SC_THREAD(programme4);
    SC_THREAD(programme5);
    SC_THREAD(programme6);
  }


  void user_input(){
      int value;
      std::cout << "Wybierz program: ";
      std::cin >> value;
      port->write(value);
      e1.notify(SC_ZERO_TIME);
  }

  void programme1(){
    while(true){
      wait(e1);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        std::cout<<"PROC1: ";

        if(value == 0){
          std::cout << "Wybrałeś nic" << std::endl;
        }else if(value == 1){
          std::cout << "Wybrałeś program 1" << std::endl;
        }else{
          port->write(value);
        }
      }

      e2.notify(SC_ZERO_TIME);
    }
  }

  void programme2(){
    
    while(true){
      wait(e1 & e2);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        std::cout<<"PROC2: ";
        if(value == 2){
          std::cout << "Wybrałeś program 2" << std::endl;
        }else{
          port->write(value);
        }
      }

      e3.notify(SC_ZERO_TIME);
       //giving time for other processes to lock the mutex
    }
  }

  void programme3(){
    while(true){
      wait(e1 & e2 & e3);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC3: ";

        if(value == 4){
          std::cout << "Wybrałeś program 3" << std::endl;
        }else{
          port->write(value);
        }
      }

      e4.notify(SC_ZERO_TIME);
    }
  }

  void programme4(){
   
 while(true){
      wait(e1 & e2 & e3 & e4);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC4: ";

        if(value == 8){
          std::cout << "Wybrałeś program 4" << std::endl;
        }else{
          port->write(value);
        }
      }

      e5.notify(SC_ZERO_TIME);
    }
  }

  void programme5(){
    
     while(true){
      wait(e1 & e2 & e3 & e4 & e5);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC5: ";

        if(value == 16){
          std::cout << "Wybrałeś program 5" << std::endl;
        }else{
          port->write(value);
        }
      }

      e6.notify(SC_ZERO_TIME);
    }
  }

  void programme6(){
     while(true){
      wait(e1 & e2 & e3 & e4 & e5 & e6);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC6: ";

        if(value == 32){
          std::cout << "Wybrałeś program 6" << std::endl;
        }else{
          port->write(value);
        }
      }

      port->getEvent().notify(SC_ZERO_TIME);
    }
  }

};




SC_MODULE(PROCESSOR_2){

  sc_port<COMMON_IF> port;
  sc_event e1, e2, e3, e4, e5;

  SC_CTOR(PROCESSOR_2){
    SC_THREAD(programme1);
    SC_THREAD(programme2);
    SC_THREAD(programme3);
    SC_THREAD(programme4);
    SC_THREAD(programme5);
    SC_THREAD(programme6);
  }

  void programme1(){
    while(true){
      wait(port->getEvent());

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        std::cout<<"PROC1_2: ";

        if(value == 65){
          std::cout << "Temperatura 30 stopni" << std::endl;
        }else{
          port->write(value);
        }
      }

      e1.notify(SC_ZERO_TIME);
    }
  }

  void programme2(){
    
    while(true){
      wait(e1);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        std::cout<<"PROC2_2: ";
        if(value == 66){
          std::cout << "Temperatura 35 stopni" << std::endl;
        }else{
          port->write(value);
        }
      }

      e2.notify(SC_ZERO_TIME);
    }
  }

  void programme3(){
    while(true){
      wait(e1 & e2);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC3_2: ";

        if(value == 68){
          std::cout << "Temperatura 40 stopni" << std::endl;
        }else{
          port->write(value);
        }
      }

      e3.notify(SC_ZERO_TIME);
    }
  }

  void programme4(){
   
 while(true){
      wait(e1 & e2 & e3);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC4_2: ";

        if(value == 72){
          std::cout << "Temperatura 60 stopni" << std::endl;
        }else{
          port->write(value);
        }
      }

      e4.notify(SC_ZERO_TIME);
    }
  }

  void programme5(){
    
     while(true){
      wait(e1 & e2 & e3 & e4);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC5_2: ";

        if(value == 80){
          std::cout << "Temperatura 80 stopni" << std::endl;
        }else{
          port->write(value);
        }
      }

      e5.notify(SC_ZERO_TIME);
    }
  }

  void programme6(){
     while(true){
      wait(e1 & e2 & e3 & e4 & e5);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
              std::cout<<"PROC6: ";

        if(value == 96){
          std::cout << "Temperatura 90 stopni" << std::endl;
        }else{
          port->write(value);
        }
      }

      port->getEvent().notify(SC_ZERO_TIME);
    }
  }
};

int sc_main(int, char*[]) {
  sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                           sc_core::SC_DO_NOTHING );

  PROCESSOR_1 proc2("PROCESSOR_1");
  PROCESSOR_2 proc1("PROCESSOR_2");
  PRIMITIVE_CH primitive("PRIMITIVE_CH");
  proc1.port(primitive);
  proc2.port(primitive);
  
  sc_start(30, SC_SEC);

  return 0;
}