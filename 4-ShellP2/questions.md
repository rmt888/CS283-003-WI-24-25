1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use `fork/execvp` instead of just calling `execvp` directly because `execvp`replaces the current process image with a new one..I think the value that `fork` provides is that without `fork`, the current process would be replaced, and control would not return to the original process. By forking, we create a child process that can execute the new program, while the parent process can continue its execution independently. (labeled in my code)

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails then it returns -1, and errno is set to indicate the error. My implementation handle this scenario by checking the return value of fork(). If it returns -1, I handle the error by printing an error message and returning an appropriate error code to indicate the failure.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() finds the command to execute by the system PATH environment variable. When it searches, it goes through the directories listed in the PATH environment variable. It iterates through each directory specified in PATH and attempts to locate the executable file. If the command is found, execvp() replaces the current process image with the new program.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling wait() in the parent process after forking is to wait for the child process to terminate and to retrieve its exit status.If we didn’t call it then the child process could become a "zombie process", which happens if the parent does not collect the child's exit status. If we didn't call wait(), the child process could become a zombie, consuming system resources.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  The information this provides when we use WEXITSTATUS() is the exit status of the child process from the status code returned by wait() or waitpid().It's important because it allows the parent process to determine how the child process terminated, whether it exited normally and to handle the termination appropriately.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  I didn't use build_cmd_buff() because I couldn't get it to work properly so I had to parse manually. But if I did figure out how to use it then build_cmd_buff() would process the input command line, handling quoted arguments by treating them as single tokens, even if they contain spaces. This is necessary to preserve the integrity of arguments that include spaces, ensuring that commands like echo "hello world" are parsed correctly as a single argument. But I had to use a helper function instead.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  There were a lot of changes that I have to make when came to refactoring my old code. Since I manually parsed, I had to do it again instead of using build_cmd_buff which I know was the goal to use. But in the end I couldn't figure it out.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  The purpose of signals in a Linux system are used to notify processes of events or to control their execution. They are a form of limited interprocess communication that allows processes to send simple notifications to each other. They differ from other forms of interprocess communication (IPC) is such as message queues or shared memory, signals are asynchronous and do not require the sender and receiver to be synchronized.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL(9) immediately terminates a process and cannot be caught or ignored. It is typically used to forcefully kill a process that is unresponsive. SIGTERM(15) requests a process to terminate gracefully, allowing it to clean up resources. It is the default signal sent by commands like kill and is commonly used to request a process to exit. SIGINT(2) happens  sent when the user presses Ctrl+C in the terminal. It interrupts the process, allowing it to handle the signal and terminate or perform other actions.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, it is stopped by the kernel and cannot continue execution until it receives a SIGCONT signal. Unlike SIGINT, SIGSTOP cannot be caught, blocked, or ignored by the process. This is because SIGSTOP is a special signal used to control process execution and is intended to be unconditionally handled by the kernel.
