#include "ftpcentre.h"
#include "poppkt.h"

CFTPCentre::CFTPCentre(void) {
   Log = new CLog("logs/ftpcentre.log", "w");
   Buffer[0] = '\0';
   BufferPos = 4;
}

CFTPCentre::~CFTPCentre(void) {
	delete Log;
}

int CFTPCentre::Dispatch(u8 *Message, u32 Size, CClient *Client) {
	int ID = *(u16 *)(Message + 2);
	switch (ID) {
		case FID_FILEREQUEST:	OnFileRequest(Message, Size, Client); break;
		default: 					OnUnknown(Message, Size, Client); break;
	}

	return (0);
}

void CFTPCentre::OnFileRequest(u8 *Message, u32 Size, CClient *Client) {
	CPopPkt Pack(Size, (char*)Message);
	u32 ArchTag = Pack.pop_u32();
	u32 ClientTag = Pack.pop_u32();
	u32 AdID = Pack.pop_u32();
	u32 Extension = Pack.pop_u32();
	u32 Offset = Pack.pop_u32();
	u32 StampHigh = Pack.pop_u32();
	u32 StampLow = Pack.pop_u32();
	string FileName = Pack.pop_string();

	SendFile((char *)FileName.c_str(), Offset, AdID, Extension, StampHigh, StampLow, Client);	
}

void CFTPCentre::OnUnknown(u8 *Message, u32 Size, CClient *Client) {
   Log->Write("Unknown packet in function %s :\n",  __PRETTY_FUNCTION__);
   //Log->HexDump( Size, (char *)Message);
}

void CFTPCentre::SendFile(char *Filename, u32 Offset, u32 AdID, u32 Extension, u32 StampHigh, u32 StampLow, CClient *Client) {
	const char *FILE_BIN_PATH = "Files/";	// put in config file?
	string Filepath = FILE_BIN_PATH;
	Filepath += Filename;

	struct stat buf;
	if (stat(Filepath.c_str(), &buf) != 0) {
		Log->Write("Error: Stat on file %s returned -1 %s\n", Filepath.c_str(), strerror(errno));

		// File doesn't exist, send file length 0 so client won't hang.
		*this << (u32)0
				<< (u32)AdID
				<< (u32)Extension
				<< (u32)StampHigh
				<< (u32)StampLow
				<< Filename;

		SendPacket(0x0000, Client);
		return;
	}

	char *Buffer = new char[buf.st_size + 1];
	if (ReadFile(Filepath.c_str(), Buffer, buf.st_size, Offset) != -1) 
		Log->Write("Info: Read file %s (%d bytes) successfully.\n", Filepath.c_str(), buf.st_size);
	else
		Log->Write("Info: File %s failed to read\n", Filepath.c_str());

	*this << (u32)buf.st_size
			<< (u32)AdID
			<< (u32)Extension
			<< (u32)StampHigh
			<< (u32)StampLow
			<< Filename;

	SendPacket(0x0000, Client);

	if (Offset > buf.st_size)
		Offset = 0;

	if (Send(Client->GetFD(), Buffer, buf.st_size) != -1)
		Log->Write("Info: Sent file %s (%d bytes) successfully.\n", Filepath.c_str(), buf.st_size);
	else
		Log->Write("Info: File %s failed to send\n", Filepath.c_str());

	delete [] Buffer;
//	Client->Disconnect();
}

CFTPCentre &CFTPCentre::operator<<(int Data) {
   *(int *)(Buffer + BufferPos) = Data;
   BufferPos += sizeof (int);
   return (*this);
}

CFTPCentre &CFTPCentre::operator<<(u8 Data) {
   *(u8 *)(Buffer + BufferPos) = Data;
   BufferPos += sizeof(u8);
   return (*this);
}

CFTPCentre &CFTPCentre::operator<<(u16 Data) {
   *(u16 *)(Buffer + BufferPos) = Data;
   BufferPos += sizeof(u16);
   return (*this);
}

CFTPCentre &CFTPCentre::operator<<(u32 Data) {
   *(u32 *)(Buffer + BufferPos) = Data;
   BufferPos += sizeof(u32);
   return (*this);
}

CFTPCentre &CFTPCentre::operator<<(u8 *Data) {
   operator<<((char *)Data);
   return (*this);
}

CFTPCentre &CFTPCentre::operator<<(char *Data) {
   if (strlen(Data) > (MAX_STAGEBUF - BufferPos))
      return (*this);

   strcpy((char *)Buffer + BufferPos, Data);
   BufferPos += strlen(Data) + 1;
   return (*this);
}

void CFTPCentre::SendPacket(u16 ID, CClient *Client) {
	*(u16 *)(Buffer + 0) = BufferPos;
	*(u16 *)(Buffer + 2) = ID;

	int n = send(Client->GetFD(), Buffer, BufferPos, 0);
	if (n < 0) {
		Log->Write("Error: SendPacket returned %d (%s)\n", n, strerror(errno));
		return;
	}

	BufferPos = 4;
	Buffer[0] = '\0';
}

int CFTPCentre::Send(int FD, const char *Buffer, int Size) {
	int ret = 0;
	int total = 0;
	int bytesleft = Size;
	while (total < Size) {
		int n = send(FD, Buffer + total, bytesleft, 0);
		if (n == -1) {
			printf("Send failed at %d bytes\n", total);
			ret = -1;
			break;
		}

		total += n;
		bytesleft -= n;
	}

	return (ret);
}

int CFTPCentre::ReadFile(const char *Filename, char *Buffer, int Size, int Offset) {
	if (Offset > Size)
		return (-1);

	FILE *fp = fopen(Filename, "rb");
	if (!fp)
		return (-1);

	fseek(fp, Offset, SEEK_SET);

	int ret = 0;

	int total = 0;
	int bytesleft = Size;

	while (total < Size) {
		int n = fread(Buffer + total, 1, bytesleft, fp);
		if (n == -1) {
			Log->Write("Error: File %s failed on fread (%s)\n", Filename, strerror(errno));
			ret = -1;
			break;
		}

		total += n;
		bytesleft -= n;
	}

	fclose (fp);
	return (ret);
}
