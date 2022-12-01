#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>


using boost::asio::ip::tcp;
using namespace std;

boost::asio::io_context io_context;

class session
  : public std::enable_shared_from_this<session>
{
  public:
    session(tcp::socket socket) // class 之建構子，可以為空
    : socket_(std::move(socket))
    {

    }

    void start()
    {
      do_read();
    }

  private:
    void do_read()
    {
      auto self(shared_from_this());
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
          [this, self](boost::system::error_code ec, std::size_t length)
          {
            sscanf(data_, "%s %s %s %s %s", REQUEST_METHOD, REQUEST_URI, SERVER_PROTOCOL, blackhole, HTTP_HOST);

            if (!ec)
            {

              // strcpy(SERVER_ADDR, socket_.local_endpoint().address().to_string().c_str());
              // sprintf(SERVER_PORT, "%u", socket_.local_endpoint().port());
              // strcpy(REMOTE_ADDR, socket_.remote_endpoint().address().to_string().c_str());
              // sprintf(REMOTE_PORT, "%u", socket_.remote_endpoint().port());

              // setenv("REQUEST_METHOD", REQUEST_METHOD, 1);
              // setenv("REQUEST_URI", REQUEST_URI, 1);
              // char *QUERY_STRING = strchr(REQUEST_URI, '?');
              // if (QUERY_STRING == NULL)
              //   setenv("QUERY_STRING", "", 1);
              // else 
              //   setenv("QUERY_STRING", QUERY_STRING + 1, 1);
              // //setenv("QUERY_STRING", QUERY_STRING, 1);
              // setenv("SERVER_PROTOCOL", SERVER_PROTOCOL, 1);
              // setenv("HTTP_HOST", HTTP_HOST, 1);
              // setenv("SERVER_ADDR", SERVER_ADDR, 1);
              // setenv("SERVER_PORT", SERVER_PORT, 1);
              // setenv("REMOTE_ADDR", REMOTE_ADDR, 1);
              // setenv("REMOTE_PORT", REMOTE_PORT, 1);
              // strcpy(temp, REQUEST_URI);
              // strcat(EXEC_FILE, strtok(temp,"?"));
              // setenv("EXEC_FILE", EXEC_FILE, 1);

              
              // std::cout << "----------------------------------------"<< std::endl;
              // std::cout << "EXEC_FILE: " << getenv("EXEC_FILE") << std::endl;
              // std::cout << "REQUEST_METHOD: " << getenv("REQUEST_METHOD") << std::endl;
              // std::cout << "REQUEST_URI: " << getenv("REQUEST_URI") << std::endl;
              // std::cout << "QUERY_STRING: " << getenv("QUERY_STRING") << std::endl;
              // std::cout << "SERVER_PROTOCOL: " << getenv("SERVER_PROTOCOL") << std::endl;
              // std::cout << "HTTP_HOST: " << getenv("HTTP_HOST") << std::endl;
              // std::cout << "SERVER_ADDR: " << getenv("SERVER_ADDR") << std::endl;
              // std::cout << "SERVER_PORT: " << getenv("SERVER_PORT") << std::endl;
              // std::cout << "REMOTE_ADDR: " << getenv("REMOTE_ADDR") << std::endl;
              // std::cout << "REMOTE_PORT: " << getenv("REMOTE_PORT") << std::endl;
              // std::cout << "------------------------------------------"<< std::endl;

              //strcpy(data_, "HTTP/1.1 200 OK\n");
              // 測試執行路徑成功
              // strcat(EXEC_FILE, REQUEST_URI);
              // if (execlp(EXEC_FILE, EXEC_FILE, NULL, NULL) < 0)
              //     perror("Error is ");

              do_write(length); // 因為是非同步的io讀寫，因此需要能有讀有寫
            }
          });
    }

    void do_write(std::size_t length)
    {
      auto self(shared_from_this());
      boost::asio::async_write(socket_, boost::asio::buffer(status_str, strlen(status_str)),
          [this, self](boost::system::error_code ec, std::size_t /*length*/)
          {
            if (!ec)
            {
              
              io_context.notify_fork(boost::asio::io_service::fork_prepare);
              int child_pid = fork();

              if(child_pid == 0) // child process
              {
                strcpy(SERVER_ADDR, socket_.local_endpoint().address().to_string().c_str());
                sprintf(SERVER_PORT, "%u", socket_.local_endpoint().port());
                strcpy(REMOTE_ADDR, socket_.remote_endpoint().address().to_string().c_str());
                sprintf(REMOTE_PORT, "%u", socket_.remote_endpoint().port());

                setenv("REQUEST_METHOD", REQUEST_METHOD, 1);
                setenv("REQUEST_URI", REQUEST_URI, 1);
                char *QUERY_STRING = strchr(REQUEST_URI, '?');
                if (QUERY_STRING == NULL)
                  setenv("QUERY_STRING", "", 1);
                else 
                  setenv("QUERY_STRING", QUERY_STRING + 1, 1);
                //setenv("QUERY_STRING", QUERY_STRING, 1);
                setenv("SERVER_PROTOCOL", SERVER_PROTOCOL, 1);
                setenv("HTTP_HOST", HTTP_HOST, 1);
                setenv("SERVER_ADDR", SERVER_ADDR, 1);
                setenv("SERVER_PORT", SERVER_PORT, 1);
                setenv("REMOTE_ADDR", REMOTE_ADDR, 1);
                setenv("REMOTE_PORT", REMOTE_PORT, 1);
                strcpy(temp, REQUEST_URI);
                strcat(EXEC_FILE, strtok(temp,"?"));
                setenv("EXEC_FILE", EXEC_FILE, 1);

                
                std::cout << "---------------Fork---------------------"<< std::endl;
                std::cout << "EXEC_FILE: " << getenv("EXEC_FILE") << std::endl;
                std::cout << "REQUEST_METHOD: " << getenv("REQUEST_METHOD") << std::endl;
                std::cout << "REQUEST_URI: " << getenv("REQUEST_URI") << std::endl;
                std::cout << "QUERY_STRING: " << getenv("QUERY_STRING") << std::endl;
                std::cout << "SERVER_PROTOCOL: " << getenv("SERVER_PROTOCOL") << std::endl;
                std::cout << "HTTP_HOST: " << getenv("HTTP_HOST") << std::endl;
                std::cout << "SERVER_ADDR: " << getenv("SERVER_ADDR") << std::endl;
                std::cout << "SERVER_PORT: " << getenv("SERVER_PORT") << std::endl;
                std::cout << "REMOTE_ADDR: " << getenv("REMOTE_ADDR") << std::endl;
                std::cout << "REMOTE_PORT: " << getenv("REMOTE_PORT") << std::endl;
                std::cout << "------------------------------------------"<< std::endl;
                

                io_context.notify_fork(boost::asio::io_service::fork_child);
                cout<<"hi"<<endl;
                int sock = socket_.native_handle();
                dup2(sock, STDERR_FILENO);
                dup2(sock, STDIN_FILENO);
                dup2(sock, STDOUT_FILENO);
                socket_.close();

                if (execlp(EXEC_FILE, EXEC_FILE, NULL) < 0)
                    perror("Error is ");

              }
              else // parent process
              {
                io_context.notify_fork(boost::asio::io_service::fork_parent);
                cout<<"parent"<<endl;
                socket_.close();
              }

              do_read();
            }
          });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    char temp[max_length];
    char status_str[max_length] = "HTTP/1.1 200 OK\n";
    char REQUEST_METHOD[max_length];
    char REQUEST_URI[max_length];
    char QUERY_STRING[max_length];
    char SERVER_PROTOCOL[max_length];
    char HTTP_HOST[max_length];
    char SERVER_ADDR[max_length];
    char SERVER_PORT[max_length];
    char REMOTE_ADDR[max_length];
    char REMOTE_PORT[max_length];
    char EXEC_FILE[max_length] = ".";
    char blackhole[max_length];
};

class server
{
  public:
    server(boost::asio::io_context& io_context, short port)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) // server 的建構子
    {
      do_accept();
    }

  private:
    void do_accept()
    {
      acceptor_.async_accept(
          [this](boost::system::error_code ec, tcp::socket socket)
          {
            if (!ec)
            {
              std::make_shared<session>(std::move(socket))->start();
            }

            do_accept();
          });
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <port>\n";
      return 1;
    }

    //boost::asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}