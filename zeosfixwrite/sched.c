/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

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


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{	
	//Get an available task_union from the freequeue to contain the characteristics of this process
	struct list_head *aviable = list_first(&freequeue);
	list_del(aviable);
	struct task_struct *task1 = list_head_to_task_struct(aviable);

	//Assign PID 0 to the process
	task1 -> PID = 0;

	//Initialize field dir_pages_baseAaddr with a new directory to store the process address space
	allocate_DIR(task1);

	/*task_union de tsk */->stack[1023] = (unsigned long)(cpu_idle);
	/*task_union de tsk */->stack[1022] = (unsigned long)0;
	//keep (in a new field of its task_struct) the position of the stack
	//where we have stored the initial value for the ebp register. This value will be loaded
	//in the esp register when undoing the dynamic link.

	idle_task = task1;

}

void init_task1(void)
{
}


void init_sched()
{

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

