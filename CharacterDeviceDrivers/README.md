# Character Device Drivers

This repository contains examples and implementations of Linux character device drivers. These drivers demonstrate how to interact with hardware or virtual devices using the Linux kernel's character device interface.

## Building and Loading Modules

refer [this](../README.md) to build and load modules.

## Usage
Once the module is loaded, you can interact with the character device through the corresponding device file (e.g., `/dev/<device_name>`). Use standard file operations like `read`, `write`, and `ioctl` to communicate with the device.