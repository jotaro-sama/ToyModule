# ToyModule
A tiny loadable kernel module used for testing and learning. The module implements a simple guest book: by writing on the device, a message can be left, and by reading from the device the latest inserted message can be read.

## Usage

To read the kernel log: 
```
tail -f /var/log/kern.log
```

To load the kernel module, compile it, then a `.ko` file is produced. Run:
```
sudo insmod guestbook.ko
```

To unload it:
```
sudo rmmod guestbook
```

Once it's running we want to create a node for the device in `/dev`:
```
cd /dev
#          dev name c is for char device
sudo mknod guestbook c <major_number> 0
```


To see it in action do for example:
```
cat /dev/guestbook
```

