# PMIinDriFuzz

This work is targeted on optimizing the "fuzzing loop" in kernel fuzzing. We want to leverage PMI to collect execution inforamtion of driver, current work is based on syzkaller.

---

## Overview
- Key capability #1
- Key capability #2
- Supported platforms / architectures (if relevant)

---

## Starup routine
- ./scripts/sh/build-env/prerequisites.sh
- git submodule init
- git submodule update
- exit
- task build-host
- sudo reboot
- task build-syzkaller-env
- task build-guest-env
- task build-image
- task build-qemu-env
- task compile-guest
- task compile-qemu
-

## Setup

TODO-L4: Complete filled commit for kernel, qemu, syzkaller.
Kernel version:

## File Structure
.
├── build/
├── config/
├── fuzzer/
├── guest-kernel/
├── host-kernel/
├── out/
├── qemu/
├── scripts/
└── tutorial/

## TODO Priority Levels

This project annotates TODO items with five priority levels:

| Level | Tag         | Meaning                                                     |
|------:|-------------|-------------------------------------------------------------|
| L0    | `TODO-L0`   | Must be completed in this project. |
| L1    | `TODO-L1`   | Nice-to-have in this project. |
| L2    | `TODO-L2`   | Interesting project worth research for this project |
| L3    | `TODO-L3`   | Interesting project for me but not related to this project. |
| L4    | `TODO-L4`   | Micro and tedious, but need to completed in recent. |

