#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


char*
fmtname(char *path)
{
	char *p;

	// Find first character after last slash.
	for(p=path+strlen(path); p >= path && *p != '/'; p--)
		;
	p++;

	return p;
}

void
find(char *path, char *targetname){
    char buf[512], *p;
	int fileDescriptor;
	struct dirent directoryEntry;
	struct stat fileStat;

    if (!strcmp(fmtname(path), targetname)) {
		printf("%s\n", path);
	}

    if((fileDescriptor = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fileDescriptor, &fileStat) <  0){
        fprintf(2, "find: cannot stat%s\n",path);
        close(fileDescriptor);
        return;
    }

    if(fileStat.type != T_DIR){
        close(fileDescriptor);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
		printf("find: path too long\n");
		close(fileDescriptor);
		return;
	}
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/';

    while (read(fileDescriptor, &directoryEntry, sizeof(directoryEntry)) == sizeof(directoryEntry)) {
		if (directoryEntry.inum == 0)
			continue;
		memmove(p, directoryEntry.name, DIRSIZ);
		p[DIRSIZ] = 0;
    if (!strcmp(directoryEntry.name, ".") || !strcmp(directoryEntry.name, ".."))
			continue;
		find(buf, targetname);
    }
	close(fileDescriptor); 
}


int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(2, "usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}