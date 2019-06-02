/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 *
 * @author Alexander Afanasyev <http://lasr.cs.ucla.edu/afanasyev/index.html>
 */

#include <ndn-cxx/face.hpp>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces can be used to prevent/limit name conflicts
namespace examples {

class Consumer : noncopyable
{
public:
  void
  run(int tid)
  {
    if(tid < 1){
    Interest interest(Name("/example/testApp/randomData"));
    interest.setInterestLifetime(2_s); // 2 seconds
    interest.setMustBeFresh(true);
    interest.setHashCode("787EC76DCAFD20C1908EB0936A12F91EDD105AB5CD7ECC2B1AE2032648345DFF");
    m_face.expressInterest(interest,
                           bind(&Consumer::onData, this,  _1, _2),
                           bind(&Consumer::onNack, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));

    std::cout << "Sending " << interest << std::endl;
    }
    else{
    srand(clock());
    
    Interest interest(Name("/example/"+to_string(rand())));
    interest.setInterestLifetime(2_s); // 2 seconds
    interest.setMustBeFresh(true);
    interest.setHashCode("787EC76DCAFD20C1908EB0936A12F91EDD105AB5CD7ECC2B1AE2032648345DFF");
    m_face.expressInterest(interest,
                           bind(&Consumer::onData, this,  _1, _2),
                           bind(&Consumer::onNack, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));

    std::cout << "Sending " << interest << std::endl;
    } 
    // processEvents will block until the requested data received or timeout occurs
    m_face.processEvents();

  }

private:
  void
  onData(const Interest& interest, const Data& data)
  {
    std::cout << data << std::endl;
  }

  void
  onNack(const Interest& interest, const lp::Nack& nack)
  {
    std::cout << "received Nack with reason " << nack.getReason()
              << " for interest " << interest << std::endl;
  }

  void
  onTimeout(const Interest& interest)
  {
    std::cout << "Timeout " << interest << std::endl;
  }

private:
  Face m_face;
};

} // namespace examples
} // namespace ndn
void *TestThread(void *threadid)
{  
   // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
   int tid = *((int*)threadid);
   std::cout << "Hello 线程 ID, " << tid << std::endl;
   ndn::examples::Consumer consumer;
   consumer.run(tid);
   pthread_exit(NULL);
}
int
main()
{
 timeval t_start, t_end;
gettimeofday( &t_start, NULL);
   int NUM_THREADS = 1;
   pthread_t threads[NUM_THREADS];
   int indexes[NUM_THREADS];// 用数组来保存i的值
   int rc;
   int i;
   for( i=0; i < NUM_THREADS; i++ ){      
      std::cout << "main() : 创建线程, " << i << std::endl;
      indexes[i] = i; //先保存i的值
      // 传入的时候必须强制转换为void* 类型，即无类型指针        
      rc = pthread_create(&threads[i], NULL, 
                         TestThread, (void *)&(indexes[i]));
      if (rc){
         std::cout << "Error:无法创建线程," << rc << std::endl;
         exit(-1);
      }
   pthread_join(threads[i], NULL);
   }
 gettimeofday( &t_end, NULL);
    double delta_t = (t_end.tv_sec-t_start.tv_sec) + 
                    (t_end.tv_usec-t_start.tv_usec)/1000000.0;
    std::cout << "all time : " << delta_t  << "s" << std::endl;

   pthread_exit(NULL);

  return 0;
}
