# RAPTOR Reference Generator

This reference generator is intended to be used in conjunction with the rest of the RAPTOR framework. It allows for custom trajectory generation by sending position commands to a position control interface which in turn sends position commands to a drone over MAVLINK. 

## Installation

You will need the following dependencies installed on your system:
- [eProsima FastDDS](https://fast-dds.docs.eprosima.com/en/latest/installation/binaries/binaries_linux.html)
- [MAVSDK](https://mavsdk.mavlink.io/main/en/cpp/guide/installation.html)
- yaml-cpp: This can be installed with `sudo apt install libyaml-cpp-dev` on Ubuntu and `brew install yaml-cpp` on macOS
  
After the dependencies have been installed, clone this repository using: 

```bash
git clone --recursive https://github.com/raptor-ethz/reference_generator
```

Then, in usual cmake fashion, build the project. There are already a number of applications in the app directory which may be helpful as examples. 

## Usage

For launching the reference generator, simply launch the executable in the build directory after building. There should also be a [position control interface](https://github.com/raptor-ethz/position_ctrl_interface) running on the same network and a [FastDDS publisher for the motion capture data](https://github.com/raptor-ethz/mocap_publisher) for the location of all relevant objects. 


If you wish to use the gripper functionality, you will need a [gripper interface](https://github.com/raptor-ethz/gripper_interface) running on an onboard computer. 

