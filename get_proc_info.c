#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_SIZE 256

int pid = -1;
char pName[128] = "";

//function to insert the kernel module
void insert_module() {
    char command[256];
    if (pid > 0) {
        //command construction for inserting the kernel module with the given pid
        sprintf(command, "sudo insmod /home/efe/Desktop/Excercises/new_place/proc_info_module.ko upid=%d", pid);
    } else if (strcmp(pName, "") != 0) {
        //command construction for inserting the kernel module with the given pname
        sprintf(command, "sudo insmod /home/efe/Desktop/Excercises/new_place/proc_info_module.ko upname=%s", pName);
    }
    system(command);
}

//function to remove the kernel module
void remove_module() {
    char command[256];
    //command construction to remove the kernel module
    sprintf(command, "sudo rmmod /home/efe/Desktop/Excercises/new_place/proc_info_module.ko");
    system(command);
}

//print contents of /proc/proc_info_module file
void printProcFile() {
    FILE *kernel_module;
    char buffer[BUFFER_SIZE];

    kernel_module = fopen("/proc/proc_info_module", "r");
    if (kernel_module == NULL) {
        perror("Failed to open /proc/proc_info_module");
        return;
    }

    while (fgets(buffer, BUFFER_SIZE, kernel_module) != NULL) {
        printf("%s", buffer);
    }
    fclose(kernel_module);
}

int main(int argc, char *argv[]) {
    //check if the correct number of command line arguments are passed
    if (argc != 3) {
        printf("wrong usage: -> %s -pid <processID> OR %s -pname <processName>\n", argv[0], argv[0]);
        return 1;
    }

    if (strcmp("-pid", argv[1]) == 0) {
        pid = atoi(argv[2]);
        //check pid being valid
        if (pid <= 0) {
            printf("-pid parameters must be non-negative integer\n");
            return 1;
        }
    } else if (strcmp("-pname", argv[1]) == 0) {
        strcpy(pName, argv[2]);
        //check pname being valid
        if (strcmp(pName, "") == 0) {
            printf("-pname parameters must be non-empty string\n");
            return 1;
        }
    } else {
printf("wrong usage: -> %s -pid <processID> OR %s -pname <processName>\n", argv[0], argv[0]);
return 1;
}

    insert_module();
    printProcFile();
    remove_module();

    return 0;
}