/* PANDABEGINCOMMENT
 * 
 * Authors:
 *  Tim Leek               tleek@ll.mit.edu
 *  Ryan Whelan            rwhelan@ll.mit.edu
 *  Joshua Hodosh          josh.hodosh@ll.mit.edu
 *  Michael Zhivich        mzhivich@ll.mit.edu
 *  Brendan Dolan-Gavitt   brendandg@gatech.edu
 * 
 * This work is licensed under the terms of the GNU GPL, version 2. 
 * See the COPYING file in the top-level directory. 
 * 
PANDAENDCOMMENT */
#define __STDC_FORMAT_MACROS

extern "C" {

#include "config.h"
#include "qemu-common.h"
#include "cpu.h"

#include "rr_log.h"
#include "panda_plugin.h"
#include "pandalog.h"        
#include "panda_common.h"
#include "../syscalls2/gen_syscalls_ext_typedefs_windows7_x86.h"
#include "../syscalls2/gen_syscalls_ext_typedefs_windows7_x64.h"
#include "panda_plugin_plugin.h"

int before_block_exec(CPUState *env, TranslationBlock *tb);

bool init_plugin(void *);
void uninit_plugin(void *);
}

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>

#ifdef TARGET_I386
#define DEFAULT_LOG_FILE "win7proc_report"

//FILE *proc_log = 0;
//FILE *proc_hist;

//typedef std::pair<std::string,uint32_t> procid;
//std::map<procid,uint64_t> bbcount;

#define KMODE_FS           0x030
#define KPCR_CURTHREAD_OFF 0x124
#define KTHREAD_KPROC_OFF  0x150
#define EPROC_PID_OFF      0x0b4
#define EPROC_NAME_OFF     0x16c

static uint32_t get_pid(CPUState *env, target_ulong eproc) {
    uint32_t pid;
    panda_virtual_memory_rw(env, eproc+EPROC_PID_OFF, (uint8_t *)&pid, 4, false);
    return pid;
}

static void get_procname(CPUState *env, target_ulong eproc, char *name) {
    panda_virtual_memory_rw(env, eproc+EPROC_NAME_OFF, (uint8_t *)name, 15, false);
    name[16] = '\0';
}

static uint32_t get_current_proc(CPUState *env) {
    // Read the kernel-mode FS segment base
    uint32_t e1, e2;
    uint32_t fs_base, thread, proc;

    // Read out the two 32-bit ints that make up a segment descriptor
    panda_virtual_memory_rw(env, env->gdt.base + KMODE_FS, (uint8_t *)&e1, 4, false);
    panda_virtual_memory_rw(env, env->gdt.base + KMODE_FS + 4, (uint8_t *)&e2, 4, false);
    
    // Turn wacky segment into base
    fs_base = (e1 >> 16) | ((e2 & 0xff) << 16) | (e2 & 0xff000000);

    // Read KPCR->CurrentThread->Process
    panda_virtual_memory_rw(env, fs_base+KPCR_CURTHREAD_OFF, (uint8_t *)&thread, 4, false);
    panda_virtual_memory_rw(env, thread+KTHREAD_KPROC_OFF, (uint8_t *)&proc, 4, false);

    return proc;
}

#define UNKNOWN_PID 0xFFFFFFFF
char cur_procname[16];
uint32_t cur_pid = UNKNOWN_PID;

int before_block_exec(CPUState *env, TranslationBlock *tb) {
    bool changed = false;
    //    bool in_kernel = false;
    if (panda_in_kernel(env)) {
        changed = cur_pid != UNKNOWN_PID;
	//        in_kernel = true;
        //        bbcount[std::make_pair("Kernel",UNKNOWN_PID)]++;
        return 0;
        cur_pid = UNKNOWN_PID;
        if (changed) strcpy(cur_procname, "Kernel");
    }
    else {
        uint32_t proc = get_current_proc(env);
        uint32_t new_pid = get_pid(env, proc);
        changed = cur_pid != new_pid;
        cur_pid = new_pid;
        if (changed) get_procname(env, proc, cur_procname);
        //        bbcount[std::make_pair(cur_procname,cur_pid)]++;
    }

    if (changed) {
        Panda__Process *np = (Panda__Process *) malloc(sizeof(Panda__Process));
        *np = PANDA__PROCESS__INIT;
        np->pid = cur_pid;
        np->name = cur_procname;
        Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
        ple.new_pid = np;
        pandalog_write_entry(&ple);
    }
    return 0;
}

