I created a Makefile, and it should be executed by running make followed by ./a.out
<br>
I took the maximum input size as 5000 and declared it as SIZ (macro)

# Spec-3

- If first command is ```hop t1 -``` the previous directory is defined as path to t1

# Spec-4

- If reveal - is performed but there are no previous directories then it prints error like this ```opendir: No such file or directory```.

- No.of files(all executables,files,folder) a directory can have=1024.

- If -l flag is present in the input command the colour coding reflects only on the file or directory name.

# Spec-5

- I have stored the erroneous commands as well.

- I have created a file called "log_file.txt" in the home directory (where shell is invoked) and stored all the commands in that.

# Spec-6

- If there are multiple sleep commands like : ```sleep 3; sleep 5``` then I did that as ```<JohnDoe@SYS:~ sleep : 3 sleep 5: 8s>```

- Did not handle background processes for the commands i have implemented like hop, reveal etc..

# Spec-7

- Virtual memory - in kilobytes


# Spec-10

- If we have some command whose output is directed into newfile.txt but the command ends up erroring, the error is printed newfile.txt?
For example:
If first command executed is : ```hop -```
then the error ```OLDPWD not set``` is printed in the newfile.txt

- Did not handle cases like this : ```echo i>newfile.txt text```

- When I run a command like ```reveal -la > a.txt```, the ANSI colour codes I used will be in the output file.

# Spec-11

- ```hop ..|wc``` when command like this is executed, it will not hop to the parent directory.
- ```hop .. > a.txt``` when command like this is executed, it will hop to the parent directory as well.

- Invalid use of pipes is reported as : ```Invalid use of pipe```

- The commands in a pipeline are run sequentially.


# Spec-12

- Backgrounding for pipes will won't work.

- Commands like ```echo "Hi" & > a.txt``` are not valid in my shell.

# Spec-14

- Sometimes, we need to press ctrl+d 2,3 times if it not logged out of shell immediately.(kills all the processes and logs out of the shell)

# Spec-15

- Here we are going to run a bg command in foreground so if its execution took more than 2 second. For Example sleep 20 was running in bg now i used fg to put in fg so in prompt it shows sleep : [ts]

-  If sleep x is currently running if ctrl d is encountered then it logs out of shell after execution of sleep x.

# Spec-16

- If -n flag is not used, it prints the usage saying "Usage: neonate -n [time_arg]\n"

- The case where n=0 is not handled.
