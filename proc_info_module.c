#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/mm.h>

#define PROC_FILE_NAME "proc_info_module"

static int upid = -1;
static char upname[TASK_COMM_LEN];

// sudo insmod proc_info_module.ko upid=<upid> upname=<upname>
// sudo insmod proc_info_module.ko upid=1234
// sudo insmod proc_info_module.ko upname=myprocess
// sudo insmod proc_info_module.ko upname=myprocess upid=1234
// cat /proc/proc_info_module
// sudo rmmod proc_info_module


module_param(upid, int, S_IRUGO);
MODULE_PARM_DESC(upid, "Process ID");
module_param_string(upname, upname, TASK_COMM_LEN, S_IRUGO);
MODULE_PARM_DESC(upname, "Process Name");

static struct proc_dir_entry *proc_file_entry;

// Helper function to print error message to dmesg log and /proc/proc_info_module
static void print_error(struct seq_file *m, const char *msg,const char *msg2)
{
    pr_err("%s\n", msg);  // Write to dmesg log
    seq_puts(m, msg2);    // Write to /proc/proc_info_module
}

static int proc_info_show(struct seq_file *m, void *v)
{
    struct task_struct *task;
    bool found = false;

    // Find the process by ID or name
    for_each_process(task) {
        if (task->pid == upid || (strcmp(task->comm, upname) == 0)) {
            found = true;
            break;
        }
    }

    if (!found) {
        print_error(m, "No process found with the specified ID or name","No process found with the specified ID or name\n");
        return 0;  // Return success, but indicate no process found
    }

    // Print process information
    seq_printf(m, "Name: %s\n", task->comm);
    seq_printf(m, "PID: %d\n", task->pid);
    seq_printf(m, "PPID: %d\n", task->real_parent->pid);
    seq_printf(m, "UID: %d\n", task_uid(task).val);
    seq_printf(m, "Path: /proc/%d\n", task->pid);
    seq_printf(m, "State: %ld\n", task->state);

    if (task->mm) { // check if it has a valid memory descriptor
        // Calculate memory usage in KB
        unsigned long rss = get_mm_counter(task->mm, MM_FILEPAGES) +
                            get_mm_counter(task->mm, MM_ANONPAGES);
        seq_printf(m, "Memory usage: %lu KB\n", rss << (PAGE_SHIFT - 10));
    }

    return 0;
}

static int proc_info_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_info_show, NULL);
}

static const struct file_operations proc_info_fops = {
    .open = proc_info_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init proc_info_module_init(void)
{
    // Create the /proc file
    proc_file_entry = proc_create(PROC_FILE_NAME, 0, NULL, &proc_info_fops);
    if (!proc_file_entry) {
        printk(KERN_ERR "Failed to create /proc file: %d\n", -ENOMEM);
        return -ENOMEM;
    }

    printk(KERN_INFO "/proc/%s created\n", PROC_FILE_NAME);
    return 0;
}

static void __exit proc_info_module_exit(void)
{
    // Remove the /proc file
    remove_proc_entry(PROC_FILE_NAME, NULL);

    printk(KERN_INFO "/proc/%s removed\n", PROC_FILE_NAME);
}

module_init(proc_info_module_init);
module_exit(proc_info_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Dynamic Kernel Module - Process Information");
