#! /bin/bash

USER=test_user

# Create the user with a home directory and bash as the shell
useradd -m -s /bin/bash $USER

# Set a user password
passwd $USER

# Add the user to the sudoers file
usermod -aG sudo $USER

# This next part must be run from the user's home directory
cd /home/$USER

# Create an SSH folder and make the user the owner
mkdir .ssh

# Make an authorized_keys file for the user to put their SSH key(s) in
touch .ssh/authorized_keys

# Make the user the owner of .ssh
chown -R $USER:$USER .ssh

# Set UFW to allow access to the ports used for simulation
ufw allow 4001

# Pull the simulator binary package
# TODO
