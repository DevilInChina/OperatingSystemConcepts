#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/types.h>
/* This function is called when the module is loaded. */
//#define NON_KERNEL
#ifdef NON_KERNEL
#include <malloc.h>
#define SIMPLE_MALLOC(Type,Size)\
    malloc(sizeof(Type)*Size)
#define SIMPLE_FREE(ptr)\
    free(ptr)
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
int simple_init(void)
{
    printk(KERN_INFO "Loading Module This is temp.\n");
    struct birthday* person;
    int i;
    for( i = 0 ; i < 5 ; ++i){
        person = SIMPLE_MALLOC(*person,1);
        INIT_LIST_HEAD(&person->list);
        person->day = 1+i;
        person->month = 7+i;
        person->year = 1994+i;
        list_add_tail(&person->list,&List_head);
    }
    //struct list_head* iterator;
    struct birthday*iterator;
    list_for_each_entry(iterator ,&List_head,list){
        printk(KERN_INFO"%02d/%02d/%04d\n",iterator->day,iterator->month,iterator->year);
    }
    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
    struct birthday*iterator,*next;
    list_for_each_entry_safe(iterator,next ,&List_head,list){
        printk(KERN_INFO"%02d/%02d/%04d deleting...\n",iterator->day,iterator->month,iterator->year);
        list_del(&iterator->list);
        SIMPLE_FREE(iterator);
    }
    printk(KERN_INFO "Removing Module ,This is temp.\n");
    cleanup_module();
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

