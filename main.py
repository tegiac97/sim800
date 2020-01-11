import serial
# import os,time,sys
from time import sleep

ser = serial.Serial(
	port='/dev/ttyUSB0',
	baudrate=115200,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS
)



# print(ser.isOpen())
# ser.write('ate0\r\n'.encode())

# msg='at\r\n'
# ser.write('ate0\r\n'.encode())
# print(ser.write(msg.encode()))
# sleep(1)
# out = ''
# while ser.inWaiting() > 0:
# 	out += ser.read(1).decode("utf-8")
# 	# print (">>" + out)
# if out != '':
# 	# out=out.replace(msg,'')
# 	print (out)
# 	print(sys.getsizeof(out)-sys.getsizeof(msg))
# 	print(type(out))


def print_respone():
	sleep(1)
	out = ''
	while ser.inWaiting() > 0:
		out += ser.read(1).decode("utf-8")
		# print (">>" + out)
	if out != '':
		# out=out.replace(msg,'')
		print (out)
		# print(sys.getsizeof(out)-sys.getsizeof(msg))
		# print(type(out))
		return out

#check
#if ok return OK, if not return ERROR
def check():
	ser.write('AT+CPIN?\r\n'.encode())
	sleep(1)
	out = ''
	while ser.inWaiting() > 0:
		out += ser.read(1).decode("utf-8")
		# print (">>" + out)
	if out != '':
		return str(out)



#====================================SMS===============================
#send sms
# string: phoneno: phone number destination
# string: msg to send
def send_sms(phoneno, msg):
	ser.write('AT+CMGF=1\r\n'.encode()) #Configuring TEXT mode
	print_respone()
	ser.write(("AT+CMGS=\"" + phoneno + "\"\n").encode())	#chr(26)= Ctrl+Z
	sleep(0.5)
	ser.write((msg+chr(26)).encode())
	print_respone()

def recv_sms():
	ser.write('AT+CMGR=1\r\n'.encode())
	print_respone()
	

#======================================PIN==================================
#list pin code
def pin_code(code):
	switcher={
		'READY': 'ready', # MT is not pending for any password
		'SIM PIN': 'waiting pin', # MT is waiting SIM PIN to be given
		'SIM PUK': "waiting puk", # MT is waiting for SIM PUK to be given
		'PH_SIM PIN': 'waiting phone sim', # ME is waiting for phone to SIM card (antitheft)
		'PH_SIM PUK': 'waiting sim puk' # ME is waiting for SIM PUK (antitheft)
	}
	return switcher.get(code,"Invalid code")

def checkpin():
	ser.write('AT+CPIN?\r\n'.encode())
	code=print_respone()
	# print(code[9:-8])
	# print(pin_code(code[9:-8]));
	return pin_code(code[9:-8])

	

#===================================CALL=================================
def voice_call(phoneno):
	ser.write(("atd"+phoneno).encode())
	print_respone()
	



#=========================================+MAIN==============================
if __name__ == '__main__':
	# ser.write('AT+CPIN=1279\r\n'.encode())
	# print(send_sms("0886885207","hello"))
	# print(checkpin())
	# print(check())
	# code = '+CPIN: SIM PIN\r\n'
	# print(type(code[7:-2]))
	# print(voice_call("0886885207"))
	# print(chr(65))
	recv_sms()
	ser.close()