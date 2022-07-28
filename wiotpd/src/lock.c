#include "lock.h"

int lock_file()
{
    int fd, rc;
    if((fd = open(lock_file_path, O_CREAT|O_RDWR, 00644)) == -1 ){
        syslog(LOG_ERR, "Can't create/open lock file\n");
        return 1;
    }

    rc = flock(fd, LOCK_EX | LOCK_NB);
    if(rc == -1){
        syslog(LOG_ERR, "Couldn't lock file ");
        return 1;
    }
    return 0;
}