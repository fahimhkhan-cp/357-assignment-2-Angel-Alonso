#include<stdio.h>
#include <stdint.h>
#include<string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#define NAME_LEN 32 

typedef struct
{
    uint32_t current_inode;
    char current_in[1024];

}FileSystem;

int32_t entry_in_dir(uint32_t dir_inode ,const char *name){
    
    char filename[32];
     sprintf(filename, "%u", dir_inode);
    FILE *f = fopen(filename,"rb");

    uint32_t inode;

    unsigned char entry_name[NAME_LEN]; 
    while (fread(&inode,sizeof(uint32_t),1,f)== 1 && fread(entry_name,1,NAME_LEN,f) == NAME_LEN) {
        entry_name[NAME_LEN - 1] = '\0'; 
        
        if (strcmp((char*)entry_name,name)==0 ){
            fclose(f);
            return inode;
        }
    }
    fclose(f);
   
     return -1;

}

char inode_type(uint32_t inode) {
    FILE *f = fopen("inodes_list", "rb");
    if (!f) {
        perror("Error opening file");
        return '.' ;
    }

    uint32_t index;
    char type;

    while (fread(&index, sizeof(uint32_t), 1, f) == 1 &&
           fread(&type, sizeof(char), 1, f) == 1) {
        if (index == inode){
            fclose(f);
            return type;
        }

    }


    fclose(f);

    return'.';
}

void print_inode_file(const char *inode_filename) {
    //for cat if I have time 

    FILE *f = fopen(inode_filename, "rb");
    if (!f) {
        perror("Error opening file");
        return;
    }

    unsigned char buf[NAME_LEN];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        fwrite(buf, 1, n, stdout);
    }

    if (ferror(f)) perror("read regular inode file");
    fclose(f);

    printf("\n");
}

void ls(FileSystem *fs){
    
    char filename[32];
    
    sprintf(filename,"%u", fs->current_inode);
    
    FILE*f = fopen(filename,"rb");
   
    uint32_t inode;
    unsigned char name[NAME_LEN];
   
    while(fread(&inode,sizeof(uint32_t),1,f)== 1 && fread(name,1,NAME_LEN,f)== NAME_LEN){
        name[NAME_LEN -1 ]= '\0';
        printf("%u %s \n",inode,name );
    }
    fclose(f);

}

void cd(FileSystem *fs, const char *dirname ){
    int32_t new_indoe = entry_in_dir(fs -> current_inode,dirname);
    char type = inode_type(new_indoe);
    if (type != 'd'){
        return;
    }
    fs-> current_inode = new_indoe;
}

int main (int argc, char *argv[]) {
    chdir(argv[1]);
    FileSystem fs ;
    fs.current_inode = 0;
    char cmd[1024];

    while(1){
        printf("%s",fs.current_in);

        cmd[strcspn(cmd, "\n")] = '\0';

        if(strcmp(cmd, "exit")==0){
            break;
        }
        else if(strcmp(cmd,"ls")== 0){
            ls(&fs);
        }
    }


}
