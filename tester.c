#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 512

int main() {
    int dev;
    dev = open("/dev/guestbook", O_RDWR);
    char *mess = "I want to know!";
    size_t len = strlen(mess);
    if (write(dev, mess, len) == -1) {
        printf("Writing the message failed. Error code: %s\n", strerror(errno));
        return 1;
    }

    char buf[BUF_SIZE];
    if (read(dev, &buf, BUF_SIZE) == -1) {
        printf("Reading the message failed. Error code: %s\n", strerror(errno));
        return 1;
    }
    printf("Message read: %s\n", buf);

}