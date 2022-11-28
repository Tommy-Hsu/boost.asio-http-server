#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>


using boost::asio::ip::tcp;
using namespace std;

class Session
  : public std::enable_shared_from_this<Session>
{
    public:

        Session(tcp::socket socket)
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
                        Parse_Request();
                        Print_Request();

                        if (strcmp(REQUEST_CGI, "/panel.cgi") == 0)
                            Show_Panel();
                        else
                            Show_Console();
                    }
                });
        }

        void do_write(string str)
        {
            auto self(shared_from_this());
            boost::asio::async_write(socket_, boost::asio::buffer(str.c_str(), str.length()),
                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                {
                    if (!ec)
                    {
                        //do_read();
                    }
                });
        }

        void Parse_Request()
        {
            strcpy(SERVER_ADDR, socket_.local_endpoint().address().to_string().c_str());
            sprintf(SERVER_PORT, "%u", socket_.local_endpoint().port());
            strcpy(REMOTE_ADDR, socket_.remote_endpoint().address().to_string().c_str());
            sprintf(REMOTE_PORT, "%u", socket_.remote_endpoint().port());

            if (strstr(REQUEST_URI, "?") != NULL)
            {
                vector<string> v;
                string t(REQUEST_URI);
                boost::split(v, t, boost::is_any_of("?"), boost::token_compress_on);
                strcpy(REQUEST_CGI, v[0].c_str());
                strcpy(QUERY_STRING, v[1].c_str());
            }
            else
                strcpy(REQUEST_CGI,REQUEST_URI);            
        }

        void Print_Request()
        {
            cout << "----------Environment--------" << endl;
            cout << "Method method: " << REQUEST_METHOD << endl;
            cout << "Request url: " << REQUEST_URI << endl;
            cout << "Request CGI: " << REQUEST_CGI <<endl;
            cout << "Query string: " << QUERY_STRING << endl;
            cout << "Server protocol: " << SERVER_PROTOCOL << endl;
            cout << "Http host: " << HTTP_HOST << endl;
            cout << "Server addr: " << SERVER_ADDR << endl;
            cout << "Server port: " << SERVER_PORT << endl;
            cout << "Remote addr: " << REMOTE_ADDR << endl;
            cout << "Remote port: " << REMOTE_PORT << endl;
            cout << "-----------------------------" << endl;
        }

        void Show_Panel()
        {   
            cout<<"Show_Panel"<<endl;
            string html = 
            "HTTP/1.1 200 OK\r\n"
            "Content-type: text/html\r\n\r\n"
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "  <head>\n"
            "    <title>NP Project 3 Panel</title>\n"
            "    <link\n"
            "      rel=\"stylesheet\"\n"
            "       href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\"\n"
            "       integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\"\n"
            "       crossorigin=\"anonymous\"\n"
            "    />\n"
            "    <link\n"
            "href=\"https://fonts.googleapis.com/css?family=Source+Code+Pro\"\n"
            "      rel=\"stylesheet\"\n"
            "    />\n"
            "    <link\n"
            "      rel=\"icon\"\n"
            "      type=\"image/png\"\n"
            "href=\"https://cdn4.iconfinder.com/data/icons/iconsimple-setting-time/512/dashboard-512.png\"\n"
            "    />\n"
            "    <style>\n"
            "      * {\n"
            "        font-family: 'Source Code Pro', monospace;\n"
            "      }\n"
            "    </style>\n"
            "  </head>\n"
            "  <body class=\"bg-secondary pt-5\">"
            "<form action=\"console.cgi\" method=\"GET\">\n"
            "      <table class=\"table mx-auto bg-light\" style=\"width: "
            "inherit\">\n"
            "        <thead class=\"thead-dark\">\n"
            "          <tr>\n"
            "            <th scope=\"col\">#</th>\n"
            "            <th scope=\"col\">Host</th>\n"
            "            <th scope=\"col\">Port</th>\n"
            "            <th scope=\"col\">Input File</th>\n"
            "          </tr>\n"
            "        </thead>\n"
            "        <tbody>";

            boost::format table(
            "          <tr>\n"
            "            <th scope=\"row\" class=\"align-middle\">Session %1%</th>\n"
            "            <td>\n"
            "              <div class=\"input-group\">\n"
            "                <select name=\"h%2%\" class=\"custom-select\">\n"
            "                   <option></option>"
            "                   <option value=\"nplinux1.cs.nctu.edu.tw\">nplinux1</option>"
                                "<option value=\"nplinux2.cs.nctu.edu.tw\">nplinux2</option>"
                                "<option value=\"nplinux3.cs.nctu.edu.tw\">nplinux3</option>"
                                "<option value=\"nplinux4.cs.nctu.edu.tw\">nplinux4</option>"
                                "<option value=\"nplinux5.cs.nctu.edu.tw\">nplinux5</option>"
                                "<option value=\"nplinux6.cs.nctu.edu.tw\">nplinux6</option>"
                                "<option value=\"nplinux7.cs.nctu.edu.tw\">nplinux7</option>"
                                "<option value=\"nplinux8.cs.nctu.edu.tw\">nplinux8</option>"
                                "<option value=\"nplinux9.cs.nctu.edu.tw\">nplinux9</option>"
                                "<option value=\"nplinux10.cs.nctu.edu.tw\">nplinux10</option>"
                                "<option value=\"nplinux11.cs.nctu.edu.tw\">nplinux11</option>"
                                "<option value=\"nplinux12.cs.nctu.edu.tw\">nplinux12</option>"
            "                </select>\n"
            "                <div class=\"input-group-append\">\n"
            "                  <span class=\"input-group-text\">.cs.nctu.edu.tw</span>\n"
            "                </div>\n"
            "              </div>\n"
            "            </td>\n"
            "            <td>\n"
            "              <input name=\"p%2%\" type=\"text\" class=\"form-control\" size=\"5\" />\n"
            "            </td>\n"
            "            <td>\n"
            "              <select name=\"f%2%\" class=\"custom-select\">\n"
            "                   <option></option>"
                                "<option value=\"t1.txt\">t1.txt</option>"
                                "<option value=\"t2.txt\">t2.txt</option>"
                                "<option value=\"t3.txt\">t3.txt</option>"
                                "<option value=\"t4.txt\">t4.txt</option>"
                                "<option value=\"t5.txt\">t5.txt</option>"
            "              </select>\n"
            "            </td>\n"
            "          </tr>");
            for (int i = 0; i < 5; ++i)
                html += (table % (i + 1) % i).str();
            html +=
            "          <tr>\n"
            "            <td colspan=\"3\"></td>\n"
            "            <td>\n"
            "              <button type=\"submit\" class=\"btn btn-info btn-block\">Run</button>\n"
            "            </td>\n"
            "          </tr>\n"
            "        </tbody>\n"
            "      </table>\n"
            "    </form>\n"
            "  </body>\n"
            "</html>";

            do_write(html);
        }

        void Show_Console()
        {
            string html =
            "HTTP/1.1 200 OK\r\n"
            "Content-type: text/html\r\n\r\n"
            "<html>\n"
            "<head>\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\" />\n"
            "<link rel=\"icon\" href=\"data:;base64,iVBORw0KGgo=\">\n"
            "<title>Network Programming Homework 3</title>\n"
            "</head>\n"
            "<body>\n"
            "<font face=\"Courier New\" size=2 color=#FFFFFF>\n"
            "<table>\n"
            "<thead>\n"
            "<tr id=\"tableHead\">\n"
            "%1%"
            "</tr>\n"
            "</thead>\n"
            "<tbody>\n"
            "<tr id=\"tableBody\">\n"
            "%2%"
            "</tr>\n"
            "</tbody>\n"
            "<\table>\n"
            "</font>\n"
            "</body>\n"
            "</html>\n";
            do_write(html);

        }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    char temp[max_length];
    //char status_str[max_length] = "HTTP/1.1 200 OK\n";
    char REQUEST_METHOD[max_length];
    char REQUEST_URI[max_length];
    char REQUEST_CGI[max_length];
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

class CGI_server
{
    public:
    CGI_server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
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
                    std::make_shared<Session>(std::move(socket))->start();
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
      std::cerr << "Usage: CGI_server [port]\n";
      return 1;
    }

    boost::asio::io_context io_context;

    CGI_server s(io_context, std::atoi(argv[1]));

    cout<<"hi Window 11"<<endl;

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}