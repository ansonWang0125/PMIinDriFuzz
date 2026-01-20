#!/bin/bash
set -x

WDIR="/storage/PMIinDriFuzz/scripts/sh/build-env"

# Update lib
# Create gpg.key
sudo mkdir -p /etc/apt/keyrings
curl -fsSL https://packages.grafana.com/gpg.key | sudo tee /etc/apt/keyrings/grafana.asc > /dev/null
echo "deb [signed-by=/etc/apt/keyrings/grafana.asc] https://packages.grafana.com/oss/deb stable main" | sudo tee /etc/apt/sources.list.d/grafana.list
sudo apt update

# Build taskfile command runner
# TODO-L3: What is MPR and what is snapd in ubuntu
# TODO-L3: I choose tasklist as build system since it is new for me.
curl -1sLf 'https://dl.cloudsmith.io/public/task/task/setup.deb.sh' | sudo -E bash

sudo apt install task

# Build environment setting
pushd ${WDIR}
cp ./.bashrc ~/.bashrc
popd

# Build local file structure
mkdir -p "/storage/PMIinDriFuzz/build"