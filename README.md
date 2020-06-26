# ToyModule
A tiny loadable kernel module used for testing and learning. The module implements a simple guest book: by writing on the device, a message can be left, and by reading from the device the latest inserted message can be read.

## Usage

To read the kernel log: 
```
tail -f /var/log/kern.log
```

The `test.sh` script does a quick test compiling and loading the module, and also unloading it and deleting the generated files when it's done.

If you want to do the testing manually:

- To load the kernel module, compile it with `make`, then a `.ko` file is produced. Run:
```
sudo insmod guestbook.ko
```

- To unload it:
```
sudo rmmod guestbook
```

- Once the module is running, you can compile (just run `gcc -o tester tester.c`) and run `tester.c` to see how it does.

- If you want to quickly remove the files generated with `make`, run `make clean`.


## Creating the device file (now done from code)

The module now creates (and deletes) its own device file via C code, and we have a `tester.c` program to test the module, so **the section below is not needed anymore**. I will keep it there in case anyone finds it useful.

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

