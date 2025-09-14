#include "fileDestination.h"
#include "consoleDestination.h"
#include "collector.h"
#include "source.h"
#include "parser.h"

using namespace std;

size_t N = 3;

int main(int argc, const char *argv[])
{

    if (2 == argc)
        N = strtoul(argv[1], 0, 10);

    shared_ptr<consoleDestination> console = make_shared<consoleDestination>();
    shared_ptr<fileDestination> files = make_shared<fileDestination>();

    shared_ptr<collector> a = make_shared<collector>();
    shared_ptr<grubFromCin> source = make_shared<grubFromCin>();
    cmdParser parser(a, N);

    a->registration(console);
    a->registration(files);

    parser.parseIt(source);

    return 0;
}
