#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ImageBrightness(unsigned char** in, unsigned char** out, int w, int h, int val);
void ImageFlip(unsigned char** in, unsigned char** out, int w, int h, int mode);
void ImageSizeChange(unsigned char** in, unsigned char** out, int w, int h, float ratio);
int width, height; //원본 파일 크기 변수

int main(void){
    unsigned char **img, **img_new;
    FILE *fp_r, *fp_w = NULL;
    char temp_str[200];
    int width_new, height_new; //새로 생성할 파일의 크기 변수
    int depth;
    int i, j, pixel;
    int mod;
    float ratio;

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
    fscanf(fp_r, "%d", &depth);
    
    //2d memory allocation
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
    //파일 픽셀 값들 읽어오기
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
        {
            fscanf(fp_r, "%d", &img[i][j]);
        }
    
    printf("파일의 크기를 몇 배로 변경하시겠습니까?(0.5 or 2입력 그 외는 원본): ");
    scanf("%f", &ratio);
    if (ratio == 0.5 || ratio == 2)
    {
        width_new = (int)(ratio*(float)width);
        height_new = (int)(ratio*(float)height);
        //printf("ratio실행확인: %d %d\n",width_new, height_new);
    }
    else
    {
        width_new = width;
        height_new = height;
        //printf("ratio실행확인: %d %d\n",width_new, height_new);
    }
    fprintf(fp_w, "%d %d\n", width_new, height_new);
    fprintf(fp_w, "%d\n", depth);//최대치는 같지만 w,h먼저 파일에 적어야 함
    //여기까지는 파일에 써짐
    img_new = (unsigned char**)malloc(height_new*sizeof(unsigned char*));
    for (i = 0; i < height_new; i++)
    {
        img_new[i] = malloc(width_new*sizeof(unsigned char));
    }
    if (img_new == NULL)
    {
        printf("2차원 배열 생성 오류\n");
        exit(1);
    }
    ImageSizeChange(img, img_new, width_new, height_new, ratio);

    printf("밝기를 얼마나 올리시겠습니까?: ");
    scanf("%d", &pixel);
    ImageBrightness(img, img_new, width_new, height_new, pixel);

    printf("* 주의: 파일의 크기를 변경하였다면 대칭기능은 사용하지 마세요 *\n");
    printf("모드를 설정해 주세요(좌우대칭은 0, 상하대칭은 1, 원본은 0,1제외 숫자입력): ");
    scanf("%d", &mod);
    ImageFlip(img, img_new, width_new, height_new, mod);
    
    
    //여기부터 실행이 되질 않음 오류!
    for (i = 0; i < height_new; i++)
        for (j = 0; j < width_new; j++)
        {
            fprintf(fp_w,"%d ",img_new[i][j]);
        }
    //printf("파일 입출력 실행완료\n");
    fclose(fp_r);
    fclose(fp_w);

    strcpy(fpath_name, fpath);
    strcat(fpath_name, fname2);
    system(fpath_name);

    for (i = 0; i < width; i++)
        free(img[i]);
    free(img);

    for (i = 0; i < width_new; i++)
        free(img_new[i]);
    free(img_new);
    
    return 0;
}

void ImageBrightness(unsigned char** in,unsigned char** out, int w, int h, int val)
{   
    int i,j,pixel;
    //pixel = pixel +val
    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
        {   
            pixel = out[i][j] + val;
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
    int i,j;
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

void ImageSizeChange(unsigned char** in, unsigned char** out, int w, int h, float ratio)
{   
    //w,h는 새로운 파일의 크기 변수임
    int i,j;
    //printf("함수 안에 들어온 ratio값:%f\n", ratio);
    //새로운 파일 2차원 배열 공간 할당
    /*
    out = (unsigned char**)malloc(h*sizeof(unsigned char*));
    for (i = 0; i < h; i++)
    {
        out[i] = malloc(w*sizeof(unsigned char));
    }
    if (out == NULL)
    {
        printf("2차원 배열 생성 오류\n");
        exit(1);
    }
    */
    //영상의 크기를 2배로 키우거나 1/2로 줄이는 함수
    if (ratio == 2)//크기 2배
    {   
        for (i = 0; i < height; i++)
            for (j = 0; j < width; j++)
            {   
                //1더하고 2곱하고 1빼면 기준값이 나옴. 그 기준값으로 나머지 채우기
                int a = (i+1)*2-1;
                int b = (j+1)*2-1;
                out[a][b] = in[i][j];
                out[a-1][b] = in[i][j];
                out[a-1][b-1] = in[i][j];
                out[a][b-1] = in[i][j];
                //printf("2배 대입 실행완료 %d %d\n", i, j);
            }
    }
    else if(ratio == 0.5)//크기 0.5배//오류지점?
    {
        for (i = 0; i < height; i++)
            for (j = 0; j < width; j++)
            {   
                if ((i%2) == 0 && (j%2) == 0)//i와 j가 짝수일때 0 포함?
                {   //이게 왜 안될까(i/2) (int)((float)j*0.5)
                    int x = (int)(i*0.5);
                    int y = (int)(j*0.5);
                    out[x][y] = in[i][j];//이 문장이 문제?
                    //printf("0.5배 대입 실행완료 %d %d\n", i, j);
                }
            }
    }
    else
    {
        for (i = 0; i < h; i++)
            for (j = 0; j < w; j++)
            {
                out[i][j] = in[i][j];
                //printf("1배 대입 실행완료 %d %d\n", i, j);
            }
    }
}