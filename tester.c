#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "guestbook.h"
#include <sys/ioctl.h>

#define BUF_SIZE 512

int write_mess(int dev, char *mess) {
        size_t len = strlen(mess);
        int status = write(dev, mess, len);
        return status;
}

int main() {
    int dev;
    dev = open("/dev/guestbook", O_RDWR);
    
    // Write test
    char *mess = "I want to know!";
    int status = write_mess(dev, mess);
    if (status == -1) {
        printf("Writing the message failed. Error code: %s\n", strerror(errno));
        return 1;
    }
    
    // Read test
    char buf[BUF_SIZE];
    if (read(dev, &buf, BUF_SIZE) == -1) {
        printf("Reading the message failed. Error code: %s\n", strerror(errno));
        return 1;
    }
    printf("Message read: %s\n", buf);

    // ioctl test
    // first we write another couple messages
    mess = "These aren't the droids you're looking for...";
    status = write_mess(dev, mess);
    if (status == -1) {
        printf("Writing the message failed. Error code: %s\n", strerror(errno));
        return 1;
    }
    
    mess = "These aren't either.";
    status = write_mess(dev, mess);
    
    // then invoke the ioctl to read the 1st message again
    status = ioctl(dev, IOCTL_CHANGE_MESS_TO_READ, 1);
    if (status < 0) {
        printf("IOCTL_CHANGE_MESS_TO_READ failed. Error code: %s\n", strerror(errno));
        return 1;
    }

    // read again to check if it worked. We should get "I want to know!"
    if (read(dev, &buf, BUF_SIZE) == -1) {
        printf("Reading the message failed. Error code: %s\n", strerror(errno));
        return 1;
    }
    printf("Message read after IOCTL_CHANGE_MESS_TO_READ: %s\n", buf);

    // now we test the other ioctl, we clear the guestbook and should get the 
    // number of cleared elements
    size_t elems_se;
    ssize_t elems_ret = ioctl(dev, IOCTL_CLEAR_GUESTBOOK, &elems_se);
    if (elems_ret < 1) {
        printf("IOCTL_CLEAR_GUESTBOOK failed. Error code: %s\n", strerror(errno));
        return 1;
    }
    printf("Returned number of elements is: %lu\n", elems_ret);
    printf("Side-effect number of elements is: %lu\n", elems_se);

    // Let's also read again to check if the guestbook is really empty. The read should fail.
    if (read(dev, &buf, BUF_SIZE) == -1) {
        printf("Reading the message failed. Error code: %s\n", strerror(errno));
    }

    close(dev);

}