# sudo bash -c 'echo enable_uart=1 >> /boot/config.txt'

## Installing components that might be missing
sudo apt install python3-pip
sudo apt install python3-venv

# Creating virtual environment
sudo python3 -m venv .venv
sudo chmod -R a+rwx .venv

# Installing requirements components
source .venv/bin/activate
python3 -m pip install -r requirements.txt
deactivate

sudo reboot now