/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

extern int zeos_ticks;
extern struct list_head freequeue;
extern struct list_head readyqueue;
int num_processos = 2; //number of processes created in the entire simulation
                       //2 because we have init and idle at the begining
int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  //error if there's no free tasks
  if (list_empty(&freequeue))
    return -ENOMEM;

  //Get a free task_struct for the process
  struct list_head *freetask = list_head(&freequeue);
  list_del(freetask);

  //copy the parent’s task_union to the child
  struct task_struct child_ts = list_head_to_task_struct(freetask);
  union task_union *child_tu;
  child_tu = (union task_union *)child_ts;
  copy_data((union task_union *)current(), child_tu, sizeof(union task_union));
  
  //Initialize field dir_pages_baseAddr with a new directory to 
  //store the process address space using the allocate_DIR routine

  allocate_DIR(child_tu->task);

  //allocate as many phisical pages as DATA pages exists
  int free_pag[NUM_PAG_DATA];
  page_table_entry *father_PT = get_PT(current());
  page_table_entry *child_PT = father_PT;

  for(int i = 0; i < NUM_PAG_DATA; i++){

    int free_ph_page = alloc_frame();

    if(free_ph_page >= 0) {
      set_ss_pag(child_PT, PAG_LOG_INIT_DATA + i, free_ph_page);
      free_pag[i] = free_ph_page;
    }

    else { //if there aren't NUM_PAG_DATA free pages, then undo allocates
      
      for(--i; i >= 0; --i) 
        free_frame(free_pag[i]);

      list_add_tail(child_tu->task, &freequeue);
      return -ENOMEM;

    }
  }

  //(in Spanish, too much for my english)
  //Queremos que el proceso hijo tenga para los campos DATA+STACK direcciones logicas
  //diferentes a las del padre, entonces desde el padre mapeamos direcciones logicas vacias, es decir que esten despues del rango 
  //de direcciones del propio padre a las direcciones fisicas del hijo (solo de DATA+STACK) y 
  //copiamos a saco el contenido que habia en DATA+STACK del padre
  for(int i = PAG_LOG_INIT_DATA; posi < (PAG_LOG_INIT_DATA + NUM_PAG_DATA); ++i){
    set_ss_pag(father_PT, i + NUM_PAG_DATA + NUM_PAG_CODE, get_frame(child_PT, i));
    //copias el contenido de las direcciones fisicas de DATA+STACK del 
    //padre a las direcciones fisicas DATA+STACK del hijo, las cuales 
    //son las direcciones logicas que estan a continuacion del rango 
    //de direcciones logico del proceso padre :D
    copy_data((void*)(i << 12), (void*)((NUM_PAG_DATA + NUM_PAG_CODE + i) << 12),PAGE_SIZE);
    del_ss_pag(father_PT, i + NUM_PAG_DATA + NUM_PAG_CODE);
  }

  set_cr3(get_DIR(current())); //flush TLB
    
  child_tu->task.PID = num_processos;
  ++num_processos;

  init_stats(&child_ts);
  /*                      
                          ______-17
    @handler              
    SAVE_ALL              ______-16
              pushl %gs; \
              pushl %fs; \
              pushl %es; \    
              pushl %ds; \
              pushl %eax; \
              pushl %ebp; \   
              pushl %edi; \
              pushl %esi; \
              pushl %ebx; \
              pushl %ecx; \   
              pushl %edx; \ ____ -5
    eip
    cs
    flags                     
    esp                       
    ss
  */
  child_tu->stack[KERNEL_STACK_SIZE - 19] = 0;
  child_tu->stack[KERNEL_STACK_SIZE - 18] = (long unsigned int)&ret_from_fork;
  child_tu->task.ksp=(long unsigned int)&(child_tu->stack[KERNEL_STACK_SIZE - 19]);

  list_add_tail(&(child_tu->task.list), &readyqueue);

  PID = (child_tu->task.PID);

  return PID;
}

void sys_exit()
{  
}


int sys_write(int fd, char * buffer, int size) 
{
	//fd: file descriptor. In this delivery it must always be 1.
	//buffer: pointer to the bytes.
	//size: number of bytes.
	//return ’ Negative number in case of error (specifying the kind of error) and
	//the number of bytes written if OK. 

	//check fd
	int error = check_fd (fd, ESCRIPTURA);
	if (error < 0) return -EBADF;
	//check size
	if (size < 0) return -EINVAL;
	//check buffer
	if (!access_ok(VERIFY_WRITE, buffer, size)) return -ENOMEM;

  int real_size = 0;
  int frame = 256;
  char buffer_so[frame];

  while (0 < size)
  {
    if (size < frame) frame = size;
    error = copy_from_user(buffer, buffer_so, frame);
  	if (error < 0) return error;
    
    int ret = sys_write_console(buffer_so, frame);
    if (ret < 0) return ret;
  
    buffer += frame;
    real_size += ret;
    size -= frame;
  }

  return real_size;
}

int sys_gettime()
{
	return zeos_ticks;
}

int ret_from_fork() 
{
  return 0;
}
