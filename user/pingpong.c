#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // p1: parent -> child
  // p2: child -> parent
  int p1[2];
  int p2[2];

  pipe(p1);
  pipe(p2);
  if (fork() == 0) {
    char buf[1];
    int pid;

    close(p1[1]);
    read(p1[0], buf, 1);
    close(p1[0]);
    pid = getpid();
    printf("%d: received ping\n", pid);

    close(p2[0]);
    write(p2[1], buf, 1);
    close(p2[1]);

    exit(0);
  } else {
    close(p1[0]);
    write(p1[1], "W", 1);
    close(p1[1]);

    char buf[1];
    int pid;

    close(p2[1]);
    read(p2[0], buf, 1);
    close(p2[0]);
    pid = getpid();
    printf("%d: received pong\n", pid);

    exit(0);
  }
}