#define IMAGEPATHNAME_OFF      0x38
#define OBJNAME_OFF            0x8
#define EPROC_OBJTABLE_OFF     0xf4

#define HANDLE_MASK1  0x000007fc
#define HANDLE_SHIFT1  2
#define HANDLE_MASK2  0x001ff800
#define HANDLE_SHIFT2  11
#define HANDLE_MASK3  0x7fe00000
#define HANDLE_SHIFT3  21
#define LEVEL_MASK 0x00000007
#define TABLE_MASK ~LEVEL_MASK
#define ADDR_SIZE 4
#define HANDLE_TABLE_ENTRY_SIZE 8

// Win7 Obj Type Indices
typedef enum {
    OBJ_TYPE_Type = 2,
    OBJ_TYPE_Directory = 3,
    OBJ_TYPE_SymbolicLink = 4,
    OBJ_TYPE_Token = 5,
    OBJ_TYPE_Job = 6,
    OBJ_TYPE_Process = 7,  
    OBJ_TYPE_Thread = 8,
    OBJ_TYPE_UserApcReserve = 9,
    OBJ_TYPE_IoCompletionReserve = 10,
    OBJ_TYPE_DebugObject = 11,
    OBJ_TYPE_Event = 12,
    OBJ_TYPE_EventPair = 13,
    OBJ_TYPE_Mutant = 14,
    OBJ_TYPE_Callback = 15,
    OBJ_TYPE_Semaphore = 16,
    OBJ_TYPE_Timer = 17,
    OBJ_TYPE_Profile = 18,
    OBJ_TYPE_KeyedEvent = 19,
    OBJ_TYPE_WindowStation = 20,
    OBJ_TYPE_Desktop = 21,
    OBJ_TYPE_TpWorkerFactory = 22,
    OBJ_TYPE_Adapter = 23,
    OBJ_TYPE_Controller = 24,
    OBJ_TYPE_Device = 25,
    OBJ_TYPE_Driver = 26,
    OBJ_TYPE_IoCompletion = 27,
    OBJ_TYPE_File = 28,
    OBJ_TYPE_TmTm = 29,
    OBJ_TYPE_TmTx = 30,
    OBJ_TYPE_TmRm = 31,
    OBJ_TYPE_TmEn = 32,
    OBJ_TYPE_Section = 33,
    OBJ_TYPE_Session = 34,
    OBJ_TYPE_Key = 35,
    OBJ_TYPE_ALPCPort = 36,
    OBJ_TYPE_PowerRequest = 37,
    OBJ_TYPE_WmiGuid = 38,
    OBJ_TYPE_EtwRegistration = 39,
    OBJ_TYPE_EtwConsumer = 40,
    OBJ_TYPE_FilterConnectionPort = 41,
    OBJ_TYPE_FilterCommunicationPort = 42,
    OBJ_TYPE_PcwObject = 43
} OBJ_TYPES;

uint32_t handle_table_code(CPUState *env, uint32_t table_vaddr) {
    uint32_t tableCode;
    // HANDLE_TABLE.TableCode is offest 0
    panda_virtual_memory_rw(env, table_vaddr, (uint8_t *)&tableCode, 4, false);
    return (tableCode & TABLE_MASK);
}


uint32_t handle_table_L1_addr(CPUState *env, uint32_t table_vaddr, uint32_t entry_num) {
    return handle_table_code(env, table_vaddr) + ADDR_SIZE * entry_num;
}


uint32_t handle_table_L2_addr(uint32_t L1_table, uint32_t L2) {
    if (L1_table != 0x0) {
        uint32_t L2_entry = L1_table + ADDR_SIZE * L2;
        return L2_entry;
    }
    return 0;
}


uint32_t handle_table_L1_entry(CPUState *env, uint32_t table_vaddr, uint32_t entry_num) {
    return (handle_table_code(env, table_vaddr) +	
            HANDLE_TABLE_ENTRY_SIZE * entry_num);
}


uint32_t handle_table_L2_entry(uint32_t table_vaddr, uint32_t L1_table, uint32_t L2) {
    if (L1_table == 0) return 0;
    return L1_table + HANDLE_TABLE_ENTRY_SIZE * L2;          
}


uint32_t handle_table_L3_entry(uint32_t table_vaddr, uint32_t L2_table, uint32_t L3) {
    if (L2_table == 0) return 0;
    return L2_table + HANDLE_TABLE_ENTRY_SIZE * L3;
}

