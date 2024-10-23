# WashingMachineSim-SystemC

## Table of contents
* [General info](#general-info)
* [Setup](#setup)
* [Technologies](#technologies)

## General info
A SystemC-based simulation of a washing machine's operation. This project models embedded systems behavior for educational and testing purposes.

**_NOTE:_**  Project is still under development

## Setup

Setup is based on this website: [learnsystemc](https://www.learnsystemc.com/setup/docker)

1. Install docker
2. Download docker image
```
docker pull learnwithexamples/systemc
```
3. Run docker image
```
docker run -it learnwithexamples/systemc
```

**_NOTE:_** If you want to use vscode as your editor download this extension [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

4. Install git on docker
```
apt install git
```

**_NOTE:_** You don't need to use sudo cause you're already login as root

5. Clone the project
```
git clone https://github.com/PeterSaletra/WashingMachineSim-SystemC.git
cd WashingMachineSim-SystemC
```

6. Build project and run it
```
make
./out.exe
```

## Technologies
- C++
- SystemC 2.3.3
