#include <stdio.h>
#include <string.h>

int main()
{
    FILE* fp = fopen("tmp.txt", "w+");
    if(!fp)
    {
        perror("fopen");
        return 0;
    }

    const char* lp = "linux-60"; //2*7 
    //int ret = fwrite(lp, 2, 4, fp);
    int ret = fwrite(lp, 1, strlen(lp), fp);
    printf("write ret = [%d]\n", ret);

    //SEEK_SET--移动到文件的头部
    //SEEK_CUR--移动到文件的当前位置，配合offset
    //SEEK_END--移动到为文件的尾部
    fseek(fp, 0, SEEK_SET);

    char arr[1024] = {0};
    //ret = fread(arr, 2, 4, fp);
    ret = fread(arr, 1, sizeof(arr) - 1, fp);
    if(ret <= 0)
    {
        perror("fread");
    }
    printf("read ret = [%d][%s]\n", ret, arr);

    fclose(fp);
    return 0;
}
