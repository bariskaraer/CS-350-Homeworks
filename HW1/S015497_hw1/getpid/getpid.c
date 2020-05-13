#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

int main(){

int my_pid = getpid();
int sys_pid = syscall(SYS_getpid);

printf("getPID = %d\n", my_pid);
printf("SYS_getpid = %d\n", sys_pid);


assert(sys_pid == (int)my_pid);

return 0;

}
