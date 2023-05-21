//===
// Todo:
//   Make thread safe/mutex locking.

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "obj.h"

CObj::CObj( void) {
  _init();
}

CObj::CObj( const char *_name) {
  _init();
  strncpy( name, _name, sizeof( name) - 1);
  dtype = 0;
}

CObj::CObj( const char *_name, const char *_cdata) {
  _init();
  strncpy( name, _name, sizeof( name) - 1);
  cdata = new char[strlen( _cdata) + 1];
  strcpy( cdata, _cdata);
  dtype = 2;
}

CObj::~CObj( void) {
  _clear();
}

void CObj::_clear( void) {
  while (objFirst) {
    sLList *tmp = objFirst->next;
    delete objFirst->obj;
    delete objFirst;
    objFirst = tmp;
  }
  // objLast = NULL;

  while (commentFirst) {
    sComments *tmp = commentFirst->next;
    delete commentFirst->data;
    delete commentFirst;
    commentFirst = tmp;
  }
  // commentLast = NULL;
}

void CObj::_init( void) {
	memset( name, 0, sizeof( name));
	objFirst = objLast = NULL;
	commentFirst = commentLast = NULL;
	dtype = 0;
	cdata = NULL;
}

int CObj::_load( FILE *fd, int lvl) {
  CObj *obj = new CObj(); append( obj);
  
  for (;;) {
    char buf[ 4096];
    if (!fgets( buf, sizeof( buf), fd)) return -1;

    trim( buf);

    if (buf[0] == '#')
      obj->commentAppend( buf);
    else {
      char *t2 = gettermnz( buf, 2);
      char *t1 = gettermn( buf, 1); 

	if (t1) {
	  if (*t1 == '}') {
	    break;
	  } else {
	    obj->setName( t1);
	    if (t2) {
	      if (*t2 == '{')
	        obj->_load( fd, lvl + 2);
	      else {
				//===
				// We need to parse things like \\ and \"
	      	if (*t2 == '"') {
	      		t2 = t2 + 1;
	      		if (strlen( t2))
	      			if (*(t2 + strlen( t2) - 1) == '"')
	      				*(t2 + strlen( t2) - 1) = 0;
					//===
					// Ok, parse time.
						char *start = t2;
						char *loc = t2;
						while (*loc) {
							switch (*loc) {
								case '\\':
									loc++;
								default:
									*start++ = *loc++;
							}
						}
						*start = 0;
	      	}
	        obj->setData( t2);
	      }
	      obj = new CObj(); append( obj);
          }
        }  
      }    
    }      
  }        

  return 0;
}

int CObj::_save( FILE *fd, int lvl) {
  sComments *tmp = commentFirst;
  while (tmp) {
    for (int i = 0; i < lvl; i++) fputc( ' ', fd);
    fprintf( fd, "%s\n", tmp->data);
    tmp = tmp->next;
  }
   
	switch( dtype) {
	case 0:
      if (name[0]) {
        for (int i = 0; i < lvl; i++) fputc( ' ', fd);
        fprintf( fd, "%s {\n", name);
      }
      {
        sLList *tmp = objFirst;
        while (tmp) {
          if (name[0])
            tmp->obj->_save( fd, lvl + 2);
          else
            tmp->obj->_save( fd, lvl);
          tmp = tmp->next;
        }
      }  
      if (name[0]) {
        for (int i = 0; i < lvl; i++) fputc( ' ', fd);
        fprintf( fd, "};\n");
      }
	  break;
	case 2: 
		for (int i = 0; i < lvl; i++) fputc( ' ', fd);

		fprintf( fd, "%s \"", name);
		unsigned char *tmp = (unsigned char *)cdata;
		while (*tmp) {
			// Ignore anything below 32
			if (*tmp >= 32) {
				char ch = *tmp;
				switch(*tmp) {
					case '"':
					case '\\':
						fprintf( fd, "\\");
				}
				fprintf( fd, "%c", ch);
			}
			tmp++;
		}
		fprintf( fd, "\"\n");
		break;
	};

	return 0;
}
 
int CObj::append( CObj *obj) {
  try {
    sLList *tmp = new sLList;
    tmp->next = NULL;
    tmp->obj = obj;
    if (objLast) {
      objLast->next = tmp;
      objLast = tmp; 
    } else {
      objFirst = objLast = tmp;
    }
  }
  catch (...) {
    return -1;
  }
  return 0;
}

int CObj::save( const char *filename) {
  FILE *fd = fopen( filename, "w");
  if (!fd) return -1;

  _save( fd, 0);

  fclose( fd);
  return 0;
}
 
int CObj::load( const char *filename) {
  FILE *fd = fopen( filename, "r");
  if (!fd) return -1;

  _load( fd, 0);

  fclose( fd);
  return 0;
}

int CObj::commentAppend( const char *_comment) {
  try {
    sComments *tmp = new sComments;
    tmp->next = NULL;
    tmp->data = new char[ strlen( _comment) + 1];
    strcpy( tmp->data, _comment);
    if (commentLast) {
      commentLast->next = tmp;
      commentLast = tmp;
    } else {
      commentFirst = commentLast = tmp;
    }
  } catch (...) {
    return -1;   
  }
  return 0;
}
 
void CObj::trim( char *buf) {
  char *p;
  
  p = buf;

  while ( isspace( *p))
    p++;

  strcpy( buf, p);

  int n;

  if (n = strlen( buf)) {
    p = buf + n - 1;
  
    while ( isspace( *p))
      *p-- = '\0';
  }
}  
   
