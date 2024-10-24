#include <systemc>
using namespace sc_core;

SC_MODULE(PROC1){
  SC_CTOR(PROC1){
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

SC_MODULE(PROC2){
  SC_CTOR(PROC2){
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

int sc_main(int, char*[]) { // entry point

  PROC2 proc2("PROC2");
  PROC1 proc1("PROC1");
  
  sc_start();

  return 0;
}

