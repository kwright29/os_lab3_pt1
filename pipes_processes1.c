// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100];
		char input_str1[100];
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        close(fd1[0]);  // Close the read end of pipe1

        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        char concat_str1[100];

        
        // Wait for child to send a string 
        wait(NULL); 
        close(fd2[1]); // Close the write end of pipe2
        read(fd2[0], concat_str1, 100);

        printf("Concatenated string: %s\n", concat_str1);
        close(fd2[0]); // Close writing end of pipes 
    } 
  
    // child process 
    else
    {   
        close(fd1[1]);  // Close writing end of pipe1
      
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Concatenated string: %s\n", concat_str);
        close(fd2[0]); 

        printf("Enter a another string to concatenate:");
        scanf("%s", input_str1); 

				int j = strlen(concat_str); 
        int m; 
        for (m=0; m<strlen(input_str1); m++) 
            concat_str[j++] = input_str1[m]; 
  
        concat_str[j] = '\0';   // string ends with '\0' 

        write(fd2[1],concat_str,strlen(concat_str)+1);
        close(fd2[1]);
    
        exit(0); 
    } 
} 