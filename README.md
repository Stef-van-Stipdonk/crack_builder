Current feature --> Create dependency graph


## Goal 
Create a basic build system in C to better understand how they work.
The idea is to create the build system in C, such that no other dependencies are needed.
The end goal is to have a single C file, which you add to your directy, compile and from then on it handles builds for you.
This way removing the need for external tools besides gcc.

The motivation behind this is my frustration with build-systems such as CMake, where the specific version matters too much.
And every time a project deviates from your version, you have to install the specified version.
Also I just find the idea of understanding build systems better interesting. <3

## Steps of a build system
- 1. Get root directory + entry point of the build system 
- 2. Recursivly loop through the entire folder structure until al sub-folders are found
- 3. Loop through the folders getting the includes and adding dependencies between files
- 4. Adding the files in the correct order to the gcc command for compiling and linking


## Goals
- [x] Get all files from root directory + sub-directories
- [x] Add ignore file to be able to ignore directories/files
- [ ] Set custom compiler
- [ ] Set custom path/file-name for ignore file
- [x] Add flag command
- [x] Get all files from specified directory
- [x] Add specific files
- [ ] Base dependency graph
- [ ] Add folder structure to the dependency graph
- [ ] Add files to the dependency graph
- [ ] Add dependencies between files to the graph
- [ ] Sort the files in the needed order 
- [x] Pass the files to gcc (gcc is the only compiler option for now)

### Not part of MVP
- [ ] Add a way to handle libraries
- [ ] Hot reloading, you save a file and the program auto rebuilds
