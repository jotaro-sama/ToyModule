# Simple helper script. For now, just tail -f /var/log/kern.log on one terminal
# and run this on another one.

make
gcc -o tester tester.c

sudo insmod guestbook.ko hotel_name=Darian
./tester

sudo rmmod guestbook
rm tester
make clean
