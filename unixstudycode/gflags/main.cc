#include <iostream>

#include <gflags/gflags.h>

DEFINE_bool(kill, false, "to kill the process");

// These are all the same
//  ./gflags-test --kill
//  ./gflags-test -kill
//  ./gflags-test -kill=true

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "kill=" << FLAGS_kill << "\n";
    return 0;
}

