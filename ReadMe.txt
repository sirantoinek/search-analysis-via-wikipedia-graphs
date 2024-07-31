READ ME:

DOWNLOAD binaries.zip FROM HERE: 
https://drive.google.com/file/d/1HBnSRNCwfCoaA9rP8cGIjFhIeS1TEznV/view?usp=sharing

The files in binaries.zip must be extracted into the bin folder for the program to function. This limitation is due to the LFS 1gb storage limit on github.

When compiling the Cmake application, set the "Working directory" in your IDE to the project root folder (should be "Search-Analysis-via-Wikipedia-Graphs").
Otherwise, the binaries will not be found as the relative path will be invalid.

Also make sure that you are using the visual studio toolchain in your Cmake profile as when using MinGW, dependencies are not found.

WARNING:
Be aware that when running, the program may use up to 8 GB of memory.
This is because the dataset is MASSIVE at 163 million rows. (The source csv file was 8.9 GB)