1. Update the kernel in recovery mode:
ssh-keygen -f "/home/user/.ssh/known_hosts" -R "c220g5-120116.wisc.cloudlab.us"
lsblk -o NAME,SIZE,FSTYPE,UUID,MOUNTPOINT
sudo mount -t ext4 /dev/sda3 /mnt/
sudo mount --bind /dev /mnt/dev
sudo mount --bind /proc /mnt/proc
sudo mount --bind /sys /mnt/sys
sudo chroot /mnt
grep "menuentry" /boot/grub/grub.cfg
vi /etc/default/grub
"Advanced options for Ubuntu>Ubuntu, with Linux 4.15.0-101-generic"
Ubuntu, with Linux 6.9.0-init+
Ubuntu, with Linux 6.9.0-passthrough+
Ubuntu, with Linux 6.9.0-mediated-pmu-passthrough+
Ubuntu, with Linux 6.9.0-passthrough-ddeferral-strategy+
Ubuntu, with Linux 6.7.0-rc1+
Ubuntu, with Linux 6.7.0-rc1-passthrough+
Ubuntu, with Linux 5.5.0-rc3
Ubuntu, with Linux 4.18.20+
Ubuntu, with Linux 4.18.20-correct+
update-grub
exit
sudo umount -l /mnt/dev /mnt/proc /mnt/sys
sudo umount /mnt
