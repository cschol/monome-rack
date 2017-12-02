monome-rack — VCVRack plugin for monome hardware
---

### What is this?

This is a work-in-progress repo for a plugin package for the [VCVRack open-source virtual modular synthesizer](https://github.com/VCVRack/Rack) that implements (some of) the Eurorack modules and control hardware manufactured by monome. This is an unofficial community-driven port and no support, warranty, or affiliation is implied.

### What is included?

* A virtual version of the venerable [grid 128](https://monome.org/docs/grid/), a flexible 16x8 grid controller
* The [white whale](https://monome.org/docs/modular/whitewhale/) grid-enabled probabilistic step sequencer
   * Uses either a virtual grid, or a real grid connected via serialosc
   * Currently limited to one instance per rack, see [issues](#issues) below

![white whale and virtual grid in VCVRack](https://user-images.githubusercontent.com/712405/33520799-fbdd1e30-d78f-11e7-96ef-f3effc5edacd.png)

### Why is this?

In order of importance, the initial goals of this project were/are:

* Provide an easier environment for developing, debugging, and testing new and improved firmware features for the monome Eurorack modules
* Allow monome users to practice, experiment, and/or record while traveling light or otherwise away from their hardware
* Bring new users into the monome ecosystem
* "Because it was there"

### Who did this?

Original hardware and firmware code in linked submodules is by [monome](monome.org). The VCVRack-specific code in this repository is a separate effort, and again, no support or warranty is implied.

[@dewb](https://github.com/Dewb) wrote the virtual grid module, the initial mock hardware layer, and ported white whale. 

Additional community contributions and feedback are very welcome! Discuss either in an [issue here](/../../issues) or in the [lines thread](https://llllllll.co/t/white-whale-in-vcvrack/10337). 


## Issues

* The state of working memory and the flash memory are persisted to the VCVRack patch, but something is wrong with the preset glyphs.
* The WW firmware code is designed to run by itself in a microcontroller and stores state in a lot of file-scope static variables. Running this code unaltered in a Rack plugin means that every plugin instance shares the same state variables, so using more than one WW module in a patch does not work as expected. 
* Unplugging a real monome’s USB cable while it’s connected to a virtual module causes a crash.
* The `mock_hardware` library that bridges between the C++ API of VCVRack and the C API of the AVR microcontroller is a simple collection of egregious hacks. It needs a redesign before it gets extended much further, i.e. to other module firmwares.
* Currently, for both virtual and physical grids, only modern varibright 128 models are supported. Adding virtual 64/256/512 grids should be straightforward; the widget code is already implemented, the FTDI hacks just need to be extended to do proper device identification. It's also a goal to support non-varibright and other older models, in both hardware and software, for better compatibility testing. 
* No performance analysis has been done. Changes were made with intent to be efficient, and initial testing sounds good, but no deep study of the timing or the CPU utilitization has been performed yet. There is likely room for improvement.

For more, see [the discussion thread here](https://llllllll.co/t/white-whale-in-vcvrack/10337).


## How to build

For now, this is a source-only plugin, no binary package is provided. The plugin has been built and tested on OS X 10.12 and Windows 10. Linux is currently untested.

1. Clone [VCVRack](https://github.com/VCVRack/Rack) 0.5.0 or later and follow its build instructions.
2. Clone this repo into the `plugins` folder under VCVRack.
4. Change into the `monome-rack` folder and clone submodules with `git submodule update --init --recursive`
5. Build with `make` or open the monome-rack folder in Visual Studio Code and select `Tasks > Run Build Task`.

## License

The original monome firmware is licensed under the GPL version 2.

The new code in this repository is also licensed under the GPL version 2.

The base64, oscpack, and serialosc libraries are licensed from their authors via GPL-compatible licenses according to the LICENSE file in each subfolder under lib/.
