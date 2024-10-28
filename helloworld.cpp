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
};

class PRIMITIVE_CH : public sc_prim_channel, public COMMON_IF {
  public:
  PRIMITIVE_CH(sc_module_name name) : sc_prim_channel(name){}

  void write(int data) override {
      fifo.write(data);
  }
  int read() override {
      return fifo.read();
  }
  bool is_fifo_not_empty(){
      return fifo.num_available() > 0;
  }
  void clear_fifo() override {
      while (fifo.num_available() > 0) {
          fifo.read();
      }
  }

  void errorHandle(){
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
};

SC_MODULE(PROCESSOR_1){

  sc_port<COMMON_IF> port;

  SC_CTOR(PROCESSOR_1){
    SC_THREAD(run);
  }

  void run(){
    unsigned int value;
    while (true){
      std::cout << "Wybierz program: ";
      std::cin >> value;
      port->write(value);
      if(value < 64){
        wait(10, SC_MS);
        programme1();
        wait(10, SC_MS);
        programme2();
        wait(10, SC_MS);
        programme3();
        wait(10, SC_MS);
        programme4();
        wait(10, SC_MS);
        programme5();
        wait(10, SC_MS);
        programme6();
        wait(10, SC_MS);
        port->errorHandle();
        wait(10, SC_MS);
      }
      wait(60, SC_MS);
    }
  }

  void programme1(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 0){
        std::cout << "Wybrałeś nic" << std::endl;
      }else if(value == 1){
        std::cout << "Wybrałeś program 1" << std::endl;
      }else{
        port->write(value);
      }
    }

  }

  void programme2(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 2){
        std::cout << "Wybrałeś program 2" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme3(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 4){
        std::cout << "Wybrałeś program 3" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme4(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 8){
        std::cout << "Wybrałeś program 4" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme5(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 16){
        std::cout << "Wybrałeś program 5" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme6(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 32){
        std::cout << "Wybrałeś program 6" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

};

SC_MODULE(PROCESSOR_2){

  sc_port<COMMON_IF> port;

  SC_CTOR(PROCESSOR_2){
    SC_THREAD(run);
  }

  void run(){
    unsigned int value;
    while (true){
      wait(10, SC_MS);
      if(port->is_fifo_not_empty()){
        value = port->read();
        port->write(value);
        if(value >= 64){
          wait(5, SC_MS);
          programme1();
          wait(5, SC_MS);
          programme2();
          wait(5, SC_MS);
          programme3();
          wait(5, SC_MS);
          programme4();
          wait(5, SC_MS);
          programme5();
          wait(5, SC_MS);
          programme6();
          wait(5, SC_MS);
          port->errorHandle();
          wait(5, SC_MS);
        }
      }
    }
  }

  void programme1(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 65){
        std::cout << "Temperatura 30 stopni" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme2(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 66){
        std::cout << "Temperatura 35 stopni" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme3(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 68){
        std::cout << "Temperatura 40 stopni" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme4(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 72){
        std::cout << "Temperatura 60 stopni" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme5(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 80){
        std::cout << "Temperatura 80 stopni" << std::endl;
      }else{
        port->write(value);
      }
    }
  }

  void programme6(){
    if(port->is_fifo_not_empty()){
      unsigned int value = port->read();
      if(value == 96){
        std::cout << "Temperatura 90 stopni" << std::endl;
      }else{
        port->write(value);
      }
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