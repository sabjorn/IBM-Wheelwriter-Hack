import serial
import sys, tty, termios
def get_ch():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

ser = serial.Serial("/dev/cu.usbserial-0001", 115200, timeout=0.1)
# single character at a time
stringHeader = chr(0x00) + chr(0x00) + chr(1 >> 8)
while True:
    try:
        ch = get_ch()
        #ser.write(stringHeader.encode() + ch.encode())
        ser.write(ch.encode())
    except:
        break
