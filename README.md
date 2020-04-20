# ENG5220-2020-Team13: BeeSafe
## Status:
[![build](https://travis-ci.com/itsBelinda/ENG5220-2020-Team13.svg?token=kxeDEXY2kiCv968c3pfm&branch=unit_tests)](https://travis-ci.com/itsBelinda/ENG5220-2020-Team13)
[![codecov](https://codecov.io/gh/itsBelinda/ENG5220-2020-Team13/branch/unit_tests/graph/badge.svg?)](https://codecov.io/gh/itsBelinda/ENG5220-2020-Team13)
<br>Currently in development.
# Archived branch

## Description
The BeeSafe project is a low-cost, open source tracking device. It is designed to keep children safe by constantly monitoring their location, and alerting parents if the child leaves the designated safe zones. It consists of a Raspberry Pi 3B, and a purpose made PCB containing a u-blox SARA-G350 GSM module, through which the Pi continuously gathers location data, and processes whether the child is in a location it is supposed to be at that time, and if the child is at a place they should not be, the device sends a text message to the parent with the child’s location they can track through a web interface. 

## Installation
To build our project, first clone the BeeSafe Github repository with
<br>
`
git clone https://github.com/itsBelinda/ENG5220-2020-Team13
`
<br>
Next, run the installation script `installBeeSafe.sh` in the root folder of the repository: this will install all required dependencies. **Note:** For optional dependencies, you can choose to install them or not by uncommenting or commenting them in the installation script.
<br>
Next, run the build script `buildBeeSafe.sh` in the root folder of the repository: this will build the project and is ready to be ran. 
> For more in-depth build instructions, please see the corresponding <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/wiki/Build-Guide"> wiki page </a>.

## Usage
To run the project as live project, ensure the build instructions were followed to create the bootloader service. If that was correctly implemented, the software should run upon the next startup.
<br>
To run the project in development, navigate into the folder `/software/BeeSafePI/build/src/` in the repository, and type
```
	./BeeSafePI
```
> For more in-depth usage instructions, please see the corresponding <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/wiki/User-Guide"> wiki page </a>.

## Documentation
An in-depth guide on the build and usage of the system, as well as detailed component descriptions and project management steps can be found on the <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/wiki"> project wiki</a>. The API documentation created with Doxygen can be found <a href="">here</a>. [TODO]

## Contributing to this project
Do you like this project? We strongly believe in the power of Open Source, so there are many ways you could contribute to our project!

### Report a bug
We are aware of the possibility that there may be bugs in the project, which were not immediately apparent for us (or <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/wiki/Project-Management%3A-Further-Work">were</a> :P ). If you find one, do let us know by opening an <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/issues">issue</a>, but please read our <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/blob/master/.github/ISSUE_TEMPLATE/bug_report.md">Bug Report Guidelines</a> first, to make it easier to work out the problem!

### Request a feature
Is there a feature you would like to see implemented in this project? We welcome feature requests, however before you open one, please read our <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/blob/master/.github/ISSUE_TEMPLATE/feature_request.md">guidelines</a> for submitting one!

### Pull requests
We welcome other contributions, too, so pull requests of patches, improvements, etc. are encouraged! By default we ask you pull the `master` branch. We only ask you to be mindful, and keep pull requests in scope and relevant. If you would be interested in contributing a larger piece of work and/or it may not be in line the above tips, please do get in touch with a <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/blob/master/README.md#credits">maintainer</a> for a discussion!

### Coding conventions
For the coding conventions we use, please look at the <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/wiki/Contributions#coding-conventions">wiki page</a> discussing it in depth!

## Feedback
If you have any, more general feedback about our project, give us a shout by filling out this <a href="https://forms.gle/tGHM2jB7GBWfdgk3A">form</a>!

<br>
Feel free to also reach out to us via email, on beesafe.uofg [at] gmail.com!

## Credits
Made with :heart: by <b>Team BeeSafe</b>:<br>
<a href="https://github.com/itsBelinda/">Belinda Kneubuhler</a><br>
<a href="https://github.com/szugyizs/">Zsuzsanna Szugyi</a><br>
<a href="https://github.com/dans-acc/">Daniels Vasiljevs</a><br>

## License
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
<br>
This project is licensed under the <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/blob/master/LICENSE">MIT license</a>.

