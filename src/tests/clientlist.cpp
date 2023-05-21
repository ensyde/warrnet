#include <list>
#include <iostream>

using namespace std;

struct CClient {
   CClient(char *Name, unsigned long IP);
   char name[32];
   unsigned long ip;
};

CClient::CClient(char *Name, unsigned long IP) {
   strcpy(name, Name);
   ip = IP;
}

int main(void) {
   list<CClient *> ClientList;
   CClient *a = new CClient("Mike", 6969),
           *b = new CClient("Jon", 1337),
           *c = new CClient("Aaron", 1234);

   // insert clients
   ClientList.push_back(a);
   ClientList.push_back(b);
   ClientList.push_back(c);

   // transverse through list
   list<CClient *>::iterator i = ClientList.begin();
   while (i != ClientList.end()) {
      cout << (*i)->name << endl;
      i++;
   }

   // clear list & delete it's pointers
   while (!ClientList.empty()) {
      CClient *t = ClientList.front();
      ClientList.pop_front();
      delete t;
   }

   return (0);
}
