#include <stdio.h>
#include <syscall-nr.h>
#include <stdint.h>
#include <stdbool.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "threads/vaddr.h"

typedef int (*syscall_func) (uint32_t, uint32_t, uint32_t);
static syscall_func syscall_handlers[20]; /* Array of syscall functions */
static void syscall_handler(struct intr_frame *);

void syscall_init(void) {
	intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
	syscall_handlers[SYS_EXIT] = (syscall_func)syscall_exit;
	syscall_handlers[SYS_WRITE] = (syscall_func)syscall_write;
	syscall_handlers[SYS_EXEC] = (syscall_func)syscall_exec;
	syscall_handlers[SYS_HALT] = (syscall_func)syscall_halt;
	syscall_handlers[SYS_WAIT] = (syscall_func)syscall_wait;
	syscall_handlers[SYS_CREATE] = (syscall_func)syscall_create;
	syscall_handlers[SYS_REMOVE] = (syscall_func)syscall_remove;
	syscall_handlers[SYS_OPEN] = (syscall_func)syscall_open;
	syscall_handlers[SYS_CLOSE] = (syscall_func)syscall_close;
	syscall_handlers[SYS_READ] = (syscall_func)syscall_read;
	syscall_handlers[SYS_FILESIZE] = (syscall_func)syscall_filesize;
	syscall_handlers[SYS_SEEK] = (syscall_func)syscall_seek;
	syscall_handlers[SYS_TELL] = (syscall_func)syscall_tell;
}
/*can be modified*/
static void syscall_handler(struct intr_frame *f UNUSED) {
	int *syscall_num = f->esp;	//get the syscall_num from the stack
	syscall_func func =  syscall_handlers[*syscall_num];		//get the function
	int ret =  func(*(syscall_num + 1), *(syscall_num + 2), *(syscall_num + 3));
	f->eax = ret;	//put return of the function in f->eax
	return;
}

/* Reads a byte at user virtual address UADDR.
 UADDR must be below PHYS_BASE.
 Returns the byte value if successful, -1 if a segfault
 occurred. */
static int get_user(const uint8_t *uaddr) {
	if (!is_user_vaddr(uaddr))
		return -1;
	int result;
	asm ("movl $1f, %0; movzbl %1, %0; 1:"
			: "=&a" (result) : "m" (*uaddr));
	return result;
}

/* Writes BYTE to user address UDST.
 UDST must be below PHYS_BASE.
 Returns true if successful, false if a segfault occurred. */
static bool put_user(uint8_t *udst, uint8_t byte) {
	if (!is_user_vaddr(udst))
		return false;
	int error_code;
	asm ("movl $1f, %0; movb %b2, %1; 1:"
			: "=&a" (error_code), "=m" (*udst) : "q" (byte));
	return error_code != -1;
}
void syscall_halt(void){
	shutdown_power_off();
}
/*To be implemented*/
void syscall_exit(int status){

}
pid_t syscall_exec(const char *file){
	return 0;
}
int syscall_wait(pid_t pid){
	return 0;
}
bool syscall_create(const char *file, unsigned initial_size){
	return true;
}
bool syscall_remove(const char *file){
	return true;
}
int syscall_open(const char *file){
	return 0;
}
int syscall_filesize(int fd){
	return 0;
}
int syscall_read(int fd, void *buffer, unsigned length){
	return 0;
}
int syscall_write(int fd, const void *buffer, unsigned length){
	return 0;
}
void syscall_seek(int fd, unsigned position){

}
unsigned syscall_tell(int fd){

}
void syscall_close(int fd){

}
