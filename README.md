CS 455: Visual Info Processing Term Project
Monika Roznere
Michael Correale

##Description
This program is intended to solve Captcha images using logistical regression and one vs all classification. Our algorithems are not implemented correctly, but the program can/run after installing the c++ library Armadillo, which contains matrix related functions.

https://github.com/conradsnicta/armadillo-code/

###Directory Breakdown
Termpaper.pdf - Term Paper containing our write up and algorithem
generator/ - Directory containing our image generator code
solver/ - Directory containing out captcha solver
solver/minimal.cpp - Main code of our project. Contains logistical regression and One vs All implementations
solver/training.cpp - Helper code for our projects. Contains data for generating/loading test data.
solver/images/ - All images used for the training
solver/dataSet.txt - Data set used for training/testing
solver/testImage.bmp - Sample test image to test with


HOW TO RUN:

First, make sure opencv, and armadillo are installed on your system
go into solver/, and run make

Program can be called using 
./training <datafile> <testing image>

Example:
./training dataSet.txt testImage.bmp


