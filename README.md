Developed a program for fast element search in a large integer array by employing multiple 
child processes.

Implemented a system where child processes signal the parent upon finding the target element, 
using SIGUSR1 custom signal. Child processes exit with specific status codes depending on 
success.

Utilized the wait() system call to synchronize processes. Upon successful discovery by any 
child, the parent terminates other child processes.
