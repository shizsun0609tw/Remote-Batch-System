#include "console.h"

Console::Console()
{

}

void Console::Link2Server()
{

}

void Console::Run()
{

}

void Console::SendInitialHTML()
{
	std::string contentType;
	std::string initHtml;

	contentType = "Content-type: text/html\r\n\r\n";
	initHTML =
		"<!DOCTYPE html>
		<html lang=\"en\">
  			<head>
    			<meta charset=\"UTF-8\" />
    			<title>NP Project 3 Sample Console</title>
    			<link
      				rel=\"stylesheet\"
      				href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\"
      				integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\"
      				crossorigin=\"anonymous\"
    			/>
    			<link
      				href=\"https://fonts.googleapis.com/css?family=Source+Code+Pro\"
      				rel=\"stylesheet\"
    			/>
    			<link
      				rel=\"icon\"
      				type=\"image/png\"
      				href=\"https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png\"
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
    			<table class=\"table table-dark table-bordered\">
      				<thead>
        				<tr>
	          				<th scope=\"col\">nplinux1.cs.nctu.edu.tw:1234</th>
    	      				<th scope=\"col\">nplinux2.cs.nctu.edu.tw:5678</th>
			        	</tr>
      				</thead>
      				<tbody>
        				<tr>
          					<td><pre id=\"s0\" class=\"mb-0\"></pre></td>
          					<td><pre id=\"s1\" class=\"mb-0\"></pre></td>
        				</tr>
      				</tbody>
    			</table>
  			</body>
		</html>
		"

}

void Console::SendShellOutput()
{

}

void Console::GetShellInput()
{

}

void Console::GetShellOutput()
{

}


int main(int argc, char* argv[])
{
	Console myConsole;

	myConsole.Link2Server();

	myConsole.Run();

	return 0;
}
