# COMP3000 F-Locked Project

F-locked is a file locking solution that aims to provide users with a way to
lock files with a password, restricting all file permissions after a lock has
been set. The user is only allowed to lock files it owns. Once a lock has been
set, the user may call the program again to unlock it with the set password.

# How to build and install
```bash
$ git clone https://github.com/zero57/comp3000-f-locked
$ cd comp3000-f-locked
$ make
$ sudo make install
```

# Example
```bash
$ mkdir /tmp/test
$ cd /tmp/test
$ echo "hello world" > myfile
$ ls -l myfile
-rw-r--r-- 1 MYUSER MYUSER 0 Apr 12 18:17 myfile
$ cat myfile
hello world
$ flocked myfile
Enter your password: mypassword
File successfully locked with the given password
$ ls -l myfile
---------- 1 MYUSER MYUSER 0 Apr 12 18:19 myfile
$ cat myfile
cat: myfile: Permission denied
$ flocked myfile
Enter your password to unlock: mypassword
Unlocking file...
Success
$ ls -l myfile
-rw-r--r-- 1 MYUSER MYUSER 0 Apr 12 18:21 myfile
$ cat myfile
hello world
```
