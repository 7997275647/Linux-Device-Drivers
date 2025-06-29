# Linux Device Drivers

This repository contains self-developed Linux device drivers. These drivers are created for educational purposes and to demonstrate various concepts of Linux kernel programming.

## Features

- Custom Linux device drivers.
- Examples showcasing kernel module development.
- Code structured for learning and experimentation.

## Prerequisites

- GCC (GNU Compiler Collection) or any compatible C compiler.
- `make` utility for building the kernel modules.
- Basic understanding of C programming and Linux kernel concepts.

## Usage

1. Clone the repository:
    ```bash
    git clone https://github.com/7997275647/Linux-Device-Drivers.git
    ```
2. Navigate to the desired driver directory and build the module:
    ```bash
    make
    ```
3. Load the module into kernal:
    ```bash
    sudo insmod <module_name>.ko
    ```
4. Check kernel logs for output:
    ```bash
    dmesg
    ```
5. Unload the module from kernal:
    ```bash
    sudo rmmod <module_name>
    ```
