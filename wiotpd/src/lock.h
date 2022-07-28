#include <fcntl.h>
#include <sys/file.h>
#include <syslog.h>
#define lock_file_path "/tmp/lock/procd_wiotpd.lock"


int lock_file();