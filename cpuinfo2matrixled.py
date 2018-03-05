# -*- coding: utf-8 -*-
import serial
import psutil

com_dir = '/dev/ttyACM0'

def main():
    interval = 1
    ser = serial.Serial(com_dir, 9600, timeout=1)
    while True:
        cpu_usages = psutil.cpu_percent(interval, percpu=True)
        ser.write(str(cpu_usages))
        interval = (sum(cpu_usages) / len(cpu_usages) / 100) + 0.3
        print(interval)
    ser.close()

# -----------------------
if __name__ == '__main__':
    main()
