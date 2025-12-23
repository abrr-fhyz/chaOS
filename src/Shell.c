#include "../lib/Shell.h"
#include "../lib/Handler.h"
#include "../lib/Util.h"

void initShell(){
	shellHandler = createProcess("shell", shell_process);
	printMessage("\t\t\t\tShell Boot Up\t\t\t---\t\t\tCompleted\n");
}