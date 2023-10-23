/*
 * sched.c - initializes struct for task 0 anda task 1
 */


#include <list.h>
#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

//#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
//#endif

extern struct list_head blocked;

struct list_head freequeue;
struct list_head readyqueue;

struct task_struct * idle_task;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}

void task_switch(union task_union*t)
{
  //implementar task switch, m'he quedat aqui
}

int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
  printk_color("IDLE PROCESS RUNNING", B_MAGENTA ,F_BLACK);
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{	
//<<<<<<< divendres
	//Get an available task_union from the freequeue to contain the characteristics of this process
	struct list_head *aviable = list_first(&freequeue);
	list_del(aviable);
	struct task_struct *idle_ts= list_head_to_task_struct(aviable);

	//Assign PID 0 to the process
	idle_ts-> PID = 0;

	//Initialize field dir_pages_baseAaddr with a new directory to store the process address space
	allocate_DIR(idle_ts);

  long *stack = (long*)((union task_union *) idle_ts);
  stack[KERNEL_STACK_SIZE - 1] = (unsigned long) cpu_idle;
  stack[KERNEL_STACK_SIZE - 2] = (unsigned long) 0;

	//keep (in a new field of its task_struct) the position of the stack
	//where we have stored the initial value for the ebp register. This value will be loaded
	//in the esp register when undoing the dynamic link.
  idle_ts -> ksp = stack[KERNEL_STACK_SIZE -2];

  idle_task = idle_ts;
=======
	struct task_struct *idle_task = list_head_to_task_struct(&freequeue);
  	list_del(&freequeue);

  	idle_task->PID = 0;
  	allocate_DIR(idle_task);
	
	union task_union * idle_task_union = (union task_union *)idle_task;

	//Store in the stack of the idle process the address of the code that it will execute (address of the cpu_idle function).
	(*idle_task_union).stack[KERNEL_STACK_SIZE-1] = (unsigned long)cpu_idle;
	//Store in the stack the initial value that we want to assign to register ebp when undoing the dynamic link (it can be 0)
	(*idle_task_union).stack[KERNEL_STACK_SIZE-2] = (unsigned long)0;

	//Finally, we need to keep (in a new field of its task_struct) the position of the stack
	//where we have stored the initial value for the ebp register. This value will be loaded
	//in the esp register when undoing the dynamic link.
	idle_task->ini_val_ebp = &(idle_task_union->stack[KERNEL_STACK_SIZE-2]);
//>>>>>>> main
}

void init_task1(void)
{
  struct list_head *aviable = list_first(&freequeue);
	list_del(aviable);
	struct task_struct *init = list_head_to_task_struct(aviable);

	//Assign PID 0 to the process
	init -> PID = 0; 

  // Initialize a new directory for the process address space
  allocate_DIR(init);
  // Complete the initialization of its address space
  set_user_pages(init);

  // no se quin es
  tss.esp = ((long*)((union task_union *) init))[KERNEL_STACK_SIZE-1];
  // tss.esp0 = ((long*)((union task_union *) init))[KERNEL_STACK_SIZE-1];
  // tss.esp1 = ((long*)((union task_union *) init))[KERNEL_STACK_SIZE-1];
  // tss.esp2 = ((long*)((union task_union *) init))[KERNEL_STACK_SIZE-1];
  set_cr3(init->dir_pages_baseAddr);
}


void init_sched()
{
  // initialize free queue
  INIT_LIST_HEAD(&freequeue);
  // fill with all tasks
  {
    int i = 0;
    while ( NR_TASKS > i++)  list_add(&(task[i].task.list), &freequeue);
  }

  // initialize readyqueue (with nothing)
  INIT_LIST_HEAD(&readyqueue);
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}
