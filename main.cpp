#include "system.h"

/*
 * the struct:
 *  - main stream: state
 *  - input queue
 *  - input stream: state
 *  - output queue
 *  - output sream: state
 *
 * the main stream behavior:
 *  - wait a user input
 *  - lock the input queue
 *  - push the data to the input queue
 *  - set the input stream state as "RUNNING"
 *  - send a wake up signal to the input stream
 *  - unlock the input queue
 *  - set the self state as "STOPPED"
 *  - sleep and wait for the output stream send a wake up signal
 *
 * the input stream behavior:
 *  - wait for the main stream send a wake up signal
 *  - lock the input queue
 *  - while the input queue is not empty, extract the first item from this queue
 *  - parse the comand
 *  - process the comand
 *  - if need to do output then:
 *  - lock the output queue
 *  - push a needed output string to the output queue
 *  - set the output stream state as "RUNNING"
 *  - send a wake up signal to the output queue
 *  - unlock the output queue
 *  - if the output queue is empty, unlock the input stream
 *  - set the self state as "STOPPED"
 *  - sleep and wait for the main stream or other send a wake up signal
 *
 * the output stream behavior:
 *  - wait for the input stream send a wake up signal
 *  - if the main stream state is "STOPPED" then:
 *  - lock the output queue
 *  - while the output queue is not empty, extract the first item from this queue
 *  - print the string to the standard output
 *  - if the output queue is empty, unlock the output queue
 *  - set the self state as "STOPPED"
 *  - set the main stream state as "RUNNING"
 *  - send the wake up signal to the main stream
 *  - sleep and wait for the input stream send a wake up signal
*/

int main(int argc, char *argv[])
{
    Core::System system;
    return system.exec();
}
