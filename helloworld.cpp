#include <systemc>
#include <thread> 
#include <chrono>
#include <iostream>

using namespace sc_core;

#define MAXINPUT 5


std::string washingMachine = "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣶⣶⣶⣶⣶⣶⣶⢰⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⣶⢶⣶⣶⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿%%°C⣿⣿⣿⣿⣿⣿PROGRAM[#]⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⠛⠛⠛⠛⠛⠛⠛⠘⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⣿⣿⣿⡿⠛⢉⣁⣤⣤⣤⣤⣈⡉⠛⢿⣿⣿⣿⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⣿⡿⠋⣠⣾⣿⠿⠛⠛⠛⠛⠿⣿⣷⣄⠙⢿⣿⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⡿⠁⣼⣿⡿⢁⣤⠖⠀⠀⠀⠀⠈⢿⡿⠇⠈⢿⣿⣿⠀⠀⠀⠀\n"K
                             "⠀⠀⠀⠀⣿⣿⠃⢸⣿⡿⠀⡞⠁⠀⠀⠀⠀⠀⠀⠀⢠⣶⡇⠘⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⠀⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⣇⠘⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⢠⡈⠛⠃⣸⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⣿⣄⠙⢿⣿⣦⣀⠀⠀⠀⠀⣀⣴⣿⡿⠃⣰⣿⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⣿⣿⣧⣀⠙⠿⣿⣿⣿⣿⣿⣿⠿⠋⣀⣼⣿⣿⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣷⣶⣤⣤⣤⣤⣤⣤⣶⣾⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀\n"
                             "⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀";


void configureConsoleCharacterEncoding() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
}

void updateTemperature(std::string &washingMachine, int temperature) {
    std::regex r("%%");
    std::string updatedTemperature = std::to_string(temperature);
    washingMachine = std::regex_replace(washingMachine, r, updatedTemperature);
}

void updateProgram(std::string &washingMachine, int program) {
    std::regex r("#");
    std::string updatedProgram = std::to_string(program);
    washingMachine = std::regex_replace(washingMachine, r, updatedProgram);
}

void testWashingMachine() {
    std::cout << washingMachine << std::endl;
    updateProgram(washingMachine, 1);
    updateTemperature(washingMachine,35);
    std::cout << washingMachine << std::endl;
}

class COMMON_IF : public sc_interface {
public:
  virtual void write(int data) = 0;
  virtual int read() = 0;
  virtual bool is_fifo_not_empty() = 0;
  virtual void clear_fifo() = 0;
  virtual void errorHandle() = 0;
  virtual sc_event& getEvent(bool proc) = 0;
private:
  virtual bool getLock() = 0;
  virtual bool freeLock() = 0;
};

class PRIMITIVE_CH : public sc_prim_channel, public COMMON_IF {
  public:

  PRIMITIVE_CH(sc_module_name name) : sc_prim_channel(name){}

  void write(int data) override {
      getLock();
      fifo.write(data);
      freeLock();
  }

  int read() override {
      int value = 0;
      getLock();
      value = fifo.read();
      freeLock();
      return value;
  }

  sc_event& getEvent(bool proc) override {
    if(proc == true) return P1TO2;
    else return P2TO1;
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
        std::cout << "Bledny wybor. Uruchom pralke ponownie." << std::endl;
        clear_fifo();
        sc_stop();
      }
    }
  }

private:
  sc_fifo<unsigned int> fifo;
  sc_mutex mutex;
  sc_event P1TO2, P2TO1;

  bool getLock() override {
      if (mutex.trylock() == -1) {
        //std::cout << sc_time_stamp() << ": obtained resource by lock()" << std::endl;
        return mutex.lock();
      } else {
        //std::cout << sc_time_stamp() << ": obtained resource by trylock()" << std::endl;
        return mutex.trylock();
      }
  }

  bool freeLock() override {
    return mutex.unlock();
  }

};

//TODO: ZMIENIC ERROR HANDLING? ZEBY OD NOWA SIE ODPALAL A NIE WYLACZAL

//TODO: zostawic te klodki jak juz sa czy wywalic? przy kolejce w primitive channel
//TODO: poki co zostawielm i zakomentowalem printy o zagarnianiu kłodki