// i.e. return pointer to the object represented by this handle
uint32_t get_handle_table_entry(CPUState *env, uint32_t pHandleTable, uint32_t handle) {
    uint32_t tableCode, tableLevels;
    // get tablecode
    panda_virtual_memory_rw(env, pHandleTable, (uint8_t *)&tableCode, 4, false);
    //printf ("tableCode = 0x%x\n", tableCode);
    // extract levels
    tableLevels = tableCode & LEVEL_MASK;  
    //printf("tableLevels = %d\n", tableLevels);
    //  assert (tableLevels <= 2);
    if (tableLevels > 2) {
        return 0;
    }
    uint32 pEntry=0;
    if (tableLevels == 0) {
        uint32_t index = (handle & HANDLE_MASK1) >> HANDLE_SHIFT1;
        pEntry = handle_table_L1_entry(env, pHandleTable, index);
    }
    if (tableLevels == 1) {
        uint32_t L1_index = (handle & HANDLE_MASK2) >> HANDLE_SHIFT2;
        uint32_t L1_table_off = handle_table_L1_addr(env, pHandleTable, L1_index);
        uint32_t L1_table;
        panda_virtual_memory_rw(env, L1_table_off, (uint8_t *) &L1_table, 4, false);
        uint32_t index = (handle & HANDLE_MASK1) >> HANDLE_SHIFT1;
        pEntry = handle_table_L2_entry(pHandleTable, L1_table, index);
    }
    if (tableLevels == 2) {
        uint32_t L1_index = (handle & HANDLE_MASK3) >> HANDLE_SHIFT3;
        uint32_t L1_table_off = handle_table_L1_addr(env, pHandleTable, L1_index);
        uint32_t L1_table;
        panda_virtual_memory_rw(env, L1_table_off, (uint8_t *) &L1_table, 4, false);
        uint32_t L2_index = (handle & HANDLE_MASK2) >> HANDLE_SHIFT2;
        uint32_t L2_table_off = handle_table_L2_addr(L1_table, L2_index);
        uint32_t L2_table;
        panda_virtual_memory_rw(env, L2_table_off, (uint8_t *) &L2_table, 4, false);
        uint32_t index = (handle & HANDLE_MASK1) >> HANDLE_SHIFT1;
        pEntry = handle_table_L3_entry(pHandleTable, L2_table, index);
    }
    uint32_t pObjectHeader;
    if ((panda_virtual_memory_rw(env, pEntry, (uint8_t *) &pObjectHeader, 4, false)) == -1) {
        return 0;
    }
    //  printf ("processHandle_to_pid pObjectHeader = 0x%x\n", pObjectHeader);
    pObjectHeader &= ~0x00000007;

    return pObjectHeader;
}

// Hack
static void unicode_to_ascii(char *uni, char *ascii, int len) {
    for (int i = 0; i < len; i++) {
        ascii[i] = uni[i*2];
    }
}

static char *read_unicode_string(CPUState *env, target_ulong pUstr) {
    uint16_t fileNameLen;
    uint32_t fileNamePtr;
    char *fileName = (char *)calloc(1, 260);
    char fileNameUnicode[260*2] = {};

    panda_virtual_memory_rw(env, pUstr,
            (uint8_t *) &fileNameLen, 2, false);
    panda_virtual_memory_rw(env, pUstr+4,
            (uint8_t *) &fileNamePtr, 4, false);

    if (fileNameLen > 259*2) {
        fileNameLen = 259*2; 
    }
    panda_virtual_memory_rw(env, fileNamePtr, (uint8_t *)fileNameUnicode, fileNameLen, false);
    unicode_to_ascii(fileNameUnicode, fileName, fileNameLen/2);

    return fileName;
}

static char * get_objname(CPUState *env, target_ulong obj) {
    uint32_t pObjectName;

    panda_virtual_memory_rw(env, obj+OBJNAME_OFF,
            (uint8_t *) &pObjectName, 4, false);
    return read_unicode_string(env, pObjectName);
}

#define FILE_OBJECT_NAME_OFF 0x30
static char *get_file_obj_name(CPUState *env, uint32_t fobj) {
    return read_unicode_string(env, fobj+FILE_OBJECT_NAME_OFF);
}

typedef struct handle_object_struct {
    uint8_t objType;
    uint32_t pObj;
} HandleObject;


