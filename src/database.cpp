#include <ctype.h>
#include "database.h"

#include <stdio.h>

CDatabase::CDatabase(void) {

}

CDatabase::~CDatabase(void) {
	map <string, CObject *>::iterator i = ObjList.begin();
	while (i != ObjList.end()) {
		delete i->second;
		i++;
	}
}

void CDatabase::Push(const char *Key, void *Data, int Size) {
	if (strlen(Key) > 255)
		return;

	CObject *obj = new CObject(Data, Size);
	ObjList [Key] = obj;
}

void CDatabase::Push(const char *Key, int Data) {
	if (strlen(Key) > 255)
		return;

	CObject *obj = new CObject(Data);
	ObjList [Key] = obj;
}

CObject *CDatabase::GetValue(const char *Key) {
	if (ObjList.find(Key) == ObjList.end())
		return (NULL);

	return (ObjList[Key]);
}

int CDatabase::Load(const char *fName) {
	FILE *fp = fopen(fName, "r");
	if (!fp)
		return (0);

	char Key[256];
	int Size, ch;

	while (!feof(fp)) {
		fscanf(fp, "%s %d ", Key, &Size);
		if (Size == 0) {
			int Data;
			fread(&Data, sizeof(int), 1, fp);

			Push(Key, Data);

			printf("Pushed %s (%d)\n", Key, Size);
			while (isspace((ch = fgetc(fp))));
			ungetc(ch, fp);
		}
		else {
			char *Data = new char[Size + 1];

			fread(Data, Size, 1, fp);

			Push(Key, Data, Size);
			printf("Pushed %s (%d)\n", Key, Size);
			while (isspace((ch = fgetc(fp))));
			ungetc(ch, fp);
		}
	}

	fclose (fp);
	return (1);
}

int CDatabase::Save(const char *fName) {
	FILE *fp = fopen(fName, "w");
	if (!fp)
		return (0);

	map <string, CObject *>::iterator i = ObjList.begin();
	while (i != ObjList.end()) {
		CObject *obj = i->second;

		if (!obj->Size) {
			fprintf(fp, "%s %d ", i->first.c_str(), obj->Size);
			fwrite(&obj->iData, sizeof(obj->iData), 1, fp);
			fputc('\n', fp);
		}
		else {
			char *p = (char *)obj->cData;

			fprintf(fp, "%s %d ", i->first.c_str(), obj->Size);
			for (int j = 0; j < obj->Size; j++)
				fputc((int)p[j], fp);

			fputc('\n', fp);
		}

		i++;
	}

	fclose (fp);
	return (1);
}
