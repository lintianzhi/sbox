#include "allowed_syscall.h"

int Allow_C[256] = {SYS_read, SYS_write, /*SYS_open, SYS_close, */SYS_execve,
                    SYS_time, SYS_access, SYS_brk, SYS_munmap, SYS_mprotect, SYS_uname, SYS_mmap2, 
                    SYS_fstat64, SYS_set_thread_area, SYS_exit_group, 0};

int Allow_JAVA[256] = {SYS_read, SYS_write, SYS_open, SYS_close, SYS_execve,
                    SYS_time, SYS_access, SYS_brk, SYS_munmap, SYS_mprotect, SYS_uname, SYS_mmap2, 
                    SYS_fstat64, SYS_set_thread_area, SYS_exit_group, SYS_readlink, SYS_stat64, 
                    SYS_set_tid_address, SYS_set_robust_list, SYS_futex, SYS_rt_sigaction, 
                    SYS_rt_sigprocmask, SYS_ugetrlimit, SYS_getgid32, SYS_getegid32,
                    SYS_getuid32, SYS_geteuid32, SYS_clone, SYS_openat, SYS_getdents64, 
                    SYS_clock_getres, SYS_clock_gettime, SYS_gettimeofday, SYS_lstat64, 
                    SYS_setrlimit, SYS_socketcall, SYS__llseek, SYS_getdents, SYS_mkdir, 
                    SYS_ftruncate, SYS_gettid, SYS_unlink, 0};
