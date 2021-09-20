# MIMOSA

MIMOSA - **M**ultiple **I**nput **M**ultiple **O**utput **S**witches. The mission of the project is to capture user's intent via as many physiological triggers (press of button, blink of an eye, flick of a cheek, shake of a toe) as possible, and provide an easy interface to transduce the intent into digital actions.

At this time MIMOSA is focused on enabling students, makers, and DIY community to quickly prototype accessible and inclusive input devices. In the future the list of complete functional and tested devices (together with bills of materials and assembly+programming instructions) will be expanding. Please see the section "Reference Devices" below for some of the devices currently prototyped.

# System Features
1. **Sensitive and adaptive** - focus on the users abilities, that could be changing over time.
2. **Maker-friendly** - the system is comprised of components that are easy to get (via Sparkfun, Adafruit, Digikey), and easy to program (support for Arduino, CircuitPython).
3. **Extandable** - multi-modal sensing modules can be daisy chained together to enable multi-trigger applications (gaming, chorded typing, advanced device control, etc.)
4. **Low-cost**. The system is breathing accessibility not only with it's functions, but also with the price point.
5. **Well-documented reference designs** - mouse control, fast-typing and accessible keyboard, gaming, Android device control, ChromeOS device control.

While MIMOSA is not an officially Google supported project, it is a great start for engineers, makers, students, hobbyists into versatile and accessible input devices.

# Example Development and Deployment Process
1. Define sensing modalities and strategies for triggering. 
2. Put together a system with MIMOSA modules to provide means for sensing and user feedback. Two tips:
  a. Often it is a good idea to start with a block diagram and a high level design document.
  b. Iterate quickly. Engage with the users early.
3. Test the system with the user and adjust sensitivity settings, modules placement, mechanical arrangements, etc.

# Custom Modules - Design Blueprints Available Throught This Repository
![blocks](/images/mimosa_blocks.jpg)
1. **High dynamic range force sensor**. This board can be used to monitor force application along 1 axis with an impressive sensitivity (well below 1g), and good dynamic range such that a stronger force can still be registered without saturation.
2. **Joystick interface**. The goal of the board is to provide a solderless interface board compatible with the tiny joysticks available on the market. Tested with [1](https://www.adafruit.com/product/3103?gclid=CjwKCAjwo4mIBhBsEiwAKgzXOLo4MXqrfOciSVSgiRNq5dNijwRDPIEh7QklhfaW0UjwFYq1YEu6FxoC2MIQAvD_BwE), [2](https://www.amazon.com/gp/product/B08397JQZG).
3. **D-sub 9-pin joystick interface** inspired by FreedomWing. Allows the connection of a widely used DB9 joysticks into the Stemma QT aka QWIIC infrastructure.

# Off-the-shelf Module Ideas
As MIMOSA is based on a widely adopted solderless communication and power distribution system (Adafruit Stemma QT, or Sparkfun QWIIC), there are many modules developed for all sort of applications that can be used together with MIMOSA modules. 

Examples of such modules:
- IMU modules, magnetometers, accelerometeres, gyroscopes.
- Proximity sensors
- Interface boards

# Reference Devices
Based on the modules developped there are a few reference implmentations of ready to go devices for end user applications. All devices are ready to go out of the box. All devices also come Arduino and/or CircuitPython compatible, so they are very easy to modify for different applications, different abilities, and expanded to comprise more sophisticated systems.

![devices](/images/mimosa_reference_devices.jpg)

Following devices are provided as a reference: 
1. Dual force buttons with USB-C interface that supports standards HID mouse/keyboard protocol.
2. Tiny joystick (tinyjoy). Just a standalone small joystick device with USB-C interface.
  a. HID joystick implementation.
  b. HID mouse implmentation.

More complex devices available through separate repositories:

4. Palmouse - magnetometer-based airmouse (inclination -> mouse speed) with an additional joystick, two MIMOSA force buttons, and USB interface.
5. Chorda - simiar form factor as Palmouse, this device is built for both hands, with two additional buttons on each hand. The key idea is that it cab be used as a mouse and as a chorded keyboard!

# Navigating the Repository
- The directories with names starting with "pcba_" are the source files of the custom modules described above (schematics + board layour + bom + fabrication files). They also include accompanying arduino libraries and examples where applicable. 
- The directories with the names starting with "device" contain the source files and code examples needed to put together complete devices. This usually includes a mechanical enclosure, photos, and documentation.

# Source Files
1. Schematic diagram + board layout (Altium).
2. Enclosure (STLs, STEP file exported from Autocad Fusion 360).
3. Firmware libraries and sketches compatible with Arduino IDE are included.

# License
This project is licensed under the Apache v2.0 License - see the [LICENSE.md](LICENSE.md) file for details

# Contributing
Thank you for your interest - please review CONTRIBUTING.md!