char *CObj::gettermn( char *buf, int n) {
  int a = 0; char *p;

  // Find the start.
  while (n) {
    if (*buf == '\0') return( NULL);

    if (isspace( *buf++))
      a = 0;
    else {  
      if (!a)
        n--; 
      a++;   
    }
  }  
     
  // Find the end.
  p = --buf;
  while ( !isspace( *p))
    if (!*p)
      break;
    else
      p++;

  *p = '\0';

  return( buf);
}
 
char *CObj::gettermnz( char *buf, int n) {
  int a = 0; char *p;

  // Find the start.
  while (n) {
    if (*buf == '\0') return( NULL);

    if (isspace( *buf++))
      a = 0;
    else {  
      if (!a)
        n--; 
      a++;   
    }
  }  
     
  return( --buf);
}
 
char *CObj::getName( void) {
  if (name[0])
    return name;
  else
    return NULL;
}

void CObj::setName( const char *_name) {
  strncpy( name, _name, sizeof( name) - 1);
}

void CObj::setData( int _idata) {
  char buf[ 20];
  sprintf( buf, "%d", _idata);
  setData( buf);
}

void CObj::setData( const char *_cdata) {
	if (dtype == 2)
		if (cdata)
			delete cdata;

	if (!_cdata)
		_cdata = "";
		
	cdata = new char[ strlen( _cdata) + 1];
	strcpy( cdata, _cdata);
	dtype = 2;
}

void CObj::setVar( const char *_name, int _idata) {
  sLList *tmp = objFirst;
  
  while (tmp) {
    if (tmp->obj)
      if (!strcasecmp( tmp->obj->name, _name)) {
        tmp->obj->setData( _idata);
        return;
      }
    tmp = tmp->next;
  }
  
  tmp = new sLList;
  tmp->next = objFirst;
  tmp->obj = new CObj();
  tmp->obj->setName( _name);
  tmp->obj->setData( _idata);

  objFirst = tmp;
  if (!objLast)
    objLast = tmp;
}

void CObj::setVar( const char *_name, const char *_cdata) {
  sLList *tmp = objFirst;
  
  while (tmp) {
    if (tmp->obj)
      if (!strcasecmp( tmp->obj->name, _name)) {
        tmp->obj->setData( _cdata);
        return;
      }
    tmp = tmp->next;
  }

	appendVar( _name, _cdata);
}

void CObj::appendVar( const char *_name, const char *_cdata) {
	sLList *tmp = new sLList;
	tmp->next = NULL;
	tmp->obj = new CObj();
	tmp->obj->setName( _name);
	tmp->obj->setData( _cdata);
	
	if (objLast)
		objLast->next = tmp;
	else
		objFirst = tmp;
	objLast = tmp;
}

char *CObj::getVar( const char *_name) {
  sLList *tmp = objFirst;
  while (tmp) {
    if (tmp->obj)
      if (!strcasecmp( tmp->obj->name, _name))
        if (tmp->obj->dtype == 2)
          return tmp->obj->cdata;
        else
          return NULL;
    tmp = tmp->next;
  }
  return NULL;
}

int CObj::getVarN( const char *_name) {
  sLList *tmp = objFirst;
  while (tmp) {
    if (tmp->obj)
      if (!strcasecmp( tmp->obj->name, _name))
        if (tmp->obj->dtype == 2)
          return atoi( tmp->obj->cdata);
        else
          return 0;
    tmp = tmp->next;
  }
  return 0;
}

//===
// Umm codeme...
void CObj::delObj( CObj *obj) {
	if (!obj)
		return;

	sLList **tmp = &objFirst;
	while (*tmp && ((*tmp)->obj != obj))
		tmp = &((*tmp)->next);
	
	if (!*tmp)
		return;

	sLList *tmp2 = *tmp;
	*tmp = tmp2->next;

	//===
	// Todo: Fixme ! :)
	{
		sLList *tmp = objFirst;
		if (!tmp)
			objLast = NULL;
		else {
			while (tmp->next)
				tmp = tmp->next;
			objLast = tmp;
		}
	}
	
	delete tmp2->obj;
	delete tmp2;
}

CObj *CObj::getObj( const char *_name) {
	if (!_name)
		return NULL;

	sLList *tmp = objFirst;
	while (tmp) {
		if (tmp->obj && tmp->obj->name)
			if (!strcasecmp( tmp->obj->name, _name)) {
				return tmp->obj;
			}
			tmp = tmp->next;
	}
	return NULL;
}

void CObj::setVarBin( const char *_name, const char *_cdata, int len) {
	static const char *hex = "0123456789ABCDEF";
	std::string res;

	res.reserve( 2 * len);
	for (int i = 0; i < len; i++) {
		res += hex[(_cdata[i] >> 4) & 0x0f];
		res += hex[_cdata[i] & 0x0f];
	}
	
	setVar( _name, res.c_str());
}

std::string CObj::getVarBin( const char *_name) {
	char *tmp = getVar( _name);
	std::string ret;

	if (!tmp)
		return ret;

	if (strlen(tmp) & 1)
		return ret;
		
	while (*tmp) {
		ret += (char) ( (hex2num(*tmp) << 4) + hex2num(*(tmp + 1)) );
		tmp += 2;
	}
	return ret;
}

int CObj::hex2num( char ch) {
	int ret = 0;
	if ((ch >= '0') && (ch <= '9'))
		ret = ch - '0';
	if ((ch >= 'A') && (ch <= 'F'))
		ret = ch - 'A' + 10;
	if ((ch >= 'a') && (ch <= 'f'))
		ret = ch - 'a' + 10;
	return ret;
}
