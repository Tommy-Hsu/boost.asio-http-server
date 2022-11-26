#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

#define MAX_SESSION 5
std::string host_name[MAX_SESSION];
std::string port_number[MAX_SESSION];
std::string file_name[MAX_SESSION];

//vector <string> m;

void Parse_QUERY_STRING()
{
   string q = getenv("QUERY_STRING");
   vector<string> query;
	boost::split(query, q, boost::is_any_of("&"), boost::token_compress_on);

   for( vector<string>::iterator it = query.begin(); it != query.end(); ++ it )
   {
      if(it->at(0) == 'h')
      {
         int id = it->at(1)-'0';
         //int id = stoi(it->substr(it->find("h")+1, it->find("=")));
         cout<<id<<endl;
         host_name[id] = it->substr(it->find("=")+1);
         cout<<host_name[id]<<endl;

         // if(it->substr(it->find("=")+1).size()>1)
         // {
         //    host_num++;
         // }
      }
      if(it->at(0) == 'p')
      {
         int id = it->at(1)-'0';
         //int id = stoi(it->substr(it->find("p")+1, it->find("=")));
         cout<<id<<endl;
         port_number[id] = it->substr(it->find("=")+1);
         cout<<port_number[id]<<endl;
      }
      if(it->at(0) == 'f')
      {
         int id = it->at(1)-'0';
         //int id = stoi(it->substr(it->find("f")+1, it->find("=")));
         cout<<id<<endl;
         file_name[id] = it->substr(it->find("=")+1);
         cout<<file_name[id]<<endl;
      }
   }
}

void Show_HTML()
{
   std::string html = "Content-type: text/html\r\n\r\n";
   html.append(R"(
   
   <!DOCTYPE html>
   <html lang="en">
   <head>
      <meta charset="UTF-8" />
      <title>NP Project 3 Sample Console</title>
      <link
         rel="stylesheet"
         href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css"
         integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2"
         crossorigin="anonymous"
      />
      <link
         href="https://fonts.googleapis.com/css?family=Source+Code+Pro"
         rel="stylesheet"
      />
      <link
         rel="icon"
         type="image/png"
         href="https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png"
      />
      <style>
         * {
         font-family: 'Source Code Pro', monospace;
         font-size: 1rem !important;
         }
         body {
         background-color: #212529;
         }
         pre {
         color: #cccccc;
         }
         b {
         color: #01b468;
         }
      </style>
   </head>

   <body>
      <table class="table table-dark table-bordered">
         <thead>
         <tr>
            <th scope="col">nplinux1.cs.nctu.edu.tw:7777</th>
            <th scope="col">nplinux2.cs.nctu.edu.tw:8888</th>
         </tr>
         </thead>
         <tbody>
         <tr>
         )");

   html.append(R"(        
            <td><pre id="s0" class="mb-0"></pre></td>
            <td><pre id="s1" class="mb-0"></pre></td>
   )");

   html.append(R"(      
         </tr>
         </tbody>
      </table>
   </body>

   </html>
   )");

   std::cout<<html<<std::endl;
}

class Session : public std::enable_shared_from_this<Session> 
{
    private:
      /*    
      tcp::resolver _resolv{ioservice};
      tcp::socket _socket{ioservice};
      tcp::resolver::query _query;
      std::vector<std::string> cmds;
      std::array<char, 4096> _cmd_buffer;
      int cmd_idx;
      std::string session_id;
      int s_id;
      std::string host_ip;
      std::string host_port;
      std::string cmd_file;
      enum { max_length = 4096 };
      std::array<char, 4096> bytes;
      */
      tcp::socket socket_;
      //tcp::resolver resolv_;
      //tcp::resolver::query _query;
      enum { max_length = 1024 };
      char data_[max_length];
      std::string host_ip;
      std::string host_port;
      std::string file;
      std::string session_id;
      std::vector<std::string> cmds;
        
    public:
        // constructor && some info to init
        Session(boost::asio::io_context& io_context, std::string host_ip, std::string host_port, std::string file, std::string session_id)
        : socket_(io_context)
        {
            read_cmd_from_file();
        }
        void start() { do_resolve(); }

    private:

        bool read_cmd_from_file()
        {
            return true;
        }

        void do_send_cmd()
        {
            // socket_.async_send(
            //    buffer(cmds[s_id][cmd_idx]+"\r\n"),
            //    [this](boost::system::error_code ec, size_t /* length */) 
            //    {
            //       if (!ec)
            //       {
                     
            //          do_read();
            //       } 
            //    });
        }

        void do_read() 
        {
            // socket_.async_read_some(
            //     buffer(data_, max_length),
            //     [this](boost::system::error_code ec, size_t length) {
            //     if (!ec)
            //     {
                    
            //     }
            //     else
            //     {
                    
            //     }
            // });
        }

        void do_connect()
        {
            // // std::cout << "cmds.size() = " << cmds.size() << std::endl;
            // socket_.async_connect(*it, [this](boost::system::error_code ec)
            // {
            //     if (!ec)
            //     {
            //         // read_cmd_from_file();
            //         // std::cout << "cmds.size() = " << cmds.size() << std::endl;
            //         do_read();
            //     }
            //     else
            //     {
                    
            //     }
            // });
        }

         void do_resolve() 
         {
            // //  std::cout << "cmds.size() = " << cmds.size() << std::endl;
            // resolv_.async_resolve(_query, [this](boost::system::error_code ec, tcp::resolver::iterator it)
            // {
            //     if(!ec)
            //     {
                    
            //     }
            //     else
            //     {

            //     }
            // });
        }
};


int main (int argc, char* argv[]) {

   try 
   {
      Parse_QUERY_STRING();
      //Show_HTML();
   } 
   catch (std::exception &e) 
   {
      cout << "exception: " << e.what() << "\n";
   }
   
   return 0;
}