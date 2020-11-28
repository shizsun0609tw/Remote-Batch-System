#include "panel.h"

#include <dirent.h>
#include <iostream>
#include <vector>

Panel::Panel()
{

}

void Panel::Run(boost::asio::ip::tcp::socket &socket)
{
    std::string FORM_METHOD = "GET";
    std::string FORM_ACTION = "console.cgi";
    std::string TEST_CASE_DIR = "test_case";

    std::vector<std::string> testFile;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(TEST_CASE_DIR.c_str())) != NULL)
    {
        std::string temp;
        while((ent = readdir(dir)) != NULL)
        {
            temp = std::string(ent->d_name);
            if (temp != "." && temp != "..")
            {
                testFile.push_back(temp);
            }
        }
        closedir(dir);
    }
    else
    {
        perror("");
    }

    std::string TEST_CASE_MENU;
    for (int i = 0; i < (int)testFile.size(); ++i)
    {
        TEST_CASE_MENU += "<option value=\"" + testFile[i] + "\">" + testFile[i] + "</option>";
    }

    std::string DOMAIN = ".cs.nctu.edu.tw";
    std::vector<std::string> hosts;
    std::vector<std::string> host_menu;
    for (int i = 0; i < 12; ++i) 
    {
        hosts.push_back("nplinux" + std::to_string(i + 1));
    }
    for (int i = 0; i < (int)hosts.size(); ++i)
    {
        host_menu.push_back("<option value=\"" + hosts[i] + DOMAIN + "\">" + hosts[i] + "</option>");
    }

    DoWrite(socket, "<Content-type>:text/html\r\n\r\n");

    DoWrite(socket, 
    "<!DOCTYPE html>\
        <html lang=\"en\">\
            <head>\
                <title>NP Project 3 Panel</title>\
                <link\
                    rel=\"stylesheet\"\
                    href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\"\
                    integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\"\
                    crossorigin=\"anonymous\"\
                />\
                <link\
                    href=\"https://fonts.googleapis.com/css?family=Source+Code+Pro\"\
                    rel=\"stylesheet\"\
                />\
                <link\
                    rel=\"icon\"\
                    type=\"image/png\"\
                    href=\"https://cdn4.iconfinder.com/data/icons/iconsimple-setting-time/512/dashboard-512.png\"\
                />\
                <style>\
                    * {\
                        font-family: \'Source Code Pro\', monospace;\
                    }\
                </style>\
            </head>\
            <body class=\"bg-secondary pt-5>\">");

   DoWrite(socket,
                "<form action=\"" + FORM_ACTION + "\" method=\"" + FORM_METHOD + "\">\
                    <table class=\"table mx-auto bg-light\" style=\"width: inherit\">\
                        <thead class=\"thead-dark\">\
                            <tr>\
                                <th scope=\"col\">#</th>\
                                <th scope=\"col\">Host</th>\
                                <th scope=\"col\">Port</th>\
                                <th scope=\"col\">Input File</th>\
                            </tr>\
                        </thead>\
                        <tbody>");

    for (int i = 0; i < 5; ++i)
    {
        std::string temp = 
                            "<tr>\
                                <th scope=\"row\" class=\"align-middle\">Session " + std::to_string(i + 1) + "</th>\
                                <td>\
                                    <div class=\"input-group\">\
                                        <select name=\"h" + std::to_string(i + 1) + "\" class=\"custom-select\">\
                                            <option></option>";
        for (int j = 0; j < (int)host_menu.size(); ++j)
        {
            temp += host_menu[j];
        }
        temp +=                        "</select>\
                                        <div class=\"input-group-append\">\
                                            <span class=\"input-group-text\">.cs.nctu.edu.tw</span>\
                                        </div>\
                                    </div>\
                                </td>\
                                <td>\
                                    <input name=\"p" + std::to_string(i) + "\" type=\"text\" class=\"form-control\" size=\"5\" />\
                                </td>\
                                <td>\
                                    <select name=\"f" + std::to_string(i) + "\" class=\"custom-select\">\
                                        <option></option>" + TEST_CASE_MENU +
                                    "</select>\
                                </td>\
                            </tr>";
        DoWrite(socket, temp);
    }

    DoWrite(socket,
                            "<tr>\
                                <td colspan=\"3\"></td>\
                                <td>\
                                    <button type=\"submit\" class=\"btn btn-info btn-block\">Run</button>\
                                </td>\
                            </tr>\
                        </tbody>\
                    </table>\
                </form>\
            </body>\
        </html>");
}

void Panel::DoWrite(boost::asio::ip::tcp::socket &socket, std::string content)
{
    boost::asio::async_write(socket, boost::asio::buffer(content, content.length()),
        [this](boost::system::error_code ec, std::size_t )
        {
            if (ec) return;
        });
}