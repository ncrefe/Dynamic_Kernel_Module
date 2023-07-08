# Process Information Logger
This repository contains a dynamic kernel module and a wrapper user space application that allows you to log details of a process in your Ubuntu operating system. The dynamic kernel module, implemented in C and named proc_info_module.c, communicates with the kernel space to retrieve process information based on either process ID (PID) or process name. The user space application, implemented in C and named get_proc_info.c, provides a convenient interface to interact with the kernel module, allowing you to insert, remove, and retrieve information from the module.

## Dynamic Kernel Module
The dynamic kernel module (proc_info_module.c) provides the following information about a process:

+ Name: Process name.
+ PID: Process ID.
+ PPID: PID of the process's parent.
+ UID: User identifier of the process.
+ Path: The path of the process in /proc.
+ State: The current state of the process (e.g., running, interruptible, uninterruptible, stopped).
+ Memory Usage: Calculated memory usage of the process in kilobytes (KB) when the process is running.

### Module Parameters
The user space application can manage the following module parameters:

+ upid: A non-negative integer that specifies the user process ID (PID).
+ upname: A string that specifies the user process name.

### Usage Flow
To utilize the dynamic kernel module, follow these steps:

1. Acquire the process ID or name to be passed as an argument.
2. Open the /proc file named proc_info_module.
3. If no processes with the provided ID or name are found, an appropriate error message will be printed using strerror() or perror(). The error will be logged in the /proc file, and the program will exit with an exit value of 2.
4. Otherwise, retrieve information about the process.
5. If the process state is running, calculate the memory usage.
6. When the user space application reads the /proc file, log the message to the /proc file.
7. Exit the program with an exit value of 0.

Please note that the /proc file will be removed when the kernel module is removed. If an error occurs during any of the above steps, an appropriate error message will be printed using strerror() or perror(). The error will be logged in the /proc file, and the program will exit with an exit value of 1.

## Wrapper User Space Application
The wrapper user space application (get_proc_info.c) is responsible for inserting and removing the module from the operating system, passing parameters to the kernel module, reading information from the /proc file, and printing the log messages in the terminal.

### Command Line Arguments
The user space application accepts the following command line arguments:

+ argv[1]: File path of the user space application.
+ argv[2]: Argument type, which can be -pid or -pname.
+ argv[3]: If -pid is given, a non-negative integer representing the process ID should be provided. If -pname is given, a string representing the process name should be provided.

Make sure to pass the correct number of command line arguments. The application should work with only one parameter. If both -pid and -pname information are provided, an error will be displayed.

### Usage Flow
To use the wrapper user space application, follow these steps:

1. Get the process ID or name argument from the terminal.
2. Pass the parameter to the kernel module while inserting the kernel object to the operating system.
3. Read the log messages written by the kernel module from the /proc file.
4. Print the log messages in the terminal.
5. Remove the kernel module.
6. Exit the program with an exit value of 0.

If an error occurs during any of the above steps, an appropriate error message will be displayed, and the program will exit with an exit value of 1.
