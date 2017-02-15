mkdir -p build
cd build
cmake ..
make
make package
rpm -qpl safe-hello-1.2.3-Linux.rpm
