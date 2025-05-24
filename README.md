# C-Shell

A custom Unix-like shell implemented in C, supporting command execution, process management, directory navigation, logging, piping, redirection, and several built-in utilities.

---

## Overview

**C-Shell** is a feature-rich shell designed to mimic and extend the behavior of standard Unix shells. It supports both built-in and external commands, advanced process management (foreground/background), command history, colored directory listings, piping, redirection, and custom utilities like `hop`, `reveal`, `seek`, `proclore`, `log`, and more.

---

## Features and Functionalities

- **Command Execution**: Run both built-in and external commands.
- **Foreground/Background Processes**: Launch and manage jobs in the foreground or background.
- **Process Management**: Bring jobs to foreground/background, kill jobs, and view running/stopped jobs.
- **Directory Navigation**: Use `hop` to change directories, including shortcuts for home and previous directories.
- **Colored Directory Listing**: Use `reveal` for `ls`-like listings with color coding and detailed info.
- **File/Directory Search**: Use `seek` to search for files/directories recursively with flexible flags.
- **Command Logging**: All commands (including erroneous ones) are logged to `log_file.txt` in the home directory.
- **Command History**: View, purge, or execute previous commands using the `log` command.
- **Piping and Redirection**: Support for `|`, `<`, `>`, and `>>` operators.
- **Signal Handling**: Handles `Ctrl+C` (SIGINT), `Ctrl+Z` (SIGTSTP), and child process signals.
- **Custom Utilities**:
  - `proclore`: View process details.
  - `neonate`: Monitor the most recently created process.
  - `iMan`: Fetch online man pages.
  - `activities`: List all jobs started by the shell.
  - `ping`: Send signals to processes.
- **Alias and Custom Functions**: Support for aliases and shell functions via `myshrc`.
- **ANSI Color Support**: Colorful prompts and outputs for better readability.

---

## Available Commands

### Built-in Commands

- `hop [dir] ...`  
  Change directory. Supports `~`, `-`, relative, and absolute paths.
- `reveal [-a] [-l] [path]`  
  List directory contents with color and optional details.
- `seek [-d] [-f] [-e] <name> [dir]`  
  Search for files/directories recursively.
- `proclore [pid]`  
  Show process info for the shell or a given PID.
- `log [purge|execute <index>]`  
  Show, purge, or execute from command history.
- `activities`  
  List all jobs started by the shell.
- `fg <pid>`  
  Bring a background job to the foreground.
- `bg <pid>`  
  Resume a stopped job in the background.
- `neonate -n <seconds>`  
  Monitor the most recently created process.
- `ping <pid> <signal>`  
  Send a signal to a process.
- `iMan <command>`  
  Fetch and display the online man page for a command.

### External Commands

- Any valid system command (e.g., `ls`, `cat`, `sleep`, etc.) can be executed.

### Operators

- `;` — Command separator.
- `|` — Pipe output of one command to another.
- `<`, `>`, `>>` — Input/output redirection.

---

## Usage

### Compilation

```bash
make
./a.out
```

### Prompt

The shell prompt displays as:

```
<username@system:~>
```

### Example Commands

```bash
hop ~/Documents
reveal -la
seek -f -e main.c ~
log
log execute 2
sleep 5 &
activities
fg 12345
bg 12345
proclore
proclore 12345
neonate -n 2
iMan ls
```

### Redirection and Piping

```bash
reveal -l > files.txt
cat files.txt | grep main
hop .. | wc
```

---

## Configuration

### Aliases and Functions

You can define aliases and shell functions in the `myshrc` file:

```sh
alias reveall="reveal -l"
alias home="hop ~"

mk_hop() {
    if [ -z "$1" ]; then
        echo "Usage: mk_hop <directory>"
        return 1
    fi
    mkdir -p "$1"
    hop "$1"
}
```

---

## Notes and Limitations

- The maximum input size is `5000` characters (`#define SIZ 5000`).
- Directory listing supports up to 1024 files per directory.
- Backgrounding for pipes is not supported.
- Some advanced shell features (like `echo i>file text`) are not handled.
- ANSI color codes will appear in redirected output files.
- Sometimes, you may need to press `Ctrl+D` multiple times to exit the shell.
- When a background command is brought to the foreground and runs for more than 2 seconds, the prompt will display the command and its duration.
- All commands, including errors, are logged in `log_file.txt` in the home directory.

---

## Existing README Content

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

---
