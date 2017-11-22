import serial
import random

#start bits
st_cmd = chr(127)
st_rsp = chr(126)

#command bits
cmd_init = chr(1)


#Buffers
last_sent_id = ''
last_received_id = ''
usb_rx_buffer = ''
usb_tx_buffer = ''

class KioskDriver:

    def __init__(self, com, baud):
        self.ser = serial.Serial(com,baud)


    def serial_start(com, baud):
        ser = serial.Serial(com,baud)

    def test_send(self, message):
        #self.ser.write(message.encode('utf-8'))
        self.ser.write(message)

    #[START] [ID] [LENGTH] [CMD] [DATA_1…DATA_N] [CHECKSUM]
    #Only pass [CMD] and all [DATA] bytes
    def send_cmd(msg):
        length = chr(len(msg.encode('utf-8')) + 1)
        message_id = id_byte()
        chksm = cmd_checksum(message_id, length, msg)
        message = st_cmd + message_id + length + msg + chksm
        #ser.write(message.encode('utf-8'))
        print('message=',message.encode('utf-8'))

    #[START] [ID] [LENGTH] [DATA_1…DATA_N] [CHECKSUM]
    def send_rsp(msg):
        message
        ser.write(message.encode('utf-8'))

    def id_byte():
        last_sent_id = chr(random.randint(1,125))
        return last_sent_id

    #Check Sum = ~ (ID + LENGTH + CMD + DATA_1 + … DATA_N)
    def cmd_checksum(ID, length, msg):
        checksum = ord(ID) + ord(length)
        for s in msg:
            checksum += ord(s)
        return chr(checksum)

    #Check Sum = ~ (ID + LENGTH + DATA_1 + … DATA_N)
    def rsp_checksum(packet):
        checksum = packet
        return checksum
