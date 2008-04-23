#include <iostream>

using namespace std;

class Bodies;

namespace CLogin
{
typedef Bodies Handler;

namespace View
{
typedef bool (Handler::*Func)(string f_name, string f_passwd, string f_server, int f_port);
}

namespace Model
{
typedef void (Handler::*Func)();
}
}

