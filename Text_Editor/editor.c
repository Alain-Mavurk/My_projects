#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <poll.h>

#define SAVE 19
#define ESCAPE 27
#define BACKSPACE 127
#define ARROW_RIGHT "\33[C"
#define ARROW_LEFT "\33[D"
#define ARROW_DOWN "\33[B"
#define ARROW_UP "\33[A"


#define DEFAULT_TEXT "\x1b[39m"
#define BLACK_TEXT "\x1b[30m"
#define DEFAULT_BACKGROUND "\x1b[49m"
#define WHITE_BACKGROUND "\x1b[47m"

#define HIDE_CURSOR "\x1b[?25l"
#define PRINT_CURSOR "\x1b[?25h"

#define MOUSE "/dev/input/mice"

typedef struct{
    char** array;
    int* c_size;
    int l_size;
}file_t;

void* myrealloc(void* alloc, size_t sz){
    alloc = realloc(alloc, sz);
    if(alloc == NULL) exit(1);
    return alloc;
}

void clear_line(){
    write(STDOUT_FILENO, "\33[K", 3);
}

void array_free(file_t file){
    for(int i = file.l_size-1; i >= 0; i--){
        free(file.array[i]);
    }
    free(file.array);
    free(file.c_size);
}

void cursor_set(int x, int y){
    char cursor[1024];
    sprintf(cursor, "\x1b[%d;%dH", x, y);
    write(STDIN_FILENO, cursor, strlen(cursor));
}

void cursor_hidden(char* set){
    write(STDOUT_FILENO, set, strlen(set));
}

void color_set(char* color_b, char* color_text){
    char buff[1024];
    sprintf(buff, "%s%s", color_b, color_text);
    write(STDIN_FILENO, buff, strlen(buff));
}

void print_new(file_t file, int l, int row_max){
    cursor_set(1, 1);
    cursor_hidden(HIDE_CURSOR);
    int i, k;
    if(l >= row_max-2){
        i = l-row_max+3;
        k = l+1;
    }else{
        i = 0;
        k = row_max-2;
    }
    for( ; i < file.l_size && i < k; i++){
        clear_line();
        for(int j = 0; j < file.c_size[i]; j++){
            if(file.array[i][j] == '\n')
                write(STDOUT_FILENO, "\r\n", 2);
            else
                write(STDOUT_FILENO, &file.array[i][j], 1);
        }
    }
    cursor_set(row_max-1, 1);
    clear_line();
    cursor_hidden(PRINT_CURSOR);
}


