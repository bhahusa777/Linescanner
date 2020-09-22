//
//  LineScanner.cpp
//  StudyPrj
//
//  Created by Bharat  Bhusan on 16/06/20.
//  Copyright © 2020 Self. All rights reserved.
//

#include "LineScanner.hpp"

#define TEST_CODE 1
class CSVReader
{
    //std::string fileName;
    std::string delimeter;
public:
    CSVReader(std::string delm1 = ",",std::string delm2 = " ") {
        //fileName=filename;
        delimeter=delm1;
        delimeter.append(delm2);
    }
    // Function to fetch data from a CSV File
    std::vector<std::vector<std::string> > getData(const std::string&);
};

std::vector<std::vector<std::string> > CSVReader::getData(const std::string &fileName)
{
    std::ifstream file(fileName);
    std::vector<std::vector<std::string> > dataList;
    std::string line = "";
    // Iterate through each line and split the content using delimeter
    while (getline(file, line))
    {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
        dataList.push_back(vec);
    }
    // Close the File
    file.close();
    return dataList;
}

int main(){
#ifdef TEST_CODE
    std::array<long double,9> array_prod_val={0.00025177,0.008666992,0.078025818,0.24130249
        ,0.343757629,0.24130249,0.078025818,0.008666992,0.000125885
    };
    // get NUM random numbers from 0 .. 100
    std::random_device seed;

    // generator
    std::mt19937 engine(seed());

    // distribution
    std::uniform_int_distribution<int> dist(0,100);
#else
        std::cout<<"Thresold vaue"<<'\n';
        auto ThVal=0;
    std::cin>>ThVal;
        std::cout<<"File name"<<'\n';
    std::string FileName;
        std::cin>>FileName;
        CSVReader FileReader(FileName);
        //Get the data from CSV File
       std::vector<std::vector<std::string> > dataList = FileReader.getData(FileName);
#endif


    long double sum_dot_prod=0;



    
    std::queue<int> produced_nums ;
    std::mutex mGenerator ;
    std::condition_variable cond_var ;
    bool done =  false ;
    bool notified =  false ;

auto future1= std::async(std::launch::async,[ & ] ( )  {
    #ifdef TEST_CODE
       for  ( int i =  0 ; i <  100 ;  ++ i )  {
           std::this_thread::sleep_for ( std::chrono::nanoseconds(510)) ;
           std::unique_lock <std :: mutex>lock(mGenerator) ;
           produced_nums.push(dist(engine)) ;
           produced_nums.push(dist(engine)) ;
           notified =  true ;
           cond_var. notify_one ( ) ;
       }
#else
    for(int count =0;i<dataList.size();i+2)
    {
        std::this_thread::sleep_for ( std::chrono::nanoseconds(510)) ;
        std::unique_lock <std :: mutex>lock(mGenerator) ;
        produced_nums.push(dataList[i]) ;
        produced_nums.push(dataList[i+1]) ;
        notified =  true ;
        cond_var. notify_one ( ) ;
    }
#endif

       done =  true ;
       cond_var. notify_one ( ) ;
   });
auto future2 = std::async( std::launch::async,[ & ] ( )  {
    std::unique_lock <std::mutex> lock(mGenerator) ;
    while  ( ! done )  {
        int count =0;
        while  ( (! notified ) || (produced_nums.size()<=9))  {   // loop to avoid spurious wakeups
            cond_var.wait ( lock ) ;
        }
        sum_dot_prod =0;
        while  ((! produced_nums.empty()) && count<9) {
                std::cout<<  "consuming random "  << produced_nums.front()<<'\n';
                sum_dot_prod = sum_dot_prod+(produced_nums.front()*array_prod_val[count]) ;
                produced_nums.pop();
                count++;
            if(count>=9)
                break;
        }
        std::cout<<  "sum of prod "  << sum_dot_prod<<'\n';
        count =0;
        notified =  false ;
    }
} ) ;

  std::cout << std::endl;

}
