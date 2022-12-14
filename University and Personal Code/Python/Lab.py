import socket, threading, os, sys
 
SERVER_ADDRESS = '143.47.184.219'
SERVER_PORT = 5378
 
def listen(connection: socket.socket):
	while True:
		try:
			buf = ''
			msg = []
			while True:
				msg = connection.recv(4096).decode()
				buf += msg
				if '\n' in buf:
					if "HELLO" in buf:
						buf = buf.replace("HELLO", "Hey")
						buf = buf.replace('\n', '!\n')
					elif "WHO-OK" in buf:
						buf = buf.replace("WHO-OK", "Online:")
					elif "SEND-OK" in buf:
						buf = "Message sent.\n"
					elif "UNKNOWN" in buf:
						buf = "User offline.\n"
					elif "DELIVERY" in buf:
						buf = buf.replace("DELIVERY", "From:")
					elif "IN-USE" in buf:
						buf = 'Please try again.\n'
						print(buf)
						sys.stdout.flush()
						os.execl(sys.executable, sys.executable, *sys.argv)
					break
			print(buf)
 
		except Exception as e:
			print(f'Error receiving message: {e}')
			connection.close()
			break
	pass
def client():
	try:
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.connect((SERVER_ADDRESS, SERVER_PORT))
		threading.Thread(target=listen, args=[sock], daemon = True).start()
 
		name = input("Enter name: ")
		handshake = f"HELLO-FROM {name}\n"
		sock.sendall(handshake.encode("utf-8"))
 
		while True:
			message = input()
			if "!quit" in message:
				break
			elif "!who" in message:
				message = "WHO\n"
			elif message[0] == '@':
				message = message.replace("@", "SEND ") + '\n'
			sock.sendall(message.encode("utf-8"))
 
	except Exception as e:
		print(f"Error connecting to server socket {e}")
		sock.close()
if __name__ == "__main__":
    client()