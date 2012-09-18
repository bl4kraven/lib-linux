#include <stdlib.h>
#include <iostream>
using namespace std;

#include "threadpool.h"

void Print(void *pvoid)
{
    cout<<"hello"<<endl;
}

int main(int argc, char **argv)
{
    ThreadPool pool;
    pool.Start(5);

    pool.Run(std::ptr_fun(Print));
    pool.Run(std::ptr_fun(Print));
    pool.Run(std::ptr_fun(Print));
    pool.Run(std::ptr_fun(Print));
    pool.Run(std::ptr_fun(Print));
    pool.Run(std::ptr_fun(Print));
    pool.Run(std::ptr_fun(Print));
    sleep(10);
    pool.Stop();
    return 0;
}
