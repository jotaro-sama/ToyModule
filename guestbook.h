
#ifndef GUESTBOOK_H
#define GUESTBOOK_H

#include <linux/ioctl.h>

#define MAJOR_NUM 100

// The users can change what message they want to read.
// Message numbers start from 1. 0 is default, last message will be read.
// Fails if there's no such message.
#define IOCTL_CHANGE_MESS_TO_READ _IOWR(MAJOR_NUM, 0, size_t)

// Using _IOR since the device will let the userspace know how many 
// messages were there before the clear
#define IOCTL_CLEAR_GUESTBOOK _IOR(MAJOR_NUM, 1, size_t*)

#define DEV_FILE_NAME "guestbook"

#endif