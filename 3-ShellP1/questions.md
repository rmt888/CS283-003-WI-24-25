1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  By using fgets() to get user input in the main while loop is a good choice because it allows us to specify the maximum number of characters to read, preventing buffer overflow vulnerabilities. fgets() reads the entire input line, including spaces and tabs, which helps pass the test with commands with multiple arguments. fgets() helps for handling user input in our program. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  In dsh_cli.c, I use malloc() to allocate memory for cmd_buff instead of defining a fixed-size array to provide flexibility in handling user input of different types of lengths. Allocating a fixed-size array requires us to define its size at compile time, which can lead to some problems. If the array is too large, it wastes memory. If it's too small, it may not work with longer inputs which can cause buffer overflows. malloc() helps allocate the exact amount of memory needed at runtime, ensuring efficient memory usage and work with inputs of different types of lengths.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing spaces from each command in the build_cmd_list() function is necessary for accurate command parsing and execution. Leading spaces can cause the command name to not be recognized, which will lead to test failures. Trailing spaces can interfere with argument parsing, causing arguments to be misread or unintended arguments to be included. By removing these spaces, we ensure that each command and its arguments are processed correctly, preventing errors during execution. If we didn't trim spaces then the commands wouldn't be found or executed improperly due to incorrect parsing. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Implementing redirection in a custom shell involves parsing commands to identify operators like >, <, and >>, and managing file descriptors accordingly. 
    
    Output Redirection (>): Directs the standard output of a command into a file, overwriting its contents. For example, ls > filelist.txt saves the directory listing into filelist.txt. 

    Input Redirection (<): Takes input for a command from a file instead of the keyboard. For exmaple, sort < unsorted.txt reads data from unsorted.txt to sort its contents. 

    Append Redirection (>>): Appends the standard output of a command to the end of a file without overwriting it. For example, echo "New entry" >> log.txt adds "New entry" to log.txt.
    
    Challenges include handling file permissions, ensuring files are created or accessed correctly, and managing errors if files cannot be opened. Also, the shell must ensure proper closure of file descriptors and handle cases where files do not exist or cannot be read. 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection and piping both manage the flow of data in the shell but serve different purposes. Redirection involves directing the input or output of a command to or from a file. For example, using > to send output to a file or < to read input from a file. Piping, denoted by the | symbol, connects the stdout of one command directly to the stdin of another, allowing the output of one command to be processed immediately by the next. For example, ls | grep "txt" takes the output of ls and passes it to grep to filter for lines containing "txt". While redirection deals with files, piping deals with direct communication between processes. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Separating stdout and stderr is important because they serve different purposes. Stdout is intended for regular output data, while stderr is used for error messages. Keeping them separate allows us and scripts to tell the difference between successful output and error messages, helping with better error handling and debugging. If they were combined, it would be challenging to parse outputs correctly, especially in automated scripts that depend on specific output formats. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our custom shell should handle command errors gracefully. When a command fails, it usually outputs an error message to stderr. By default, stderr and stdout are separate, but sometimes we might want to merge them, especially when redirecting outputs to a file or another command. A way to merge them, we can implement syntax to redirect stderr to stdout using 2>&1. For example, command > output.txt 2>&1 redirects both stdout and stderr to output.txt. Implementing this requires parsing the command line to detect such redirection instructions and appropriately setting up the file descriptors before executing the command. Challenges include correctly parsing and setting file descriptors, handling edge cases, and ensuring that the redirections do not interfere with the intended behavior of commands.