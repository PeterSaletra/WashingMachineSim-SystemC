#include <systemc>
using namespace sc_core;

//interface declaring funtions that primitive channel has to implement
class COMMON_IF : public sc_interface {
public:
  virtual void write(int data) = 0;
  virtual int read() = 0;
  virtual bool is_fifo_not_empty() = 0;
  virtual void clear_fifo() = 0;
};

//primitive channel which will synchronize FIFO access in the future
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

private:
  sc_fifo<int> fifo;
};

// Module simulating processor1
SC_MODULE(PROCESSOR_1){

  sc_port<COMMON_IF> port;

  SC_CTOR(PROCESSOR_1){
    SC_THREAD(run);
  }

  void run(){
    while (true)
    {
      std::cout << "Proc 1" << std::endl;
      wait(2, SC_SEC);
    }
  }

};

// Module simulating processor2
SC_MODULE(PROCESSOR_2){

    sc_port<COMMON_IF> port;

  SC_CTOR(PROCESSOR_2){
    SC_THREAD(run);
  }

  void run(){
    while (true)
    {
      std::cout << "Proc 2" << std::endl;
      wait(2, SC_SEC);
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

