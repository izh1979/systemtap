/* COVERAGE: read write lseek llseek */
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/uio.h>
#include <sys/syscall.h>

#define STRING1 "red"
#define STRING2 "green"
#define STRING3 "blue"
int main()
{
  int fd;
  loff_t res;
  char buf[64], buf1[32], buf2[32], buf3[32];

  fd = open("foobar1",O_WRONLY|O_CREAT, 0666);
  //staptest// open ("foobar1", O_WRONLY|O_CREAT[[[[.O_LARGEFILE]]]]?, 0666) = NNNN

  write(fd,"Hello world", 11);
  //staptest// write (NNNN, "Hello world", 11) = 11

  write(fd,"Hello world abcdefghijklmnopqrstuvwxyz 01234567890123456789", 59);
  //staptest// write (NNNN, "Hello world abcdefghijklmnopqrstuvwxyz 012345"..., 59) = 59

  pwrite(fd,"Hello Again",11,12);
  //staptest// pwrite (NNNN, "Hello Again", 11, 12) = 11

  lseek(fd, 0, SEEK_SET);
  //staptest// lseek (NNNN, 0, SEEK_SET) = 0

  lseek(fd, 1, SEEK_CUR);
  //staptest// lseek (NNNN, 1, SEEK_CUR) = 1

  lseek(-1, 0, SEEK_SET);
  //staptest// lseek (-1, 0, SEEK_SET) = -NNNN (EBADF)

  lseek(fd, -1, SEEK_END);
  //staptest// lseek (NNNN, -1, SEEK_END) = NNNN

  lseek(fd, 0, -1);
  //staptest// lseek (NNNN, 0, 0x[f]+) = -NNNN (EINVAL)

#ifdef SYS__llseek
  syscall(SYS__llseek, fd, 1, 0, &res, SEEK_SET);
  //staptest// llseek (NNNN, 0x1, 0x0, XXXX, SEEK_SET) = 0

  syscall(SYS__llseek, fd, 0, 0, &res, SEEK_SET);
  //staptest// llseek (NNNN, 0x0, 0x0, XXXX, SEEK_SET) = 0

  syscall(SYS__llseek, fd, 0, 12, &res, SEEK_CUR);
  //staptest// llseek (NNNN, 0x0, 0xc, XXXX, SEEK_CUR) = 0

  syscall(SYS__llseek, fd, 8, 1, &res, SEEK_END);
  //staptest// llseek (NNNN, 0x8, 0x1, XXXX, SEEK_END) = 0

  syscall(SYS__llseek, -1, 1, 0, &res, SEEK_SET);
  //staptest// llseek (-1, 0x1, 0x0, XXXX, SEEK_SET) = -NNNN (EBADF)

  syscall(SYS__llseek, fd, -1, 0, &res, SEEK_SET);
  //staptest// llseek (NNNN, 0x[f]+, 0x0, XXXX, SEEK_SET) = -NNNN (EINVAL)

  syscall(SYS__llseek, fd, 0, -1, &res, SEEK_SET);
  //staptest// llseek (NNNN, 0x0, 0x[f]+, XXXX, SEEK_SET) = NNNN

  syscall(SYS__llseek, fd, 0, 0, (loff_t *)-1, SEEK_SET);
#ifdef __s390__
  //staptest// llseek (NNNN, 0x0, 0x0, 0x[7]?[f]+, SEEK_SET) = -NNNN (EFAULT)
#else
  //staptest// llseek (NNNN, 0x0, 0x0, 0x[f]+, SEEK_SET) = -NNNN (EFAULT)
#endif

  syscall(SYS__llseek, fd, 0, 0, &res, -1);
  //staptest// llseek (NNNN, 0x0, 0x0, XXXX, 0x[f]+) = -NNNN (EINVAL)
#endif

  close (fd);
  //staptest// close (NNNN) = 0

  fd = open("foobar1",O_RDONLY);
  //staptest// open ("foobar1", O_RDONLY[[[[.O_LARGEFILE]]]]?) = NNNN

  read(fd, buf, 11);
  //staptest// read (NNNN, XXXX, 11) = 11

  read(fd, buf, 50);
  //staptest// read (NNNN, XXXX, 50) = 50

  pread(fd, buf, 11, 10);
  //staptest// pread (NNNN, XXXX, 11, 10) = 11

  close (fd);
  //staptest// close (NNNN) = 0

  return 0;
}
