#!/bin/bash

# Create gpg.key
sudo mkdir -p /etc/apt/keyrings
curl -fsSL https://packages.grafana.com/gpg.key | sudo tee /etc/apt/keyrings/grafana.asc > /dev/null
echo "deb [signed-by=/etc/apt/keyrings/grafana.asc] https://packages.grafana.com/oss/deb stable main" | sudo tee /etc/apt/sources.list.d/grafana.list

sudo apt update
sudo apt-get -y install libncurses-dev \
                        gawk \
                        flex \
                        bison \
                        openssl \
                        libelf-dev \
                        libssl-dev
