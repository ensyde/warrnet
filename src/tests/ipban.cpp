#include <iostream>
#include <string>
#include <map>

using namespace std;
map<string, bool>IPList;

void BanIP(string IP) {
   IPList[IP] = true;
}

void UnbanIP(string IP) {
   IPList[IP] = false;
}

bool IsBanned(string IP) {
   map<string, bool>::iterator i = IPList.find(IP);
   if (i == IPList.end())
      return (false);
   else
      return (i->second);
}

int main(void) {
   cout << "Non-existant: " << static_cast<int>(IsBanned("127.0.0.1")) << endl;
   BanIP("127.0.0.1");   
   cout << "Banned: " << static_cast<int>(IsBanned("127.0.0.1")) << endl;
   UnbanIP("127.0.0.1");
   cout << "Unbanned: " << static_cast<int>(IsBanned("127.0.0.1")) << endl;
   BanIP("127.0.0.1");   
   cout << "Banned again: " << static_cast<int>(IsBanned("127.0.0.1")) << endl;
   return (0);
}
