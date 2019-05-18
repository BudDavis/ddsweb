# ddsweb

Instructions:
   git clone https://github.com/BudDavis/ddsweb.git
   git clone https://github.com/zaphoyd/websocketpp
   git clone https://github.com/eclipse-cyclonedds/cyclonedds.git

  cd cyclonedds/
  mkdir build
  mkdir install
  cd build/
  cmake -DCMAKE_INSTALL_PREFIX=../install ../src/
  make
  make install




cmake -DCMAKE_PREFIX_PATH=../cyclonedds/install ../helloworld/
