Serial communication setup
1. sudo raspi-config
2. Interface options -> Serial port -> No -> Yes -> Ok -> Finish -> Reboot
3. sudo apt install python3-pip
4. python -m pip install pyserial --break-system-packages

status_detection service setup
1. copy status_detection.py to /home/myiot/python/status_detection.py
2. copy status_detection.service to /etc/systemd/system/status_detection.service
3. sudo chmod 644 /etc/systemd/system/status_detection.service
4. sudo systemctl daemon-reload
5. sudo systemctl enable status_detection.service
6. sudo reboot