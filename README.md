## README

### Makefile Execution
- I created a Makefile, and it should be executed by running:
  ```bash
  make
  ./a.out
  ```
- I took the maximum input size as 5000 and declared it as `SIZ` (macro):
  ```c
  #define SIZ 5000
  ```

### Specifications

#### Spec-3
- If the first command is:
  ```bash
  hop t1 -
  ```
  The previous directory is defined as the path to `t1`.

#### Spec-4
- If `reveal -` is performed but there are no previous directories, it prints an error like this:
  ```bash
  opendir: No such file or directory
  ```
- The number of files (all executables, files, and folders) a directory can have is:
  ```c
  #define MAX_FILES 1024
  ```
- If the `-l` flag is present in the input command, the color coding reflects only on the file or directory name.

#### Spec-5
- Erroneous commands are also stored.
- A file called `log_file.txt` is created in the home directory (where the shell is invoked) and all the commands are stored in it:
  ```c
  FILE *log_file = fopen("log_file.txt", "a");
  if (log_file != NULL) {
      fprintf(log_file, "%s\n", command);
      fclose(log_file);
  }
  ```

#### Spec-6
- If there are multiple `sleep` commands like:
  ```bash
  sleep 3; sleep 5
  ```
  They are displayed as:
  ```
  <JohnDoe@SYS:~ sleep : 3 sleep 5: 8s>
  ```
- Background processes are not handled for the implemented commands like `hop`, `reveal`, etc.

#### Spec-7
- Virtual memory is measured in kilobytes.

#### Spec-10
- If a command’s output is redirected to `newfile.txt` but the command results in an error, the error is printed in `newfile.txt`. For example:
  ```bash
  hop -
  ```
  Results in:
  ```
  OLDPWD not set
  ```
  Being written to `newfile.txt`.
- Cases like:
  ```bash
  echo i>newfile.txt text
  ```
  Are not handled.
- When a command like:
  ```bash
  reveal -la > a.txt
  ```
  Is executed, the ANSI color codes used will appear in the output file.

#### Spec-11
- When a command like:
  ```bash
  hop ..|wc
  ```
  Is executed, it will not hop to the parent directory.
- However:
  ```bash
  hop .. > a.txt
  ```
  Will hop to the parent directory.
- Invalid use of pipes is reported as:
  ```
  Invalid use of pipe
  ```
- The commands in a pipeline are run sequentially.

#### Spec-12
- Backgrounding for pipes does not work.
- Commands like:
  ```bash
  echo "Hi" & > a.txt
  ```
  Are not valid in this shell.

#### Spec-14
- Sometimes, you may need to press `Ctrl+D` 2 or 3 times if the shell does not log out immediately (kills all processes and logs out of the shell).

#### Spec-15
- When a background command is brought to the foreground, if its execution takes more than 2 seconds (e.g., `sleep 20` running in the background is brought to the foreground using `fg`), the prompt will display:
  ```
  sleep : [ts]
  ```
- If `sleep x` is running and `Ctrl+D` is encountered, the shell logs out after the execution of `sleep x`.

#### Spec-16
- If the `-n` flag is not used, the shell prints the usage:
  ```
  Usage: neonate -n [time_arg]\n
  ```
- The case where `n=0` is not handled.

