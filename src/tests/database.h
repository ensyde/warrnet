#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>

using namespace std;

// if size = 0, assume int
struct CObject {
public:
	CObject(void *data, int size) : cData(data), Size(size) {}

	CObject(int data) : iData(data), Size(0) {}

	~CObject(void) {}

	union {
		int iData;
		void *cData;
	};

	int Size;
};

class CDatabase {
	map <string, CObject *> ObjList;
public:
	CDatabase(void);
	~CDatabase(void);
	
	void Push(const char *Key, void *Data, int Size);
	void Push(const char *Key, int Data);
	
	CObject *GetValue(const char *Key);

	// 1 = success, 0 = failure
	int Load(const char *fName);
	int Save(const char *fName);
};

#endif	
