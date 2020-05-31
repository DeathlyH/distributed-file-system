#include <iostream>
#include "rpc/client.h"
#include "rpc/rpc_error.h"

//#include "harp_client.hpp"

//int main() {
//  RpcClient rpc_client("127.0.0.1", 8000);
//  Client user_client(&rpc_client);
//  user_client.GetCommunicationService()->WriteFile("/Users/Jiaming/Desktop/test3/file.txt", "Hello2");
//  return 0;
//}


int main(){
	rpc::client c("127.0.0.1", 8080);
    std::string file_name = "/Users/Jiaming/Desktop/test/file.txt";
	std::string file_content = "hello world 21:49";
	  //front_end.WriteFile(file_name, file_content);
    try{
    	c.call("RegisterBackup", "127.0.0.1", 8070);
	    c.call("WriteFile", file_name, file_content);

    }
    catch (rpc::rpc_error &e) {
    	std::cout<< std::endl<<e.what() <<std::endl;
    	std::cout<< "in function "<< e.get_function_name() << ": ";

    	using err_t = std::tuple<int, std::string>;
    	auto err = e.get_error().as<err_t>();
    	std::cout << "[error " << std::get<0>(err) <<"]: "<<std::get<1>(err)<<std::endl;
    	return 1;
    }
	return 0;
}
