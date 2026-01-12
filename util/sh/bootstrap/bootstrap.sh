#! /bin/bash

# Use this script to boot this system on cloud lab.
set -x

STORAGE="/storage"

# Generate ssh key
ssh-keygen -t ed25519 -C "r13922187@ntu.edu.tw"
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
cat .ssh/id_ed25519.pub

read -n 1 -p "Please copy the public key to GitHub ssh dashboard, if completed, enter \"y\"!"

# Create extend storage on cloud lab
sudo mkdir /storage
sudo /usr/local/etc/emulab/mkextrafs.pl ${STORAGE}
sudo chown -R ${USER} ${STORAGE}

# Build our system
pushd ${STORAGE}
git clone git@github.com:ansonWang0125/PMIinDriFuzz.git
popd