static HandleObject *get_handle_object(CPUState *env, uint32_t eproc, uint32_t handle) {
    uint32_t pObjectTable;
    if (-1 == panda_virtual_memory_rw(env, eproc+EPROC_OBJTABLE_OFF, (uint8_t *)&pObjectTable, 4, false)) {
        return NULL;
    }
    uint32_t pObjHeader = get_handle_table_entry(env, pObjectTable, handle);
    if (pObjHeader == 0) return NULL;
    uint32_t pObj = pObjHeader + 0x18;
    uint8_t objType = 0;
    if (-1 == panda_virtual_memory_rw(env, pObjHeader+0xc, &objType, 1, false)) {
        return NULL;
    }
    HandleObject *ho = (HandleObject *) malloc(sizeof(HandleObject));
    ho->objType = objType;
    ho->pObj = pObj;
    return ho;
}


static char *get_handle_object_name(CPUState *env, HandleObject *ho) {
    if (ho == NULL){
        char *procName = (char *) calloc(8, 1);
	sprintf(procName, "unknown");
         return procName;
    }
    switch (ho->objType) {
        case OBJ_TYPE_File:
            return get_file_obj_name(env, ho->pObj);
        case OBJ_TYPE_Key: {
            char *fileName = (char *) calloc(100, 1);
            sprintf(fileName, "_CM_KEY_BODY@%08x", ho->pObj);
            return fileName;
        }
	  break;
        case OBJ_TYPE_Process: {
            char *procName = (char *) calloc(100, 1);
            char procExeName[16] = {};
            uint32_t procPid = get_pid(env, ho->pObj);
            get_procname(env, ho->pObj, procExeName);
            sprintf(procName, "[%d] %s", procPid, procExeName);
            return procName;
        }
            break;
        default:
	  char *procName = (char *) calloc(8, 1);
	    sprintf(procName, "unknown");
	    return procName;
    }
}


static char * get_handle_name(CPUState *env, uint32_t eproc, uint32_t handle) {
    HandleObject *ho = get_handle_object(env, eproc, handle);
    return get_handle_object_name(env, ho);
}

// Individual system call callbacks

Panda__Process *create_panda_process (uint32_t pid, char *name) {
    Panda__Process *p = (Panda__Process *) malloc(sizeof(Panda__Process));
    *p = PANDA__PROCESS__INIT;
    p->pid = pid;
    p->name = name;
    return p;
}

void w7p_NtCreateUserProcess_return(
        CPUState* env,
        target_ulong pc,
        uint32_t ProcessHandle,
        uint32_t ThreadHandle,
        uint32_t ProcessDesiredAccess,
        uint32_t ThreadDesiredAccess,
        uint32_t ProcessObjectAttributes,
        uint32_t ThreadObjectAttributes,
        uint32_t ProcessFlags,
        uint32_t ThreadFlags,
        uint32_t ProcessParameters,
        uint32_t CreateInfo,
        uint32_t AttributeList) {
    uint16_t procNameLen;
    uint32_t procNamePtr;
    char procName[260] = {};
    char procNameUnicode[260*2] = {};
    
    panda_virtual_memory_rw(env, ProcessParameters+IMAGEPATHNAME_OFF,
            (uint8_t *) &procNameLen, 2, false);
    panda_virtual_memory_rw(env, ProcessParameters+IMAGEPATHNAME_OFF+4,
            (uint8_t *) &procNamePtr, 4, false);
    if (procNameLen > 259*2) {
        procNameLen = 259*2;
    }  
    panda_virtual_memory_rw(env, procNamePtr, (uint8_t *)procNameUnicode, procNameLen, false);
    unicode_to_ascii(procNameUnicode, procName, procNameLen/2);
    // Retrieve the returned handle and look up the name/PID of the newly created
    // process
    uint32_t handle;
    panda_virtual_memory_rw(env, ProcessHandle, (uint8_t *)&handle, 4, false);
    uint32_t eproc = get_current_proc(env);
    HandleObject *ho = get_handle_object(env, eproc, handle);
    char *newProc = get_handle_object_name(env, ho);
    uint32_t newPid = get_pid(env, ho->pObj);
    Panda__Process *cur_p = create_panda_process(cur_pid, cur_procname);
    Panda__Process *new_p = create_panda_process(newPid, newProc);
    Panda__NtCreateUserProcess *ntcup = 
      (Panda__NtCreateUserProcess *) malloc (sizeof(Panda__NtCreateUserProcess));
    *ntcup = PANDA__NT_CREATE_USER_PROCESS__INIT;
    ntcup->new_long_name = procName;
    ntcup->cur_p = cur_p;
    ntcup->new_p = new_p;
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_create_user_process = ntcup;
    pandalog_write_entry(&ple);        
    free(newProc);
}

