# DTMF Gaming Console System
A gaming platform using DTMF communication between nodes.

## Solution Architecture
There are two projects in this solution:
- [dtmf-lib](console-app/README.md); a library for DTMF-based node communication.
- [console-app](console-app/README.md); an implementation of the library for user and device interaction.

## Directory Structure
The following directory structure is used within a project folder:

```cpp
project
├── bin             : for all executables (applications)
├── obj             : build intermediates
├── lib             : for included libraries
├── include         : for external/API header files
├── src             : for source files
|   ├── namespace1
└── doc             : for documentation
```
