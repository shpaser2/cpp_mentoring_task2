#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>
#include <thread>


int main ()
{
   using namespace boost::interprocess;

   std::this_thread::sleep_for(std::chrono::seconds(1));

   named_mutex::remove("mtx");
   named_condition::remove("cnd");
   std::cout << "w1 " << "removed cnd and mtx \n";

   named_mutex mtx(open_or_create, "mtx");
   std::cout << "w2 " << "created mtx \n";

   named_condition cnd(open_or_create, "cnd");
   std::cout << "w3 " << "created cnd \n";

   std::cout << "w4 " << "lock mutex in wait\n";
   scoped_lock<named_mutex> lock(mtx);

   std::cout << "w5 " << lock.owns() << " owns \n";

   std::cout << "w6 " << "wfor notification \n";

   cnd.wait(lock);

   std::cout << "w7 " << "Condition met!" << std::endl;
   named_mutex::remove("mtx");
   named_condition::remove("cnd");

   std::cout << "w8 " << "Cnd and mtx deleted! \n";

   return 0;
}
