import serial
import numpy as np

class KioskDriver:

    #Main state dict
    state = {"flash":0, "opmode": "run", "statusindicator":"green", "fan_top":0, "fan_bot":0, "prox":0, "helpsw":0, "upscharge":0, "upsdischarge":0, "upsfault":0, "tampertop":0, "tamperbot":0, "systemreset":0}

    lastmsg = []

    #start bits
    st_cmd = 255
    st_rsp = 254

    #command bits
    cmd_reset = 2
    cmd_setFlash = 7
    cmd_setStatusIndicator = 10
    cmd_setFan1 = 32
    cmd_setFan2 = 34

    rsp_ok = 1
    rsp_badcheck = 2


    def __init__(self, com, baud):
        self.ser = serial.Serial(
        port = com,
        baudrate = baud,
        timeout = 0,
        )

    def serial_start(com, baud):
        ser = serial.Serial(com,baud)

    #Command Instruction Set
    def reset(self):
        self.send_cmd(self.cmd_reset,[1])

    def setFlash(self, data):
        self.send_cmd(self.cmd_setFlash, [data])

    def setStatusIndicator(self, data):
        self.send_cmd(self.cmd_setStatusIndicator,[data])

    def setFan1(self, data):
        self.send_cmd(self.cmd_setFan1, [data])

    def setFan2(self, data):
        self.send_cmd(self.cmd_setFan2, [data])

    def close(self):
        self.ser.close()

    def processCMD(self, msg):
        command = msg[1]
        checksum_read = msg[3+msg[2]]
        data = msg[3:(3+msg[2])]
        chk_array = msg[1:(len(msg)-1)]

        if (self.checksum(list(map(int,chk_array))) != checksum_read):
            data_rsp = self.rsp_badcheck
            length_rsp = 1
        else:
            if(command == 0x01):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state['opmode'] = "run"
                elif (data[0] == 0x02):
                    self.state.opmode = "service"
                elif (data[0] == 0x03):
                    self.state.opmode = "sleep"
                elif (data[0] == 0x04):
                    self.state.opmode = "alarm"
            elif(command == 0x10):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state.prox = 1
                else:
                    self.state.prox = 0
            elif( command == 0x0e):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state.helpsw = 1
                else:
                    self.state.helpsw = 0
            elif (command == 0x12):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state.upscharge = 1
                else:
                    self.state.upscharge = 0
            elif (command == 0x13):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state.upsdischarge = 1
                else:
                    self.state.upsdischarge = 0
            elif (command == 0x14):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state.upsfault = 1
                else:
                    self.state.upsfault = 0
            elif (command == 0x16):
                msgdata = 1
            elif (command == 0x17):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state['tampertop'] = 1
                else:
                    self.state['tampertop'] = 0
            elif (command == 0x19):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state.tamperbot = 1
                else:
                    self.state.tamperbot = 0
            elif (command == 0x23):
                msgdata = 1
                if(data[0] == 0x01):
                    self.state.systemreset = 1
                else:
                    self.state.systemreset = 0
            else:
                msgdata = 2
            self.send_rsp(command, [msgdata])

    def processRSP(self, lastmsg):
        while True:
            if (self.in_waiting() > 0):
                rsp = self.read(self.in_waiting())
                if (rsp[1] == lastmsg[1]):
                    break
            else:
                self.ser.write(bytearray(lastmsg))


        while(lastmsg[1] != rsp[1]):
            self.ser.write(bytearray(lastmsg))
            if (self.in_waiting() > 0):
                rsp = self.read(self.in_waiting())


    #[START][CMD][LENGTH][DATA_1…DATA_N][CHECKSUM]
    #Only pass [CMD] and all [DATA] bytes
    def send_cmd(self, cmd, data):
        message = [self.st_cmd]
        message_end = [cmd, len(data)]
        for x in data:
            message_end.append(x)
        message = message + message_end
        message.append(self.checksum(message_end))
        self.ser.write(bytearray(message))
        self.processRSP(message)

    #[START][LENGTH][DATA_1…DATA_N][CHECKSUM]
    def send_rsp(self, cmd, data):
        message = [self.st_rsp]
        message_end = [cmd, len(data)]
        for x in data:
            message_end.append(x)
        message = message + message_end
        message.append(self.checksum(message_end))
        print('sent: ' , bytes(message))
        self.ser.write(bytearray(message))

    def read(self, readbytes):
        rsp = self.ser.read(readbytes)
        return rsp

    def in_waiting(self):
        return self.ser.in_waiting

    #Check Sum = ~ (ID + LENGTH + CMD + DATA_1 + … DATA_N)
    def checksum(self, msg):
        #add all bytes and invert, taking only first 8 bits
        return int(np.invert(np.array([np.sum(msg)],dtype=np.uint8)))
