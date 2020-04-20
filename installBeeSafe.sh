#!/bin/sh

sudo apt-get update -y
sudo apt-get upgrade -y

sudo apt-get install git -y
sudo apt-get install build-essential -y
sudo apt-get install cmake openssl libcpprest-dev -y

# Not necessary packages, but are required to recreate the entire project
# And are also quite handy
sudo apt-get install flex bison doxygen graphviz libreadline6-dev lcov  -y

# Only handy packages
sudo apt install screen npm -y

sudo apt-get clean

# Uncomment and edit if you prefer
# And if weirdly you haven't happen to have done already
#git config --global user.name "yourusername"
#git config --global user.email "youremail@email.com"
#git clone https://github.com/itsBelinda/ENG5220-2020-Team13/
