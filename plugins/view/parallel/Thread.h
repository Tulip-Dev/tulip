/*
 * Thread.h
 *
 *  Created on: 15 nov. 2008
 *      Author: Antoine Lambert
 *
 */

#ifndef THREAD_H_
#define THREAD_H_

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <pthread.h>

class Thread {

  public :
    virtual ~Thread();

    // start the thread execution
    void start();

    // join the thread to the calling process
    void join();

    // virtual method containing the instructions the thread has to run
    // all derived classes have to implement it
    virtual void run() = 0;

  private :

    pthread_t _posixThread;

};

#endif // DOXYGEN_NOTFOR_DEVEL

#endif /* THREAD_H_ */
