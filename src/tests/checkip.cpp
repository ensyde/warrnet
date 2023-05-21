#include <map>
#include <iostream>

typedef unsigned long u32;

using namespace std;

map<u32, bool> IPBanList;
map<u32, u32> IPCountList;

bool IsIP_Okay(u32 IP) {
   map<u32,bool>::iterator i = IPBanList.find(IP);
   map<u32,u32>::iterator j = IPCountList.find(IP);

   // if ip isnt found on either list, it's OK
   if (i == IPBanList.end() && j == IPCountList.end())
      return (true);

   // if ip is banned, it's not ok
   else if (i != IPBanList.end() && i->second == true)
      return (false);

   // if ip is connected 8 times, it's not ok
   else if (j != IPCountList.end() && j->second > 8)
      return (false);

   // everything is OK
   else
      return (true);
}

int main(void) {
   u32 ip1 = 696969,
       ip2 = 9291012,
       ip3 = 91019532,
       ip4 = 919222,
       ip5 = 10233;

   IPBanList[ip1] = true;
   IPBanList[ip2] = false;
   IPBanList[ip3] = false;
   IPBanList[ip4] = false;
   IPBanList[ip5] = true;

   IPCountList[ip1] = 1;
   IPCountList[ip2] = 2;
   IPCountList[ip3] = 10;
   IPCountList[ip4] = 6;
   IPCountList[ip5] = 8;

   int ips[] = {ip1,ip2,ip3,ip4,ip5};
   for (int i = 0; i < 5; i++) {
      char banned[32];
      if (IPBanList[ips[i]] == true)
         strcpy(banned, "Banned");
      else
         strcpy(banned, "Not Banned");

      char isok[32];
      if (IsIP_Okay(ips[i]) == true)
         strcpy(isok, "Is Okay");
      else
         strcpy(isok, "Is not Okay");

      cout << "IP#" << i << " [" << banned << "] [# Of Connections: " << IPCountList[ips[i]] << "]" 
           << " *** (" << isok << ") ***" << endl;
   }
  
   return (0);
}
