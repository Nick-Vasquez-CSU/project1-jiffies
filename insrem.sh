sudo -p "osc"
sudo dmesg -c
clear

printf "Seconds.c\n"
printf "beginning script...\n\n"
sudo insmod Seconds.ko
sudo rmmod Seconds
sudo dmesg
printf "\n...end of script\n\n"