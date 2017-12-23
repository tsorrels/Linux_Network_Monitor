# socket_monitor

This project is a simple terminal socket monitoring tool with a simple user interface to manage processes with active sockets.

To run in Linux:
```bash
$ make
$ ./netmonitor

```
## Basic UI commanders:

```bash
 'q'		/* gracefully exit  */
 'p'		/* toggle pause updates from netstat  */
 'k'		/* send kill command to selected process */
 'y'		/* confirm command */
 'arrowup'
 'arrowdown'

```

![UI View](/doc/sock_terminal_view.jpg)

##SystemD support:
This program can kill processes managed by systemd. Daemons managed by systemd are terminated using the systemd 'stop' command.  If the processes is a service set to restart upon bootup the user can choose to disable the service to prevent restart. 
