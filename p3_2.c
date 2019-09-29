
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/sched/signal.h>
#include <linux/rculist.h>
/* This function is called when the module is loaded. */
//#define NON_KERNEL
#ifdef NON_KERNEL
#include <malloc.h>
#define SIMPLE_MALLOC(Type,Size)\
    malloc(sizeof(Type)*Size)
#define SIMPLE_FREE(ptr)\
    free(ptr)
#define list_entry_rcu(x,y,z) (struct task_struct*)x
///fake ^^
#else

#define SIMPLE_MALLOC(Type,Size)\
    kmalloc(sizeof(*person)*Size,GFP_KERNEL)
#define SIMPLE_FREE(ptr)\
    kfree(ptr)
#endif
struct birthday{
    int day,month,year;
    struct list_head list;
};
static LIST_HEAD(List_head);
void dfs(struct task_struct *x){
    printk(KERN_INFO,"%d %s %d\n",x->pid,x->comm,x->state);
    struct task_struct *task;
    struct list_head *iterator;
    list_for_each(iterator,&x->children){
        task = list_entry(iterator,struct task_struct,sibling);
        dfs(task);
    }
}
int simple_init(void)
{
    printk(KERN_INFO "Loading Module This is temp.\n");
    struct task_struct *root = &init_task;
    root = next_task(root);
    dfs(root);

    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {

    printk(KERN_INFO "Removing Module ,This is temp.\n");
    cleanup_module();
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");