// terminates a process and all of its threads
void w7p_NtTerminateProcess_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t ProcessHandle,
        uint32_t ExitStatus) {
    Panda__Process *cur_p = create_panda_process(cur_pid, cur_procname);
    Panda__Process *term_p = 0;
    if (ProcessHandle == ((uint32_t) -1)) {
        // self destruct
        term_p = create_panda_process(cur_pid, cur_procname);
    }
    else {
        // less common -- kill another process
        uint32_t handle;
        panda_virtual_memory_rw(env, ProcessHandle, (uint8_t *)&handle, 4, false);
        uint32_t eproc = get_current_proc(env);
        HandleObject *ho = get_handle_object(env, eproc, handle);
        if (ho) {
            term_p = create_panda_process(get_pid(env, ho->pObj),
                                          get_handle_object_name(env, ho));
        }
    }
    if (term_p) {
        Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
        Panda__NtTerminateProcess *nttp = 
            (Panda__NtTerminateProcess *) malloc (sizeof(Panda__NtTerminateProcess));
        *nttp = PANDA__NT_TERMINATE_PROCESS__INIT;
        nttp->cur_p = cur_p;
        nttp->term_p = term_p;
        ple.nt_terminate_process = nttp;
        pandalog_write_entry(&ple);
    }
}

Panda__ProcessFile *create_cur_process_file (char *filename) {
    Panda__ProcessFile *pf = (Panda__ProcessFile *) malloc(sizeof(Panda__ProcessFile));
    *pf = PANDA__PROCESS_FILE__INIT;
    pf->proc = create_panda_process(cur_pid, cur_procname);
    pf->filename = filename;
    return pf;
}

// creates a new file or opens an existing file
void w7p_NtCreateFile_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t FileHandle,
        uint32_t DesiredAccess,
        uint32_t ObjectAttributes,
        uint32_t IoStatusBlock,
        uint32_t AllocationSize,
        uint32_t FileAttributes,
        uint32_t ShareAccess,
        uint32_t CreateDisposition,
        uint32_t CreateOptions,
        uint32_t EaBuffer,
        uint32_t EaLength) {
    char *fileName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_create_file = create_cur_process_file(fileName);
    pandalog_write_entry(&ple);
    free(fileName);
}

void w7p_NtReadFile_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t FileHandle,
        uint32_t Event,
        uint32_t UserApcRoutine,
        uint32_t UserApcContext,
        uint32_t IoStatusBlock,
        uint32_t Buffer,
        uint32_t BufferLength,
        uint32_t ByteOffset,
        uint32_t Key) {
    char *fileName = get_handle_name(env, get_current_proc(env), FileHandle);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_read_file = create_cur_process_file(fileName);
    pandalog_write_entry(&ple);
    free(fileName);
}

void w7p_NtDeleteFile_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t ObjectAttributes) {
    char *fileName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_delete_file = create_cur_process_file(fileName);
    pandalog_write_entry(&ple);
    free(fileName);
}

void w7p_NtWriteFile_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t FileHandle,
        uint32_t Event,
        uint32_t ApcRoutine,
        uint32_t ApcContext,
        uint32_t IoStatusBlock,
        uint32_t Buffer,
        uint32_t Length,
        uint32_t ByteOffset,
        uint32_t Key) {
    char *fileName = get_handle_name(env, get_current_proc(env), FileHandle);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_write_file = create_cur_process_file(fileName);
    pandalog_write_entry(&ple);
    free(fileName);
}


Panda__ProcessKey *create_cur_process_key (char *keyname) {
    Panda__ProcessKey *pk = (Panda__ProcessKey *) malloc(sizeof(Panda__ProcessKey));
    *pk = PANDA__PROCESS_KEY__INIT;
    pk->proc = create_panda_process(cur_pid, cur_procname);
    pk->keyname = keyname;
    return pk;
}


// used to determine key from handle, by asid
// keymap[asid][keyhandle] = keyname
std::map < uint64_t, std::map < uint64_t, char * > > keymap;