//TODO: dodac ASCII cyferki do programow

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
    while(true){
        int value;
        std::cout << "Wybierz program: ";
        std::cin >> value;
        port->write(value);
        e1.notify(SC_ZERO_TIME);
        wait(port->getEvent(0));
        }
  }

  void programme1(){
    while(true){
      wait(e1);
      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC1: ";

        if(value == 0){
          std::cout << "Wylaczono pralke." << std::endl;
          sc_stop();
        } 
        else if(value == 1)std::cout << "Wybrałeś program 1" << std::endl;
        else port->write(value);

      }

      e2.notify(SC_ZERO_TIME);
    }
  }

  void programme2(){
    
    while(true){
      wait(e1 & e2);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC2: ";

        if(value == 2) std::cout << "Wybrałeś program 2" << std::endl;
        else port->write(value);
      }

      e3.notify(SC_ZERO_TIME);
    }
  }

  void programme3(){
    while(true){
      wait(e1 & e2 & e3);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC3: ";

        if(value == 4) std::cout << "Wybrałeś program 3" << std::endl;
        else port->write(value);
      }

      e4.notify(SC_ZERO_TIME);
    }
  }

  void programme4(){
   
 while(true){
      wait(e1 & e2 & e3 & e4);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC4: ";

        if(value == 8) std::cout << "Wybrałeś program 4" << std::endl;
        else port->write(value);
      }

      e5.notify(SC_ZERO_TIME);
    }
  }

  void programme5(){
  
     while(true){
      wait(e1 & e2 & e3 & e4 & e5);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC5: ";

        if(value == 16) std::cout << "Wybrałeś program 5" << std::endl;
        else port->write(value);
      }

      e6.notify(SC_ZERO_TIME);
    }
  }

  void programme6(){
     while(true){
      wait(e1 & e2 & e3 & e4 & e5 & e6);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC6: ";

        if(value == 32) std::cout << "Wybrałeś program 6" << std::endl;
        else port->write(value);

        wait(SC_ZERO_TIME); // waiting to add value to fifo
        port->errorHandle();
      }

      port->getEvent(1).notify(SC_ZERO_TIME);
    }
  }
};

SC_MODULE(PROCESSOR_2){

  sc_port<COMMON_IF> port;
  sc_event e1, e2, e3, e4, e5, e6;

  SC_CTOR(PROCESSOR_2){
    SC_THREAD(user_input);
    SC_THREAD(programme1);
    SC_THREAD(programme2);
    SC_THREAD(programme3);
    SC_THREAD(programme4);
    SC_THREAD(programme5);
    SC_THREAD(programme6);
  }

  void user_input(){
    while(true){
        wait(port->getEvent(1));
        int value;
        std::cout << "Wybierz temperaturę: ";
        std::cin >> value;
        port->write(value);
        e1.notify(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        }
  }

  void programme1(){
    while(true){
      wait(e1);
      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC1_2: ";

        if(value == 0){ 
          std::cout << "Wylaczono pralke" << std::endl;
          sc_stop();
        }
        else if(value == 1) std::cout << "Temperatura 30 stopni" << std::endl;
        else port->write(value);
      }

      e2.notify(SC_ZERO_TIME);
      wait(SC_ZERO_TIME);
    }
  }

  void programme2(){
    
    while(true){
      wait(e1 & e2);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC2_2: ";

        if(value == 2) std::cout << "Temperatura 35 stopni" << std::endl;
        else port->write(value);
      }

      e3.notify(SC_ZERO_TIME);
      wait(SC_ZERO_TIME);
    }
  }

  void programme3(){
    while(true){
      wait(e1 & e2 & e3);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC3_2: ";

        if(value == 4) std::cout << "Temperatura 40 stopni" << std::endl;
        else port->write(value);
      }

      e4.notify(SC_ZERO_TIME);
    }
  }

  void programme4(){
    while(true){
      wait(e1 & e2 & e3 & e4);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC4_2: ";

        if(value == 8) std::cout << "Temperatura 60 stopni" << std::endl;
        else port->write(value);
      }

      e5.notify(SC_ZERO_TIME);
    }
  }

  void programme5(){
    
     while(true){
      wait(e1 & e2 & e3 & e4 & e5);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC5_2: ";

        if(value == 16) std::cout << "Temperatura 80 stopni" << std::endl;
        else port->write(value);
      }

      e6.notify(SC_ZERO_TIME);
    }
  }
  
  void programme6(){
     while(true){
      wait(e1 & e2 & e3 & e4 & e5 & e6);

      if(port->is_fifo_not_empty()){
        unsigned int value = port->read();
        //std::cout<<"PROC6_2: ";

        if(value == 32) std::cout << "Temperatura 90 stopni" << std::endl;
        else port->write(value);

        wait(SC_ZERO_TIME); // waiting to add value to fifo
        port->errorHandle();
      }

      std::cout << "Doing laundry" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(10));
      port->getEvent(0).notify(SC_ZERO_TIME);
    }
  }
};

int sc_main(int, char*[]) {
  
  PROCESSOR_1 proc2("PROCESSOR_1");
  PROCESSOR_2 proc1("PROCESSOR_2");
  PRIMITIVE_CH primitive("PRIMITIVE_CH");
  proc1.port(primitive);
  proc2.port(primitive);
  
  sc_start();

  return 0;
}