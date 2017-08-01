#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>

#define BUFFSIZE 1000000
long read_file(char *filename, char *text);
void createProcess(char *file, long size);
int readA(char *file, long size, int letter);
void print(int n);

/*
* 
*/
int main(void) {
int k;
char *read;

long i = read_file("hello.txt", read);
// printf("This process has PID: %d\n", getpid());
// printf("This process has a parent process with PID: %d\n", getppid());
return (0);
}

long read_file(char *filename, char *text) {
int j = 0;
FILE *fp = fopen(filename, "r");

long size = 0; // Number of characters read.
int len = 0;

if (fp == NULL) {
 fprintf(stderr, "Error could not open file: %s\n", strerror(errno));
return -1;
}
// Go to the end of the file.
if (fseek(fp, 0L, SEEK_END) == 0) {

// Get the size of the file.
size = ftell(fp);
if (size == -1) {

fprintf(stderr, "Error could not open file: %s\n", strerror(errno));
return -1;
}
// Go back to the start of the file.
if (fseek(fp, 0L, SEEK_SET) != 0) {

fprintf(stderr, "Error rewinding to start of file: %s\n", strerror(errno));
return -1;
}
text = (char*) malloc(sizeof (char) *size);

// Read the entire file into memory.
len = fread(text, sizeof (char), (size_t) size, fp);



if (len == 0) {

fprintf(stderr, "Error reading file into memory: %s\n", strerror(errno));
return -1;
} else {
text[++len] = '\0'; // Add a null-terminator, just to be safe.
}
}

(void) fclose(fp);
createProcess(text, size);
return size;
}
// creates different process and call methods

void createProcess(char *text, long size) {
int PROCS = 26;
int status, nbytes;

char buffer[BUFFSIZE];
int i = 0, j = 0,k=0,l=0;

// Remember: pipes have a "read"
// end and a "write" end.
int pipes[PROCS][2];
pid_t ret[PROCS];
// Create a pipe for every child.
for (i = 0; i < PROCS; i++) {
if (pipe(pipes[i]) != 0) {
perror("pipe()");
exit(1);
}
}
// create child process
for (j = 0;j < PROCS; j++) {
ret[j] = fork();
if (ret[j] < 0) {
perror("fork()");
printf("From error %d",j);
abort();
} else if (ret[j] == 0) {
close(pipes[j][0]);
if(write(pipes[j][1],text,(strlen(text)+1)) != (strlen(text+1))){
// printf("from process %d \n",j);
int count = readA(text, size, j + 65);
print(count);
abort();
}else{

}
close(pipes[j][1]);

}
}
if (getpid() > 0) { // Parent
// Wait for each child
// proces to complete.
for (k = 0; k < PROCS; k++) {
ret[k]= wait(&status);
}
// Read from each pipe 
for (l = 0; l < PROCS; l++) {
close(pipes[l][1]);
// read(pipes[i][0], &msg, 25);
nbytes = read(pipes[l][0],buffer,sizeof(buffer));
close(pipes[l][0]);
if(nbytes <0){
perror("read");
abort();
}else{
close(pipes[l][0]);

}
}
// Do any other work in parent.

}

}


// count and print A

int readA(char* file, long size, int letter) {

long i, count = 0;
char c;
for (i = 0; i < size; ++i) {
c = toupper(*file++);
if (c == letter) {
count++;
}
}
printf("Letter %c(%ld)\n", letter, count);

return count;
}
// count and print Other


// print star accoding to lenth of letters

void print(int n) {

int i, div = 1;

while ((div * div) <= n)
div++;
if (div > 80)
div = 80;

for (i = 1; i <= n; i++) {
printf("*");
if ((i % div) == 0)
printf("\n");
}
}
