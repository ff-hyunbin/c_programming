#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ImageBrightness(unsigned char** in, unsigned char** out, int w, int h, int val);
void ImageFlip(unsigned char** in, unsigned char** out, int w, int h, int mode);

int main(void){
    unsigned char **img, **img_new;
    FILE *fp_r, *fp_w = NULL;
    char temp_str[200];
    int width, height;
    int depth;
    int i, j, pixel;
    int mod;

    char fname[50];
    char fname2[50];
    char fpath[100] = "\"C:\\Program Files\\IrfanView\\i_view64.exe\" ";
    char fpath_name[150];

    printf("읽을 파일의 이름: ");
    scanf("%s", fname);

    printf("새로 만들 파일의 이름: ");
    scanf("%s", fname2);

    fp_r = fopen(fname, "r");
    if (fp_r == NULL)
    {
        printf("파일 열기 실패");
        exit(1);
    }
    fp_w = fopen(fname2, "w");
    if (fp_w == NULL)
    {
        printf("파일 열기 실패");
        exit(1);
    }

    fgets(temp_str, 50, fp_r);
    fputs(temp_str, fp_w);

    fgets(temp_str, 50, fp_r);
    fputs(temp_str, fp_w);

    fscanf(fp_r, "%d", &width);
    fscanf(fp_r, "%d", &height);
    fprintf(fp_w, "%d %d\n", width, height);//%d사이에 1칸 띄어쓰기 중요

    fscanf(fp_r, "%d", &depth);
    fprintf(fp_w, "%d\n", depth);
    //여기까지는 문제없음

    //2d memory allocation 정보를 할당할 2차원 배열 생성?
    img = (unsigned char**)malloc(height*sizeof(unsigned char*));
    for (i = 0; i < height; i++)
    {
        img[i] = malloc(width*sizeof(unsigned char));
    }
    if (img == NULL)
    {
        printf("2차원 배열 생성 오류\n");
        exit(1);
    }


    //새로 생성할 파일의 2차원 배열 생성?
    img_new = (unsigned char**)malloc(height*sizeof(unsigned char*));
    for (i = 0; i < height; i++)
    {
        img_new[i] = malloc(width*sizeof(unsigned char));
    }
    if (img_new == NULL)
    {
        printf("2차원 배열 생성 오류\n");
        exit(1);
    }


    //파일 픽셀 값들 읽어오기//문제
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            fscanf(fp_r, "%d", &img[i][j]);
        }

    printf("밝기를 얼마나 올리시겠습니까?: ");
    scanf("%d", &pixel);
    ImageBrightness(img, img_new, width, height, pixel);
    
    printf("모드를 설정해 주세요(좌우대칭은 0, 상하대칭은 1, 원본은 0,1제외 숫자입력): ");
    scanf("%d", &mod);
    ImageFlip(img, img_new, width, height, mod);

    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            fprintf(fp_w,"%d ",img_new[i][j]);
        } 
    
    fclose(fp_r);
    fclose(fp_w);

    strcpy(fpath_name, fpath);
    strcat(fpath_name, fname2);
    system(fpath_name);

    for (i = 0; i < width; i++)
        free(img[i]);
    free(img);

    for (i = 0; i < width; i++)
        free(img_new[i]);
    free(img_new);
    
    return 0;
}

void ImageBrightness(unsigned char** in,unsigned char** out, int w, int h, int val)
{   
    //파일 픽셀 값 50 늘려서 저장
    int i,j,pixel;
    //pixel = pixel +val
    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
        {   
            pixel = in[i][j] + val;
            if (pixel > 255)
                out[i][j] = 255;
            else if(pixel < 0)
                out[i][j] = 0;
            else
                out[i][j] = pixel;
        }
    
    return;
}

void ImageFlip(unsigned char** in,unsigned char** out, int w, int h, int mode)
{   
    int i,j,pixel;
    if (mode == 0)
    {
        for (i = 0; i < h; i++)
            for (j = 0; j < w; j++)
            {
                out[i][j] = in[i][w-1-j];
            }
    }
    else if(mode == 1)
    {
        for (i = 0; i < h; i++)
            for (j = 0; j < w; j++)
            {
                out[i][j] = in[h-1-i][j];
            }
    }
    else
        return;
    
    return;
}