/*
    guestbook.h
    Copyright (C) 2020  jotaro-sama

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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