n = recv(tcpSocket, Buffer + BufferLen, sizeof(Buffer) - BufferLen, 0);
BufferLen += n;
if (CClient.ConnectionType == EConnectionType[C_UNKNOWN] && Buffer[0] != 0xFF) {
	switch(Buffer[0]) {
		case 1: 
		case 2: 
		case 3: ConnectionType = Buffer[0]; break;
		case 'c': ConnectionType = 3; break;
		default: disc();
	}
	memmove(Buffer, Buffer + 1, Buffer - 1);
}
while (BufferLen) {
	u16 PacketLen = *(u16 *)(Buffer + 2);
	if (PacketLen > BufferLen)
		break;

	DispatchTCP(Buffer, PacketLen);
	memmove(Buffer, Buffer + PacketLen, BufferLen - PacketLen);
	BufferLen -= PacketLen;
}
