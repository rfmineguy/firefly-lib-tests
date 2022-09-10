#include "firefly/IO/File.h"
#include "firefly/IO/Log.h"
#include <firefly/IO/IO.h>

#include <stdlib.h>

#define NDEBUG      // disables all debugging done via the macros

int main() {
    FILE* f = SetLogStream(FileOpen("res/file.txt", WRITE));
    LOG_WARN("Hello World, %d", 45);
    LOG_CRITICAL("This is a critical error");
    LOG_DEBUG("This is a debug message [%d]", 69);
    LOG_INFO("This is an info message [%d]", 420);
    LOG_ERROR("This is an error message [%d]", 21);
    FileClose(f);

    f = SetReadStream(FileOpen("res/file.txt", READ));
    long size;
    char* text = Read(&size);
    printf("Read text, size=%lu\n", size);
    printf("Contents -> [%s]\n%s", "res/file.txt", text);
    free(text);
    FileClose(f);

    f = SetWriteStream(FileOpen("res/file.txt", APPEND));
    Write("Hello World\n");
    FileClose(f);

    LOG_WARN("Program ending. Successful run");
}
