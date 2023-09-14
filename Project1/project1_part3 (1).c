#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday
{
    char *name;
	int month;
	int day;
	int year;
    
    struct list_head list; //next pointer?
};


/**
 * The following defines and initializes a list_head object named birthday_list
 */

static LIST_HEAD(birthday_list); //head node

int part3_init(void)
{
    
    printk(KERN_INFO "***Loading Proejct1 Part3 Module***\n");

    /* Create a linked list containing five struct birthday elements*/

    /* NOTE:THE NAME OF FIRST STRUCT BIRTHDAY SHOULD BE YOUR OWN NAME*/

   
    struct birthday *person;
    person = kmalloc(sizeof(*person), GFP_KERNEL); //node
    person->day = 7;
    person->month = 3;
    person->year = 1993;
    person->name = "Blake Caraballo";

    INIT_LIST_HEAD(&person->list);
    list_add_tail(&person->list, &birthday_list);
    

    /* Traverse the linked list  */
    struct birthday *ptr;
    list_for_each_entry(ptr, &birthday_list, list){
        printk(KERN_INFO "Name = %s Birthday: Month: %d Day: %d Year %d",ptr->name,ptr->month,ptr->day,ptr->year);
        printk("testing");
    }

    
   

    return 0;
}

void part3_exit(void) {

    printk(KERN_INFO "***Removing Project1 Part3 Module***\n");

    /* Remove the elements from the linked list and return the free memory back to the kernel */

    struct birthday *ptr, *next;

     list_for_each_entry_safe(ptr,next,&birthday_list,list){
        printk(KERN_INFO "Removing: Name = %s Birthday: Month: %d Day: %d Year %d",ptr->name,ptr->month,ptr->day,ptr->year);
        list_del(&ptr->list);
        printk(KERN_INFO "Removing: Name = %s Birthday: Month: %d Day: %d Year %d",ptr->name,ptr->month,ptr->day,ptr->year);
        kfree(next);
     }
   

}

module_init( part3_init );
module_exit( part3_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Proejct1 Part3");
MODULE_AUTHOR("GSU_CSC4320_6320_TH_Spring2023");

