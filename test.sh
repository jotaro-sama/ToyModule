# Simple helper script. For now, just tail -f /var/log/kern.log on one terminal
# and run this on another one if the device has already been created.

make
gcc -o tester tester.c

sudo insmod guestbook.ko hotel_name=Darian
sudo ./tester

sudo rmmod guestbook
rm tester
make clean