void save_reg_key(uint32_t KeyHandle, char *keyName) {
    keymap[panda_current_asid(cpu_single_env)][KeyHandle] = strdup(keyName);
}

char *get_keyname(uint32_t KeyHandle) {
    return keymap[panda_current_asid(cpu_single_env)][KeyHandle];
}


// CreateKey  -- creates a new registry key or opens an existing one.
void w7p_NtCreateKey_return(
        CPUState* env,
        target_ulong pc,
        uint32_t pKeyHandle,
        uint32_t DesiredAccess,
        uint32_t ObjectAttributes,
        uint32_t TitleIndex,
        uint32_t Class,
        uint32_t CreateOptions,
        uint32_t Disposition) {
    char *keyName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_create_key = create_cur_process_key(keyName);
    pandalog_write_entry(&ple);    
    uint32_t KeyHandle;
    panda_virtual_memory_rw(env, pKeyHandle, (uint8_t *) &KeyHandle, 4, false);
    save_reg_key(KeyHandle, keyName);
    free(keyName);
}


#if 0
//CreateKeyTransacted  -- creates a new registry key or opens an existing one, and it associates the key with a transaction.
void w7p_NtCreateKeyTransacted_enter(
      CPUState* env,
      target_ulong pc,
      uint32_t KeyHandle,
      uint32_t DesiredAccess,
      uint32_t ObjectAttributes,
      uint32_t TitleIndex,
      uint32_t Class,
      uint32_t CreateOptions,
      uint32_t TransactionHandle,
      uint32_t Disposition) {
    char *keyName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_create_key_transacted = create_cur_process_key(keyName);
    pandalog_write_entry(&ple);    
    save_reg_key(KeyHandle, keyName);
    free(keyName);
}
#endif


// OpenKey -- opens an existing registry key. 
void w7p_NtOpenKey_return (
        CPUState* env,
        target_ulong pc,
        uint32_t pKeyHandle,
        uint32_t DesiredAccess,
        uint32_t ObjectAttributes) {
    char *keyName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_open_key = create_cur_process_key(keyName);
    pandalog_write_entry(&ple);        
    uint32_t KeyHandle;
    panda_virtual_memory_rw(env, pKeyHandle, (uint8_t *) &KeyHandle, 4, false);
    save_reg_key(KeyHandle, keyName);
    free(keyName);
}


// OpenKeyEx -- opens an existing registry key. 
void w7p_NtOpenKeyEx_return (
        CPUState* env,
        target_ulong pc,
        uint32_t pKeyHandle,
        uint32_t DesiredAccess,
        uint32_t ObjectAttributes,
        uint32_t OpenOptions) {
    char *keyName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_open_key_ex = create_cur_process_key(keyName);
    pandalog_write_entry(&ple);        
    uint32_t KeyHandle;
    panda_virtual_memory_rw(env, pKeyHandle, (uint8_t *) &KeyHandle, 4, false);
    save_reg_key(KeyHandle, keyName);
    free(keyName);
}

#if 0
// OpenKeyTransacted -- opens an existing registry key and associates the key with a transaction. 
void w7p_NtOpenKeyTransacted_enter (
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle,
        uint32_t DesiredAccess,
        uint32_t ObjectAttributes,
        uint32_t TransactionHandle) {
    char *keyName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_open_key_transacted = create_cur_process_key(keyName);
    pandalog_write_entry(&ple);        
    save_reg_key(KeyHandle, keyName);
    free(keyName);
}
    

// OpenKeyTransactedEx -- opens an existing registry key and associates the key with a transaction. 
void w7p_NtOpenKeyTransactedEx_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle,
        uint32_t DesiredAccess,
        uint32_t ObjectAttributes,
        uint32_t TransactionHandle) {
    char *keyName = get_objname(env, ObjectAttributes);
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    ple.nt_open_key_transacted_ex = create_cur_process_key(keyName);
    pandalog_write_entry(&ple);        
    save_reg_key(KeyHandle, keyName);
    free(keyName);
}
#endif

void forget_reg_key(uint32_t KeyHandle) {
    keymap[panda_current_asid(cpu_single_env)].erase(KeyHandle);
}


// DeleteKey -- deletes an open key from the registry
void w7p_NtDeleteKey_enter (
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle) {
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    char *keyname = get_keyname(KeyHandle);
    if (keyname) {
        ple.nt_delete_key = create_cur_process_key(keyname);
        pandalog_write_entry(&ple);     
    }
    forget_reg_key(KeyHandle);
}


