#ifndef OBJ_H 
#define OBJ_H

#include <stdio.h>
#include <string>

class CObj;

struct sLList {
	sLList *next;   
	CObj    *obj;
};
  
//===
// Comments appear before the object in
// the config files. All comments start
// with '#', and are automagically placed
// at the same indent level as the object.
struct sComments {
	sComments *next;
	char      *data;
};

class CObj {
private:
	char cwd[ 512];

	int _load( FILE *fd, int lvl);
	int _save( FILE *fd, int lvl);
	void _clear( void);
	void _init( void);

	void trim( char *buf);
	char *gettermn( char *buf, int n);
	char *gettermnz( char *buf, int n);
  
public:
	sComments  *commentFirst, *commentLast;
	
	char       name[ 33];
	
	int        dtype;         //  0 - List
	                          //  1 - int
	                          //  2 - char
	                          //  3 - data
	union {
		int      idata;
		char     *cdata;
	};

		sLList  	*objFirst,     *objLast;

	// Init
		CObj( void);
		CObj( const char *_name);
		CObj( const char *_name, const char *_cdata);
		~CObj( void);

	// Misc stack operations
		int append( CObj *obj);
		int push( CObj *obj);
		CObj *pop( void);

	// Recursive manipulators
		int save( const char *filename);
		int load( const char *filename);

	// This level data manipulators
		int commentAppend( const char *_comment);
    
		char *getName( void);
		void setName( const char *_name);
		void setData( const char *_cdata);
		void setData( int _idata);

	// Next level data manipulators
		char *getVar( const char *_name);
		int  getVarN( const char *_name);
		std::string getVarBin( const char *_name);

		void setVar( const char *_name, const char *_cdata);
		void setVarBin( const char *_name, const char *_cdata, int len);
		void appendVar( const char *_name, const char *_cdata);
		void setVar( const char *_name, int _idata);
		CObj *getObj( const char *_name);
		void delObj( CObj *obj);
		int hex2num( char ch);
};

#endif
