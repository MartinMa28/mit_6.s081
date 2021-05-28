#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  return p;
}


void find(char *path, char *file_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  fd = open(path, O_RDONLY);
  if (fd < 0) {
    fprintf(2, "find: cannot open current directory\n");
    exit(1);
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot read stat\n");
    close(fd);
    exit(1);
  }

  switch (st.type) {
    case T_FILE:
      if (strcmp(fmtname(path), file_name) == 0) {
        printf("%s\n", path);
      }
      break;
    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p = '/';
      p++;

      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
          continue;
        }

        if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, ".."))) {
          memmove(p, de.name, DIRSIZ);
          p[DIRSIZ] = 0;

          find(buf, file_name);
        }
      }
      break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    exit(0);
  }
  
  printf("find_name: %s\n", argv[1]);
  find(argv[1], argv[2]);
  
  exit(0);
}