// QueryKey -- provides information about the class of a registry key, and the number and sizes of its subkeys.
void w7p_NtQueryKey_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle,
        uint32_t KeyInformationClass,
        uint32_t KeyInformation,
        uint32_t Length,
        uint32_t ResultLength) {
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    char *keyname = get_keyname(KeyHandle);
    if (keyname) {
        ple.nt_query_key = create_cur_process_key(keyname);
        pandalog_write_entry(&ple);     
    }
}


Panda__ProcessKeyValue *create_cur_process_key_value (char *keyname, char *valuename) {
    Panda__ProcessKeyValue *pkv = (Panda__ProcessKeyValue *) malloc(sizeof(Panda__ProcessKeyValue));
    *pkv = PANDA__PROCESS_KEY_VALUE__INIT;
    pkv->pk = (Panda__ProcessKey *) malloc(sizeof(Panda__ProcessKey));
    *(pkv->pk) = PANDA__PROCESS_KEY__INIT;
    pkv->pk->proc = create_panda_process(cur_pid, cur_procname);
    pkv->pk->keyname = strdup(keyname);
    pkv->value_name = strdup(valuename);
    return pkv;
}


// QueryValueKey -- routine returns a value entry for a registry key.
void w7p_NtQueryValueKey_enter (
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle,
        uint32_t ValueName,
        uint32_t KeyValueInformationClass,
        uint32_t KeyValueInformation,
        uint32_t Length,
        uint32_t ResultLength) {
    char *vn = read_unicode_string(env, ValueName);
    if (vn) {
        Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
        char *keyname = get_keyname(KeyHandle);
        if (keyname) {
            ple.nt_query_value_key = create_cur_process_key_value(keyname, vn);
            pandalog_write_entry(&ple);
        }
    }
    free(vn);
}

    
// DeleteValueKey -- deletes a value entry matching a name from an open key in the registry. If no such entry exists, an error is returned
void w7p_NtDeleteValueKey_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle,
        uint32_t ValueName) {
    char *vn = read_unicode_string(env, ValueName);
    if (vn) {
        Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
        char *keyname = get_keyname(KeyHandle);
        if (keyname) {
            ple.nt_delete_value_key = create_cur_process_key_value(keyname, vn);
            pandalog_write_entry(&ple);
        }
    }
    free(vn);
}



Panda__ProcessKeyIndex *create_cur_process_key_index(char *keyname, uint32_t index) {
    Panda__ProcessKeyIndex *pki = (Panda__ProcessKeyIndex *) malloc(sizeof(Panda__ProcessKeyIndex));
    *pki = PANDA__PROCESS_KEY_INDEX__INIT;
    pki->pk = create_cur_process_key(keyname);
    pki->index = index;
    return pki;
}
    

// EnumerateKey -- returns information about a subkey of an open registry key.
void w7p_NtEnumerateKey_enter(
       CPUState* env,
       target_ulong pc,
       uint32_t KeyHandle,
       uint32_t Index,
       uint32_t KeyInformationClass,
       uint32_t KeyInformation,
       uint32_t Length,
       uint32_t ResultLength) {
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    char *keyname = get_keyname(KeyHandle);
    if (keyname) {
        ple.nt_enumerate_key = create_cur_process_key_index(keyname, Index);
        pandalog_write_entry(&ple);
    }
}

// EnumerateValueKey -- gets information about the value entries of an open key.
void w7p_NtEnumerateValueKey_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle,
        uint32_t Index,
        uint32_t KeyValueInformationClass,
        uint32_t KeyValueInformation,
        uint32_t Length,
        uint32_t ResultLength) {
    Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
    char *keyname = get_keyname(KeyHandle);
    if (keyname) {
        ple.nt_enumerate_value_key = create_cur_process_key_index(keyname, Index);
        pandalog_write_entry(&ple);
    }
}
    


// SetValueKey -- creates or replaces a registry key's value entry.
void w7p_NtSetValueKey_enter(
        CPUState* env,
        target_ulong pc,
        uint32_t KeyHandle,
        uint32_t ValueName,
        uint32_t TitleIndex,
        uint32_t Type,
        uint32_t Data,
        uint32_t DataSize) {
    char *vn = read_unicode_string(env, ValueName);
    if (vn) {
        Panda__LogEntry ple = PANDA__LOG_ENTRY__INIT;
        char *keyname = get_keyname(KeyHandle);
        if (keyname) {
            ple.nt_set_value_key = create_cur_process_key_value(keyname, vn);
            pandalog_write_entry(&ple);
        }
    }
    free(vn);
}


