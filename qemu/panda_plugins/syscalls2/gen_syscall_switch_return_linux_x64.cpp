

extern "C" {
#include "panda_plugin.h" 
}

#include "syscalls2.h" 
#include "panda_common.h"
#include "panda_plugin_plugin.h"

extern "C" {
#include "gen_syscalls_ext_typedefs_linux_x64.h"   // osarch
#include "gen_syscall_ppp_register_return_linux_x64.cpp"  // osarch
}

#include "gen_syscall_ppp_boilerplate_return_linux_x64.cpp" // osarch

void syscall_return_switch_linux_x64 ( CPUState *env, target_ulong pc, target_ulong ordinal) {  // osarch
#ifdef TARGET_X86_64                                          // GUARD
    switch( ordinal ) {                          // CALLNO
// 0 long sys_read [' unsigned int  fd', ' char __user *  buf', ' size_t  count']
case 0: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_read_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 2 long sys_open [' const char __user *  filename', ' int  flags', ' umode_t  mode']
case 2: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_open_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 3 long sys_close [' unsigned int  fd']
case 3: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_close_return, env,pc,arg0) ; 
}; break;
// 5 long sys_newfstat [' unsigned int  fd', ' struct stat __user *  statbuf']
case 5: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_newfstat_return, env,pc,arg0,arg1) ; 
}; break;
// 8 long sys_lseek [' unsigned int  fd', ' off_t  offset', ' unsigned int  origin']
case 8: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_lseek_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 11 long sys_munmap [' unsigned long  addr', ' size_t  len']
case 11: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_munmap_return, env,pc,arg0,arg1) ; 
}; break;
// 12 long sys_brk [' unsigned long  brk']
case 12: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_brk_return, env,pc,arg0) ; 
}; break;
// 16 long sys_ioctl [' unsigned int  fd', ' unsigned int  cmd', ' unsigned long  arg']
case 16: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_ioctl_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 21 long sys_access [' const char __user *  filename', ' int  mode']
case 21: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_access_return, env,pc,arg0,arg1) ; 
}; break;
// 22 long sys_pipe [' int __user *  fildes']
case 22: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_pipe_return, env,pc,arg0) ; 
}; break;
// 24 long sys_sched_yield ['void']
case 24: {
PPP_RUN_CB(on_sys_sched_yield_return, env,pc) ; 
}; break;
// 26 long sys_msync [' unsigned long  start', ' size_t  len', ' int  flags']
case 26: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_msync_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 28 long sys_madvise [' unsigned long  start', ' size_t  len_in', ' int  behavior']
case 28: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_madvise_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 29 long sys_shmget [' key_t  key', ' size_t  size', ' int  shmflg']
case 29: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_shmget_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 30 long sys_shmat [' int  shmid', ' char __user *  shmaddr', ' int  shmflg']
case 30: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_shmat_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 31 long sys_shmctl [' int  shmid', ' int  cmd', ' struct shmid_ds __user *  buf']
case 31: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
target_ulong arg2 = get_return_pointer(env, 2);
PPP_RUN_CB(on_sys_shmctl_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 32 long sys_dup [' unsigned int  fildes']
case 32: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_dup_return, env,pc,arg0) ; 
}; break;
// 33 long sys_dup2 [' unsigned int  oldfd', ' unsigned int  newfd']
case 33: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_dup2_return, env,pc,arg0,arg1) ; 
}; break;
// 34 long sys_pause ['void']
case 34: {
PPP_RUN_CB(on_sys_pause_return, env,pc) ; 
}; break;
// 36 long sys_getitimer [' int  which', ' struct itimerval __user *  value']
case 36: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_getitimer_return, env,pc,arg0,arg1) ; 
}; break;
// 37 long sys_alarm [' unsigned int  seconds']
case 37: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_alarm_return, env,pc,arg0) ; 
}; break;
// 39 long sys_getpid ['void']
case 39: {
PPP_RUN_CB(on_sys_getpid_return, env,pc) ; 
}; break;
// 40 long sys_sendfile64 [' int  out_fd', ' int  in_fd', ' loff_t __user *  offset', ' size_t  count']
case 40: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
target_ulong arg2 = get_return_pointer(env, 2);
uint32_t arg3 = get_return_32(env, 3);
PPP_RUN_CB(on_sys_sendfile64_return, env,pc,arg0,arg1,arg2,arg3) ; 
}; break;
// 41 long sys_socket [' int  family', ' int  type', ' int  protocol']
case 41: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_socket_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 46 long sys_sendmsg [' int  fd', ' struct msghdr __user *  msg', ' unsigned  flags']
case 46: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_sendmsg_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 48 long sys_shutdown [' int  fd', ' int  how']
case 48: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_shutdown_return, env,pc,arg0,arg1) ; 
}; break;
// 49 long sys_bind [' int  fd', ' struct sockaddr __user *  umyaddr', ' int  addrlen']
case 49: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_bind_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 50 long sys_listen [' int  fd', ' int  backlog']
case 50: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_listen_return, env,pc,arg0,arg1) ; 
}; break;
// 60 long sys_exit [' int  error_code']
case 60: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_exit_return, env,pc,arg0) ; 
}; break;
// 62 long sys_kill [' pid_t  pid', ' int  sig']
case 62: {
uint32_t arg0 = get_return_32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_kill_return, env,pc,arg0,arg1) ; 
}; break;
// 63 long sys_newuname [' struct new_utsname __user *  name']
case 63: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_newuname_return, env,pc,arg0) ; 
}; break;
// 64 long sys_semget [' key_t  key', ' int  nsems', ' int  semflg']
case 64: {
uint32_t arg0 = get_return_32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_semget_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 67 long sys_shmdt [' char __user *  shmaddr']
case 67: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_shmdt_return, env,pc,arg0) ; 
}; break;
// 68 long sys_msgget [' key_t  key', ' int  msgflg']
case 68: {
uint32_t arg0 = get_return_32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_msgget_return, env,pc,arg0,arg1) ; 
}; break;
// 71 long sys_msgctl [' int  msqid', ' int  cmd', ' struct msqid_ds __user *  buf']
case 71: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
target_ulong arg2 = get_return_pointer(env, 2);
PPP_RUN_CB(on_sys_msgctl_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 72 long sys_fcntl [' unsigned int  fd', ' unsigned int  cmd', ' unsigned long  arg']
case 72: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_fcntl_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 73 long sys_flock [' unsigned int  fd', ' unsigned int  cmd']
case 73: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_flock_return, env,pc,arg0,arg1) ; 
}; break;
// 74 long sys_fsync [' unsigned int  fd']
case 74: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_fsync_return, env,pc,arg0) ; 
}; break;
// 75 long sys_fdatasync [' unsigned int  fd']
case 75: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_fdatasync_return, env,pc,arg0) ; 
}; break;
// 76 long sys_truncate [' const char __user *  path', ' long  length']
case 76: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_truncate_return, env,pc,arg0,arg1) ; 
}; break;
// 77 long sys_ftruncate [' unsigned int  fd', ' unsigned long  length']
case 77: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_ftruncate_return, env,pc,arg0,arg1) ; 
}; break;
// 79 long sys_getcwd [' char __user *  buf', ' unsigned long  size']
case 79: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_getcwd_return, env,pc,arg0,arg1) ; 
}; break;
// 80 long sys_chdir [' const char __user *  filename']
case 80: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_chdir_return, env,pc,arg0) ; 
}; break;
// 81 long sys_fchdir [' unsigned int  fd']
case 81: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_fchdir_return, env,pc,arg0) ; 
}; break;
// 82 long sys_rename [' const char __user *  oldname', ' const char __user *  newname']
case 82: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_rename_return, env,pc,arg0,arg1) ; 
}; break;
// 83 long sys_mkdir [' const char __user *  pathname', ' umode_t  mode']
case 83: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_mkdir_return, env,pc,arg0,arg1) ; 
}; break;
// 84 long sys_rmdir [' const char __user *  pathname']
case 84: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_rmdir_return, env,pc,arg0) ; 
}; break;
// 85 long sys_creat [' const char __user *  pathname', ' umode_t  mode']
case 85: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_creat_return, env,pc,arg0,arg1) ; 
}; break;
// 86 long sys_link [' const char __user *  oldname', ' const char __user *  newname']
case 86: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_link_return, env,pc,arg0,arg1) ; 
}; break;
// 87 long sys_unlink [' const char __user *  pathname']
case 87: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_unlink_return, env,pc,arg0) ; 
}; break;
// 88 long sys_symlink [' const char __user *  oldname', ' const char __user *  newname']
case 88: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_symlink_return, env,pc,arg0,arg1) ; 
}; break;
// 90 long sys_chmod [' const char __user *  filename', ' umode_t  mode']
case 90: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_chmod_return, env,pc,arg0,arg1) ; 
}; break;
// 91 long sys_fchmod [' unsigned int  fd', ' umode_t  mode']
case 91: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_fchmod_return, env,pc,arg0,arg1) ; 
}; break;
// 92 long sys_chown [' const char __user *  filename', ' uid_t  user', ' gid_t  group']
case 92: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_chown_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 93 long sys_fchown [' unsigned int  fd', ' uid_t  user', ' gid_t  group']
case 93: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_fchown_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 94 long sys_lchown [' const char __user *  filename', ' uid_t  user', ' gid_t  group']
case 94: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_lchown_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 95 long sys_umask [' int  mask']
case 95: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_umask_return, env,pc,arg0) ; 
}; break;
// 97 long sys_getrlimit [' unsigned int  resource', ' struct rlimit __user *  rlim']
case 97: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_getrlimit_return, env,pc,arg0,arg1) ; 
}; break;
// 98 long sys_getrusage [' int  who', ' struct rusage __user *  ru']
case 98: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_getrusage_return, env,pc,arg0,arg1) ; 
}; break;
// 99 long sys_sysinfo [' struct sysinfo __user *  info']
case 99: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_sysinfo_return, env,pc,arg0) ; 
}; break;
// 100 long sys_times [' struct tms __user *  tbuf']
case 100: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_times_return, env,pc,arg0) ; 
}; break;
// 102 long sys_getuid ['void']
case 102: {
PPP_RUN_CB(on_sys_getuid_return, env,pc) ; 
}; break;
// 103 long sys_syslog [' int  type', ' char __user *  buf', ' int  len']
case 103: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_syslog_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 104 long sys_getgid ['void']
case 104: {
PPP_RUN_CB(on_sys_getgid_return, env,pc) ; 
}; break;
// 105 long sys_setuid [' uid_t  uid']
case 105: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_setuid_return, env,pc,arg0) ; 
}; break;
// 106 long sys_setgid [' gid_t  gid']
case 106: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_setgid_return, env,pc,arg0) ; 
}; break;
// 107 long sys_geteuid ['void']
case 107: {
PPP_RUN_CB(on_sys_geteuid_return, env,pc) ; 
}; break;
// 108 long sys_getegid ['void']
case 108: {
PPP_RUN_CB(on_sys_getegid_return, env,pc) ; 
}; break;
// 109 long sys_setpgid [' pid_t  pid', ' pid_t  pgid']
case 109: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_setpgid_return, env,pc,arg0,arg1) ; 
}; break;
// 110 long sys_getppid ['void']
case 110: {
PPP_RUN_CB(on_sys_getppid_return, env,pc) ; 
}; break;
// 111 long sys_getpgrp ['void']
case 111: {
PPP_RUN_CB(on_sys_getpgrp_return, env,pc) ; 
}; break;
// 112 long sys_setsid ['void']
case 112: {
PPP_RUN_CB(on_sys_setsid_return, env,pc) ; 
}; break;
// 113 long sys_setreuid [' uid_t  ruid', ' uid_t  euid']
case 113: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_setreuid_return, env,pc,arg0,arg1) ; 
}; break;
// 114 long sys_setregid [' gid_t  rgid', ' gid_t  egid']
case 114: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_setregid_return, env,pc,arg0,arg1) ; 
}; break;
// 115 long sys_getgroups [' int  gidsetsize', ' gid_t __user *  grouplist']
case 115: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_getgroups_return, env,pc,arg0,arg1) ; 
}; break;
// 116 long sys_setgroups [' int  gidsetsize', ' gid_t __user *  grouplist']
case 116: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_setgroups_return, env,pc,arg0,arg1) ; 
}; break;
// 117 long sys_setresuid [' uid_t  ruid', ' uid_t  euid', ' uid_t  suid']
case 117: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_setresuid_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 118 long sys_getresuid [' uid_t __user *  ruid', ' uid_t __user *  euid', ' uid_t __user *  suid']
case 118: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
target_ulong arg2 = get_return_pointer(env, 2);
PPP_RUN_CB(on_sys_getresuid_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 119 long sys_setresgid [' gid_t  rgid', ' gid_t  egid', ' gid_t  sgid']
case 119: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_setresgid_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 120 long sys_getresgid [' gid_t __user *  rgid', ' gid_t __user *  egid', ' gid_t __user *  sgid']
case 120: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
target_ulong arg2 = get_return_pointer(env, 2);
PPP_RUN_CB(on_sys_getresgid_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 121 long sys_getpgid [' pid_t  pid']
case 121: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_getpgid_return, env,pc,arg0) ; 
}; break;
// 122 long sys_setfsuid [' uid_t  uid']
case 122: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_setfsuid_return, env,pc,arg0) ; 
}; break;
// 123 long sys_setfsgid [' gid_t  gid']
case 123: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_setfsgid_return, env,pc,arg0) ; 
}; break;
// 124 long sys_getsid [' pid_t  pid']
case 124: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_getsid_return, env,pc,arg0) ; 
}; break;
// 125 long sys_capget [' cap_user_header_t  header', ' cap_user_data_t  dataptr']
case 125: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_capget_return, env,pc,arg0,arg1) ; 
}; break;
// 126 long sys_capset [' cap_user_header_t  header', ' const cap_user_data_t  data']
case 126: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_capset_return, env,pc,arg0,arg1) ; 
}; break;
// 127 long sys_rt_sigpending [' sigset_t __user *  set', ' size_t  sigsetsize']
case 127: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_rt_sigpending_return, env,pc,arg0,arg1) ; 
}; break;
// 130 long sys_rt_sigsuspend [' sigset_t __user *  unewset', ' size_t  sigsetsize']
case 130: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_rt_sigsuspend_return, env,pc,arg0,arg1) ; 
}; break;
// 132 long sys_utime [' char __user *  filename', ' struct utimbuf __user *  times']
case 132: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_utime_return, env,pc,arg0,arg1) ; 
}; break;
// 133 long sys_mknod [' const char __user *  filename', ' umode_t  mode', ' unsigned  dev']
case 133: {
target_ulong arg0 = get_return_pointer(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_mknod_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 135 long sys_personality [' unsigned int  personality']
case 135: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_personality_return, env,pc,arg0) ; 
}; break;
// 136 long sys_ustat [' unsigned  dev', ' struct ustat __user *  ubuf']
case 136: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_ustat_return, env,pc,arg0,arg1) ; 
}; break;
// 137 long sys_statfs [' const char __user *  pathname', ' struct statfs __user *  buf']
case 137: {
target_ulong arg0 = get_return_pointer(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_statfs_return, env,pc,arg0,arg1) ; 
}; break;
// 138 long sys_fstatfs [' unsigned int  fd', ' struct statfs __user *  buf']
case 138: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_fstatfs_return, env,pc,arg0,arg1) ; 
}; break;
// 139 long sys_sysfs [' int  option', ' unsigned long  arg1', ' unsigned long  arg2']
case 139: {
int32_t arg0 = get_return_s32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_sysfs_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 140 long sys_getpriority [' int  which', ' int  who']
case 140: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_getpriority_return, env,pc,arg0,arg1) ; 
}; break;
// 141 long sys_setpriority [' int  which', ' int  who', ' int  niceval']
case 141: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_setpriority_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 142 long sys_sched_setparam [' pid_t  pid', ' struct sched_param __user *  param']
case 142: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_sched_setparam_return, env,pc,arg0,arg1) ; 
}; break;
// 143 long sys_sched_getparam [' pid_t  pid', ' struct sched_param __user *  param']
case 143: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_sched_getparam_return, env,pc,arg0,arg1) ; 
}; break;
// 145 long sys_sched_getscheduler [' pid_t  pid']
case 145: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_sched_getscheduler_return, env,pc,arg0) ; 
}; break;
// 146 long sys_sched_get_priority_max [' int  policy']
case 146: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_sched_get_priority_max_return, env,pc,arg0) ; 
}; break;
// 147 long sys_sched_get_priority_min [' int  policy']
case 147: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_sched_get_priority_min_return, env,pc,arg0) ; 
}; break;
// 149 long sys_mlock [' unsigned long  start', ' size_t  len']
case 149: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_mlock_return, env,pc,arg0,arg1) ; 
}; break;
// 150 long sys_munlock [' unsigned long  start', ' size_t  len']
case 150: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_munlock_return, env,pc,arg0,arg1) ; 
}; break;
// 151 long sys_mlockall [' int  flags']
case 151: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_mlockall_return, env,pc,arg0) ; 
}; break;
// 152 long sys_munlockall ['void']
case 152: {
PPP_RUN_CB(on_sys_munlockall_return, env,pc) ; 
}; break;
// 153 long sys_vhangup ['void']
case 153: {
PPP_RUN_CB(on_sys_vhangup_return, env,pc) ; 
}; break;
// 156 long sys_sysctl [' struct __sysctl_args __user *  args']
case 156: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_sysctl_return, env,pc,arg0) ; 
}; break;
// 159 long sys_adjtimex [' struct timex __user *  txc_p']
case 159: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_adjtimex_return, env,pc,arg0) ; 
}; break;
// 160 long sys_setrlimit [' unsigned int  resource', ' struct rlimit __user *  rlim']
case 160: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_setrlimit_return, env,pc,arg0,arg1) ; 
}; break;
// 161 long sys_chroot [' const char __user *  filename']
case 161: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_chroot_return, env,pc,arg0) ; 
}; break;
// 162 long sys_sync ['void']
case 162: {
PPP_RUN_CB(on_sys_sync_return, env,pc) ; 
}; break;
// 163 long sys_acct [' const char __user *  name']
case 163: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_acct_return, env,pc,arg0) ; 
}; break;
// 166 long sys_umount [' char __user *  name', ' int  flags']
case 166: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_umount_return, env,pc,arg0,arg1) ; 
}; break;
// 167 long sys_swapon [' const char __user *  specialfile', ' int  swap_flags']
case 167: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_swapon_return, env,pc,arg0,arg1) ; 
}; break;
// 168 long sys_swapoff [' const char __user *  specialfile']
case 168: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_swapoff_return, env,pc,arg0) ; 
}; break;
// 170 long sys_sethostname [' char __user *  name', ' int  len']
case 170: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_sethostname_return, env,pc,arg0,arg1) ; 
}; break;
// 171 long sys_setdomainname [' char __user *  name', ' int  len']
case 171: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_setdomainname_return, env,pc,arg0,arg1) ; 
}; break;
// 186 long sys_gettid ['void']
case 186: {
PPP_RUN_CB(on_sys_gettid_return, env,pc) ; 
}; break;
// 196 long sys_flistxattr [' int  fd', ' char __user *  list', ' size_t  size']
case 196: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_flistxattr_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 199 long sys_fremovexattr [' int  fd', ' const char __user *  name']
case 199: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_fremovexattr_return, env,pc,arg0,arg1) ; 
}; break;
// 200 long sys_tkill [' pid_t  pid', ' int  sig']
case 200: {
uint32_t arg0 = get_return_32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_tkill_return, env,pc,arg0,arg1) ; 
}; break;
// 201 long sys_time [' time_t __user *  tloc']
case 201: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_time_return, env,pc,arg0) ; 
}; break;
// 206 long sys_io_setup [' unsigned  nr_events', ' aio_context_t __user *  ctxp']
case 206: {
uint32_t arg0 = get_return_32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_io_setup_return, env,pc,arg0,arg1) ; 
}; break;
// 207 long sys_io_destroy [' aio_context_t  ctx']
case 207: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_io_destroy_return, env,pc,arg0) ; 
}; break;
// 213 long sys_epoll_create [' int  size']
case 213: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_epoll_create_return, env,pc,arg0) ; 
}; break;
// 218 long sys_set_tid_address [' int __user *  tidptr']
case 218: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_set_tid_address_return, env,pc,arg0) ; 
}; break;
// 219 long sys_restart_syscall ['void']
case 219: {
PPP_RUN_CB(on_sys_restart_syscall_return, env,pc) ; 
}; break;
// 225 long sys_timer_getoverrun [' timer_t  timer_id']
case 225: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_timer_getoverrun_return, env,pc,arg0) ; 
}; break;
// 226 long sys_timer_delete [' timer_t  timer_id']
case 226: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_timer_delete_return, env,pc,arg0) ; 
}; break;
// 231 long sys_exit_group [' int  error_code']
case 231: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_exit_group_return, env,pc,arg0) ; 
}; break;
// 234 long sys_tgkill [' pid_t  tgid', ' pid_t  pid', ' int  sig']
case 234: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_tgkill_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 241 long sys_mq_unlink [' const char __user *  u_name']
case 241: {
target_ulong arg0 = get_return_pointer(env, 0);
PPP_RUN_CB(on_sys_mq_unlink_return, env,pc,arg0) ; 
}; break;
// 251 long sys_ioprio_set [' int  which', ' int  who', ' int  ioprio']
case 251: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_ioprio_set_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 252 long sys_ioprio_get [' int  which', ' int  who']
case 252: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_ioprio_get_return, env,pc,arg0,arg1) ; 
}; break;
// 253 long sys_inotify_init ['void']
case 253: {
PPP_RUN_CB(on_sys_inotify_init_return, env,pc) ; 
}; break;
// 255 long sys_inotify_rm_watch [' int  fd', ' __s32  wd']
case 255: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_inotify_rm_watch_return, env,pc,arg0,arg1) ; 
}; break;
// 258 long sys_mkdirat [' int  dfd', ' const char __user *  pathname', ' umode_t  mode']
case 258: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_mkdirat_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 263 long sys_unlinkat [' int  dfd', ' const char __user *  pathname', ' int  flag']
case 263: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_unlinkat_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 268 long sys_fchmodat [' int  dfd', ' const char __user *  filename', ' umode_t  mode']
case 268: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
uint32_t arg2 = get_return_32(env, 2);
PPP_RUN_CB(on_sys_fchmodat_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 269 long sys_faccessat [' int  dfd', ' const char __user *  filename', ' int  mode']
case 269: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_faccessat_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 272 long sys_unshare [' unsigned long  unshare_flags']
case 272: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_unshare_return, env,pc,arg0) ; 
}; break;
// 276 long sys_tee [' int  fdin', ' int  fdout', ' size_t  len', ' unsigned int  flags']
case 276: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
uint32_t arg2 = get_return_32(env, 2);
uint32_t arg3 = get_return_32(env, 3);
PPP_RUN_CB(on_sys_tee_return, env,pc,arg0,arg1,arg2,arg3) ; 
}; break;
// 283 long sys_timerfd_create [' int  clockid', ' int  flags']
case 283: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_timerfd_create_return, env,pc,arg0,arg1) ; 
}; break;
// 284 long sys_eventfd [' unsigned int  count']
case 284: {
uint32_t arg0 = get_return_32(env, 0);
PPP_RUN_CB(on_sys_eventfd_return, env,pc,arg0) ; 
}; break;
// 287 long sys_timerfd_gettime [' int  ufd', ' struct itimerspec __user *  otmr']
case 287: {
int32_t arg0 = get_return_s32(env, 0);
target_ulong arg1 = get_return_pointer(env, 1);
PPP_RUN_CB(on_sys_timerfd_gettime_return, env,pc,arg0,arg1) ; 
}; break;
// 290 long sys_eventfd2 [' unsigned int  count', ' int  flags']
case 290: {
uint32_t arg0 = get_return_32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_eventfd2_return, env,pc,arg0,arg1) ; 
}; break;
// 291 long sys_epoll_create1 [' int  flags']
case 291: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_epoll_create1_return, env,pc,arg0) ; 
}; break;
// 292 long sys_dup3 [' unsigned int  oldfd', ' unsigned int  newfd', ' int  flags']
case 292: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
int32_t arg2 = get_return_s32(env, 2);
PPP_RUN_CB(on_sys_dup3_return, env,pc,arg0,arg1,arg2) ; 
}; break;
// 293 long sys_pipe2 [' int __user *  fildes', ' int  flags']
case 293: {
target_ulong arg0 = get_return_pointer(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_pipe2_return, env,pc,arg0,arg1) ; 
}; break;
// 294 long sys_inotify_init1 [' int  flags']
case 294: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_inotify_init1_return, env,pc,arg0) ; 
}; break;
// 300 long sys_fanotify_init [' unsigned int  flags', ' unsigned int  event_f_flags']
case 300: {
uint32_t arg0 = get_return_32(env, 0);
uint32_t arg1 = get_return_32(env, 1);
PPP_RUN_CB(on_sys_fanotify_init_return, env,pc,arg0,arg1) ; 
}; break;
// 306 long sys_syncfs [' int  fd']
case 306: {
int32_t arg0 = get_return_s32(env, 0);
PPP_RUN_CB(on_sys_syncfs_return, env,pc,arg0) ; 
}; break;
// 308 long sys_setns [' int  fd', ' int  nstype']
case 308: {
int32_t arg0 = get_return_s32(env, 0);
int32_t arg1 = get_return_s32(env, 1);
PPP_RUN_CB(on_sys_setns_return, env,pc,arg0,arg1) ; 
}; break;
default:
PPP_RUN_CB(on_unknown_sys_linux_x64_return, env, pc, EAX);
}
PPP_RUN_CB(on_all_sys_linux_x64_return, env, pc, EAX);
#endif
 } 
