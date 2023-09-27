#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>
#include <thread>


int main ()
{
   using namespace boost::interprocess;

   // named_mutex::remove("mtx");
   // named_condition::remove("cnd");
   std::cout << "cvnotify starting \n";
   

   named_mutex mtx(open_or_create, "mtx");
   std::cout << "N1 " << "opened mtx \n";

   named_condition cnd(open_or_create, "cnd");
   std::cout << "N2 " << "opened cnd \n";

   std::cout << "N3 " << "mtx lock from notify \n";
   {
      scoped_lock<named_mutex> lock(mtx);

      std::cout << "N3.5 " << lock.owns() << " owns \n";

      std::cout << "N4 " << "notify sleeping \n";
      std::this_thread::sleep_for(std::chrono::seconds(2));

      std::cout << "N5 " << "notifying cnd \n";
   }
   cnd.notify_one();

   std::cout << "N6 " << "Signaled condition!" << std::endl;
   std::cout << "N7 " << "notify unlock mtx \n";

   return 0;
}