int main(int argc, char* argv[])
{
    system("clear");
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    struct winsize ws;
    file_t file;
    file.array = NULL;
    file.c_size = NULL;
    file.array = malloc(sizeof(char*));
    *file.array = NULL;
    file.c_size = myrealloc(file.c_size, sizeof(int));
    file.c_size[0] = 0;
    char buff[1024], buff2[1024];
    unsigned char data_mouse[3];
    int fd, size, insert = 0, 
    normal = 1, l = 0, c = 0, row = 0, 
    row_l = 0, fd_mouse;
    fd_mouse = open(MOUSE, O_RDONLY);
    fds[1].fd = fd_mouse;
    fds[1].events = POLLIN;
    if(argc > 1){
        fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    }
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int row_max = ws.ws_row;
    int col_max = ws.ws_col;

    if(argc > 1){
        while ((size = read(fd, buff, 1024)) != 0)
        {
            for(int j =0; j < size; j++){
                if(buff[j] == '\n') {
                    file.array[l] = myrealloc(file.array[l], c+1);
                    file.array[l][c] = buff[j];
                    file.c_size[l]++;
                    l++;
                    file.array = myrealloc(file.array, sizeof(char*)*(l+1));
                    file.array[l] = NULL;
                    file.c_size = myrealloc(file.c_size, (l+1)*sizeof(int));
                    file.c_size[l] = 0;
                    c=0;
                }else{
                    file.array[l] = myrealloc(file.array[l], c+1);
                    file.array[l][c] = buff[j];
                    c++;
                    file.c_size[l]++;
                }
            }
            file.l_size = l+1;
        }
        print_new(file, 0, row_max);
        close(fd);
    }else{
        file.l_size = 1;
    }
    
    struct termios termios_p, termios_o;
    tcgetattr(STDIN_FILENO, &termios_o);
    cfmakeraw(&termios_p);
    tcsetattr(STDIN_FILENO, TCSADRAIN, &termios_p);

    l = 0, c = 0;
    int cur_l = 1, cur_c = 1, cur_mouse_l = 1, cur_mouse_c = 1;
    cursor_set(cur_l, cur_c);
    cursor_hidden(PRINT_CURSOR);



    while (poll(fds, 2, -1) > 0)
    {
        if(fds[0].revents & POLLIN){
            size = read(STDIN_FILENO, buff, 3);
            buff[size] = '\0';
            

            if(!normal){
                if(buff[0] == ESCAPE && strlen(buff) == 1){
                    normal = 1;
                    
                }else if (!strcmp(buff, ARROW_RIGHT))
                {
                    if(l < file.l_size-1){
                        if(c < file.c_size[l]-1){ 
                            c++;
                            if(cur_c >= col_max){
                                cur_l++;
                                cur_c = 1;
                            }else{
                                cur_c++;
                            }
                        }
                    }else{
                        if(c < file.c_size[l]){ 
                            c++;
                            if(cur_c > col_max){
                                cur_l++;
                                cur_c = 1;
                            }else{
                                cur_c++;
                            }
                        }
                    }
                }
                else if (!strcmp(buff, ARROW_LEFT))
                {
                    if(c != 0){
                        if(c >= col_max && cur_c == 1){
                            cur_l--;
                            c--;
                            cur_c = col_max;
                        }else{
                            c--;
                            cur_c--;
                        }
                    }
                }
                else if (!strcmp(buff, ARROW_UP))
                {
                    
                    if(l != 0){
                        l--;
                        if(cur_l > 1){
                            if(file.c_size[l] > col_max){
                                for(int i = 0; i <= file.c_size[l]/col_max  && cur_l > 1; i++){
                                    cur_l--;
                                }
                            }else{
                                cur_l--;
                            }
                        }

                        if(c >= file.c_size[l]){
                            c = file.c_size[l]-1;
                            cur_c = c+1;
                        }
                        row--;
                    }
                }
                else if (!strcmp(buff, ARROW_DOWN))
                {
                    if(l+1 < file.l_size){
                        l++;
                        if(cur_l < row_max-2){
                            if(file.c_size[l-1] > col_max){
                                for(int i = 0; i <= file.c_size[l-1]/col_max  && cur_l < row_max-2; i++){
                                    cur_l++;
                                }
                            }else{
                                cur_l++;
                            }
                        }
                    }

                    if(c >= file.c_size[l]){
                        if(l < file.l_size-1 ){
                            c = file.c_size[l]-1;
                        }else{
                            c = file.c_size[l];
                        }    
                        cur_c = c+1;
                    }
                    row++;
                }
                else if (buff[0] == BACKSPACE)
                {
                    if(insert){
                            
                        if(c == 1 && c == file.c_size[l]){
                            c--;
                            cur_c--;
                            file.array[l][c] = '\0';
                            file.c_size[l]--;
                        }else if(c != 0 && file.c_size[l] > 1){
                            if(c == file.c_size[l]){
                                file.array[l] = myrealloc(file.array[l], file.c_size[l]-1);
                            }else{
                                int i;
                                for(i = c-1; i < file.c_size[l]-1; i++){
                                    file.array[l][i] = file.array[l][i+1];
                                }
                            }
                            c--;
                            cur_c--;
                            file.c_size[l]--;

                        }

                    }else{
                        if(c == 0 && l !=0){
                            l--;
                            cur_l--;
                            c = file.c_size[l]-1;
                            cur_c = c+1;
                        }else if(c != 0){
                            c--;
                            cur_c--;
                        }
                    }

                    if(insert)
                        print_new(file, row_l, row_max);

                }else if(buff[0] == SAVE){
                    if(argc > 1){
                        if(fork() == 0){
                            execlp("rm", "rm", argv[1], NULL);
                            exit(1);
                        }
                        wait(NULL);
                        int fd_save = open(argv[1], O_WRONLY | O_CREAT, 0644);
                        for(int i = 0; i < file.l_size; i++){
                            write(fd_save, file.array[i], file.c_size[i]);
                        }
                        close(fd_save);
                    }
                }
                else
                {
                    if (buff[0] == '\r'){

                        if(file.array[l] != NULL && c < file.c_size[l]){
                            int i = file.l_size-1;
                            file.array = myrealloc(file.array, sizeof(char*)*(file.l_size+1));
                            file.array[i+1] = NULL;
                            file.c_size = myrealloc(file.c_size, (file.l_size+1)*sizeof(int));
                            file.c_size[i+1] = 0;
                            file.l_size++;
                            while(i > l){

                                file.array[i+1] = file.array[i];
                                file.c_size[i+1] = file.c_size[i];
                                i--;
                            }
                            c++;
                            l++;
                            file.array[l] = NULL;
                            int j = 0;
                            for(int i = c; i < file.c_size[l-1]; i++){
                                file.array[l] = myrealloc(file.array[l], j+1);
                                file.array[l][j] = file.array[l-1][i];
                                j++;
                            }
                            if(file.array[l-1][c-1] != '\n'){
                                file.c_size[l-1] -= j-1;
                                file.array[l-1] = myrealloc(file.array[l-1], file.c_size[l-1]);
                                file.array[l-1][c] = '\n';
                                file.c_size[l] = j;
                            }else{
                                file.array[l] = myrealloc(file.array[l], 1);
                                file.array[l][0] = '\n';
                                file.c_size[l] = 1;
                            }
                          
                            c=0;
                            if(cur_l < row_max-2){
                                cur_l++;
                                cur_c = 1;
                            }
                            row++;
                        }else{
                            file.c_size[l]++;
                            file.array[l] = myrealloc(file.array[l], c+1);
                            file.array[l][c] = '\n';
                            l++;
                            file.array = myrealloc(file.array, sizeof(char*)*(file.l_size+1));
                            file.array[file.l_size] = NULL;
                            file.c_size = myrealloc(file.c_size, (file.l_size+1)*sizeof(int));
                            file.c_size[file.l_size] = 0;
                            file.l_size++;
                            c=0;
                            cur_c = 1;
                            if(cur_l < row_max-2){
                                cur_l++;
                            }
                            row++;
                        }
                    }else{

                        if(file.array[l] != NULL && file.array[l][c] == '\n'){
                            file.array[l] = myrealloc(file.array[l], c+2);
                            file.array[l][c+1] = '\n';
                            file.array[l][c] = buff[0];
                            c++;
                            file.c_size[l]++;
                        }else if(c == file.c_size[l] || file.c_size[l] == 0){
                            file.array[l] = myrealloc(file.array[l], c+1);
                            file.array[l][c] = buff[0];
                            file.c_size[l]++;
                            c++;
                        }else{
                            if(insert){
                                file.array[l] = myrealloc(file.array[l], file.c_size[l]+1);
                                for(int i = file.c_size[l]; i >= c; i--){
                                    file.array[l][i] = file.array[l][i-1];
                                }
                                file.c_size[l]++;
                            }    
                            file.array[l][c] = buff[0];
                            c++;
                        }
                        if(cur_c <= col_max){
                            cur_c++;
                        }else{
                            if(l < file.l_size-1){
                                if(cur_l < row_max-2){
                                    cur_l++;
                                }
                                row++;
                                cur_c = 1;
                            }
                        }
                    }
                    print_new(file, row_l, row_max);
                }

            }else{
                if(buff[0] == ':'){
                    tcsetattr(STDIN_FILENO, TCSADRAIN, &termios_o);
                    cursor_set(row_max-1, 1);
                    color_set(WHITE_BACKGROUND, BLACK_TEXT);
                    for(int i = 0; i < col_max; i++){
                        write(STDOUT_FILENO, " ", 1);
                    }
                    cursor_set(row_max-1, 1);
                    write(STDOUT_FILENO, buff, 1);
                    size = read(STDIN_FILENO, buff, 1024);
                    buff[size-1] = '\0';

                    if(!strncmp(buff, "q!", 2)){
                        exit(0);
                    }else if(!strcmp(buff, "w")){
                        if(argc > 1){
                            if(fork() == 0){
                                execlp("rm", "rm", argv[1], NULL);
                                exit(1);
                            }
                            wait(NULL);
                            int fd_save = open(argv[1], O_WRONLY | O_CREAT, 0644);
                            for(int i = 0; i < file.l_size; i++){
                                write(fd_save, file.array[i], file.c_size[i]);
                            }
                            close(fd_save);
                        }
                    }else if(!strncmp(buff, "w", 1) && strlen(buff) > 2){
                        int fd_save = open(buff+2, O_WRONLY | O_CREAT, 0644);
                        for(int i = 0; i < file.l_size; i++){
                            write(fd_save, file.array[i], file.c_size[i]);
                        }
                        close(fd_save);
                    }
                    color_set(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                    tcsetattr(STDIN_FILENO, TCSADRAIN, &termios_p);
                    print_new(file, row_l, row_max);
                }
                else if(buff[0] == SAVE){
                    if(argc > 1){
                        if(fork() == 0){
                            execlp("rm", "rm", argv[1], NULL);
                            exit(1);
                        }
                        wait(NULL);
                        int fd_save = open(argv[1], O_WRONLY | O_CREAT, 0644);
                        for(int i = 0; i < file.l_size; i++){
                            write(fd_save, file.array[i], file.c_size[i]);
                        }
                        close(fd_save);
                    }
                }
                else if(buff[0] == 'i'){
                    normal = 0;
                    insert = 1;
                }else if(buff[0] == 'r'){
                    normal = 0;
                    insert = 0;
                }            
                else if (!strcmp(buff, ARROW_RIGHT))
                {
                    if(l < file.l_size-1 ){
                        if(c < file.c_size[l]-1){ 
                            c++;
                            if(cur_c >= col_max){
                                cur_l++;
                                row++;
                                cur_c = 1;
                            }else{
                                cur_c++;
                            }
                        }
                    }else{
                        if(c < file.c_size[l]){ 
                            c++;
                            if(cur_c > col_max){
                                cur_l++;
                                row++;
                                cur_c = 1;
                            }else{
                                cur_c++;
                            }
                        }
                    }
                }
                else if (!strcmp(buff, ARROW_LEFT))
                {
                    if(c != 0){
                        if(c >= col_max && cur_c == 1){
                            cur_l--;
                            row--;
                            c--;
                            cur_c = col_max;
                        }else{
                            c--;
                            cur_c--;
                        }
                    }
                }
                else if (!strcmp(buff, ARROW_UP))
                {
                    
                    if(l != 0){
                        l--;
                        if(cur_l > 1){
                            if(file.c_size[l] > col_max){
                                for(int i = 0; i <= file.c_size[l]/col_max  && cur_l > 1; i++){
                                    cur_l--;
                                }
                            }else{
                                cur_l--;
                            }
                        }
                        
                        if(c >= file.c_size[l]){
                            c = file.c_size[l]-1;
                            cur_c = c+1;
                        }
                        row--;
                    }
                }
                else if (!strcmp(buff, ARROW_DOWN))
                {
                    if(l+1 < file.l_size){
                        l++;
                        if(cur_l < row_max-2){
                            if(file.c_size[l-1] > col_max){
                                for(int i = 0; i <= file.c_size[l-1]/col_max  && cur_l < row_max-2; i++){
                                    cur_l++;
                                }
                            }else{
                                cur_l++;
                            }
                        }
                    }
                    

                    if(c >= file.c_size[l]){
                        if(l < file.l_size-1 ){
                            c = file.c_size[l]-1;
                        }else{
                            c = file.c_size[l];
                        }    
                        cur_c = c+1;
                    }
                    row++;
                }
                else if (buff[0] == BACKSPACE)
                {


                    if(c == 0 && l != 0){
                        l--;
                        row--;
                        if(cur_l > 1){
                            cur_l--;
                        }
                        c = file.c_size[l]-1;
                        cur_c = c+1;
                    }else if (c != 0){
                        c--;
                        cur_c--;
                    }

                }else{
                    normal = 1;
                    insert = 0;
                }
            }    

            
        }

        if(fds[1].revents & POLLIN){
            read(fd_mouse, data_mouse, 3);
            int left = 0;
            char x, y;
            left = data_mouse[0] & 0x1;
            x = data_mouse[1];
            y = data_mouse[2];
            if(x == -1 && cur_mouse_c > 1){
                cur_mouse_c--;
            }else if(x == 1 && cur_mouse_c < 70){
                cur_mouse_c++;
            }
            if(y == 1 && cur_mouse_l > 1){
                cur_mouse_l--;
            }else if(y == -1 && cur_mouse_l < row_max-2){
                cur_mouse_l++;
            }

            if(left){
                if(file.l_size < row_max-2){
                    if(cur_mouse_l < file.l_size){
                        l = cur_mouse_l - 1;
                        cur_l = cur_mouse_l;
                        row = cur_l-1;
                        if(cur_mouse_c < file.c_size[l]){
                            c = cur_mouse_c -1;
                            cur_c = cur_mouse_c;
                        }else{
                            c = file.c_size[l]-1;
                            cur_c = c+1;
                        }
                    }else{
                        l = file.l_size -1;
                        cur_l = l+1;
                        row = cur_l-1;
                        if(cur_mouse_c < file.c_size[l]){
                            c = cur_mouse_c -1;
                            cur_c = cur_mouse_c;
                        }else{
                            c = file.c_size[l]-1;
                            cur_c = c+1;
                        }
                    }
                }else{
                    if(cur_l > cur_mouse_l){
                        int decalage = cur_l - cur_mouse_l;
                        l -= decalage;
                        cur_l = cur_mouse_l;
                        row = cur_l-1;
                        if(cur_mouse_c < file.c_size[l]){
                            c = cur_mouse_c -1;
                            cur_c = cur_mouse_c;
                        }else{
                            c = file.c_size[l]-1;
                            cur_c = c+1;
                        }
                    }else{
                        int decalage = cur_mouse_l - cur_l;
                        l += decalage;
                        cur_l = cur_mouse_l;
                        row = cur_l-1;
                        if(cur_mouse_c < file.c_size[l]){
                            c = cur_mouse_c -1;
                            cur_c = cur_mouse_c;
                        }else{
                            c = file.c_size[l]-1;
                            cur_c = c+1;
                        }
                    }
                }
            }

        }

        if(row >= row_max-2){

            row_l = l;
            print_new(file, row_l, row_max);
            row--;
        }else if(row < 0){
            row_l--;
            print_new(file, row_l, row_max);
            row++;
        }

        cursor_set(row_max, 1);
        clear_line();
        

        cursor_set(row_max, col_max/2 -3);
        cursor_hidden(HIDE_CURSOR);
        if(!normal && insert){
            write(STDOUT_FILENO, "Mode insertion", 14);
        }else if(!normal && !insert){
            write(STDOUT_FILENO, "Mode remplacement", 17);
        }else{
            write(STDOUT_FILENO, "Mode normal", 11);
        }
        cursor_set(row_max, 1);
        sprintf(buff2, "l: %d c: %d", l+1, c+1);
        write(STDOUT_FILENO, buff2, strlen(buff2));

        cursor_set(cur_l, cur_c);
        cursor_hidden(PRINT_CURSOR);
    }

    array_free(file);
    tcsetattr(STDIN_FILENO, TCSADRAIN, &termios_o);
    return 0;
}