#endif

bool init_plugin(void *self) {
    printf("Initializing plugin win7proc\n");

#ifdef TARGET_I386

#if 0
    panda_arg_list *args;
    args = panda_get_args("win7proc");
    if (!pandalog) {
        const char *proclog_filename = panda_parse_string(args, "log_prefix", DEFAULT_LOG_FILE);
        char logbuf[260] = {};
        strcpy(logbuf, proclog_filename);
        strcat(logbuf, "_proclog.txt");
        proc_log = fopen(logbuf, "w");
        if(!proc_log) {
            fprintf(stderr, "Couldn't open %s. Abort.\n", logbuf);
            return false;
        }
        strcpy(logbuf, proclog_filename);
        strcat(logbuf, "_prochist.txt");
        proc_hist = fopen(logbuf, "w");
        if(!proc_hist) {
            fprintf(stderr, "Couldn't open %s. Abort.\n", logbuf);
            return false;
        }
    }
#endif
    //    panda_require("syscalls2");

    panda_cb pcb;

    pcb.before_block_exec = before_block_exec;
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);

    PPP_REG_CB("syscalls2", on_NtCreateUserProcess_return, w7p_NtCreateUserProcess_return);
    PPP_REG_CB("syscalls2", on_NtTerminateProcess_enter, w7p_NtTerminateProcess_enter);

    PPP_REG_CB("syscalls2", on_NtCreateFile_enter, w7p_NtCreateFile_enter);
    PPP_REG_CB("syscalls2", on_NtReadFile_enter, w7p_NtReadFile_enter);
    PPP_REG_CB("syscalls2", on_NtDeleteFile_enter, w7p_NtDeleteFile_enter);
    PPP_REG_CB("syscalls2", on_NtWriteFile_enter, w7p_NtWriteFile_enter);

    PPP_REG_CB("syscalls2", on_NtCreateKey_return, w7p_NtCreateKey_return);
    //    PPP_REG_CB("syscalls2", on_NtCreateKeyTransacted_enter, w7p_NtCreateKeyTransacted_enter);
    PPP_REG_CB("syscalls2", on_NtOpenKey_return, w7p_NtOpenKey_return);
    PPP_REG_CB("syscalls2", on_NtOpenKeyEx_return, w7p_NtOpenKeyEx_return);
    //    PPP_REG_CB("syscalls2", on_NtOpenKeyTransacted_enter, w7p_NtOpenKeyTransacted_enter);
    //    PPP_REG_CB("syscalls2", on_NtOpenKeyTransactedEx_enter, w7p_NtOpenKeyTransactedEx_enter);
    PPP_REG_CB("syscalls2", on_NtDeleteKey_enter, w7p_NtDeleteKey_enter);
    PPP_REG_CB("syscalls2", on_NtQueryKey_enter, w7p_NtQueryKey_enter);
    PPP_REG_CB("syscalls2", on_NtQueryValueKey_enter, w7p_NtQueryValueKey_enter);
    PPP_REG_CB("syscalls2", on_NtDeleteValueKey_enter, w7p_NtDeleteValueKey_enter);
    PPP_REG_CB("syscalls2", on_NtEnumerateKey_enter, w7p_NtEnumerateKey_enter);
    PPP_REG_CB("syscalls2", on_NtSetValueKey_enter, w7p_NtSetValueKey_enter);

    return true;
#else
    fprintf(stderr, "Plugin is not supported on this platform.\n");
    return false;
#endif

}

void uninit_plugin(void *self) {
    printf("Unloading win7proc\n");
#ifdef TARGET_I386
    //    fclose(proc_log);

    /*
    for (std::map<procid,uint64_t>::iterator it = bbcount.begin(); it != bbcount.end(); it++) {
        if (it->first.second == UNKNOWN_PID) {
            fprintf(proc_hist, "%s,-1,%" PRId64 "\n", it->first.first.c_str(), it->second);
        }
        else {
            fprintf(proc_hist, "%s,%d,%" PRId64 "\n", it->first.first.c_str(), it->first.second, it->second);
        }
    }
    fclose(proc_hist);
    */
#endif
}
