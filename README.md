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

## Example

We will use `apps/grab_object` as example to illustrate how the code works.

First, we import the items we want to use in the code:

```c++
#include "Gripper.h"
#include "Item.h"
#include "Quad.h"
```

Then, we create a FastDDS domain participant. This is the default participant which will represent this program in the global FastDDS space.

```c++
std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");
```
Then, we can create the participants for each of the items we are using. For each participant, we are passing a participant ID in form of a string, the default participant we created in the previous step, the FastDDS topic we will get the data from and in the case of the Quad, a FastDDS topic we will publish data under. 

In terms of data flow, we are listening to topics that are published by the [mocap publisher](https://github.com/raptor-ethz/mocap_publisher) where the topic names are also defined. The `pos_cmd` position commands are received by the [position control interface](https://github.com/raptor-ethz/position_ctrl_interface).
```c++
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");
  Item box("Box", dp, "mocap_srl_box");
```

Then, we check if we are receiving data from the stand and the quad and try to initialize the quad. We try to take off with the quad. If any of the checks fail, we exit the process.

```c++
 if (!quad.checkForData() || !stand.checkForData() || !box.checkForData()) {
    std::cerr << "Fatal Error (main:23) Terminate Process." << std::endl;
    return 1;
  }
  quad.setState(initialized);

  if (!quad.takeOff()) {
    // TODO: try again after 10s
    std::cerr << "Fatal Error (main:29) Terminate Process." << std::endl;
    return 1;
  }
```
Then, we invoke the method `quad.quickSwoop`. This will put the gripper into the grasping position, the drone will automatically position itself according to the length of the swooping motion and then fly to the object, actuate the gripper and fly away again.

Let's also consider the arguments: we are passing the box object and the gripper object. The box object will provide the target position of the object we will pick up. The gripper object serves for actuation. Then, we are passing the length of the swoop. This determines how far the quad positions itself from the object for the initial positioning in meters - if it it set to 2 metres, that also means the total length of the swooping motion will be 4 metres. Then, we can also pass some offsets in metres for small adjustments of the position. Then, we are also passing the height that we want to swoop from, here, 2 metres, and finally, a time for which we will remain at the position of the object to grasp it, here, 50 milliseconds. At last, we also pass the angle for the gripper to close to with respect to the center of the gripper. 

```c++
  quad.quickSwoop(box, gripper, 2, 0.05, 0.03, -0.02, 2, 50, 3); // potato
```
Then, we release the object and close the gripper again.
```c++
gripper.set_angle_sym(60);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  gripper.set_angle_sym(5);
```

At last, we land in the stand and exit the process.
```c++
 quad.land(stand);

  return 0;
```