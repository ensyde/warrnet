#include <iostream>
#include <string>
#include <map>

using namespace std;

typedef unsigned long u32;
map<string, int>IPList;


// this would be a u32 in the real program, easier to test with string though.
void AddConnection(string IP) {
   map<string, int>::iterator i = IPList.find(IP);
   if (i == IPList.end())
      IPList[IP] = 1;
   else
      IPList[IP]++;
}

int main(void) {
   AddConnection("127.0.0.1");
   cout << IPList["127.0.0.1"] << endl;
   AddConnection("127.0.0.1");
   cout << IPList["127.0.0.1"] << endl;

   return (0);
}
