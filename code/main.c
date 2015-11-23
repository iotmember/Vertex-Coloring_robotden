
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//-------------------------Global Var & Defines------------------------
char *FileInput="input.txt";
char *FileOutput="output.txt";
#define MAX_VERTEX      20                              //So dinh toi da cua do thi
unsigned int n=0;                                         //Bien n chua so dinh thuc te cua Do thi
unsigned int SacSo=0;                                   //So mau Toi thieu de to mau cho do thi
unsigned int V[MAX_VERTEX][MAX_VERTEX];         //Danh sach KE cua cac dinh
unsigned int Deg[MAX_VERTEX];                       //Luu so bac cua cac dinh, sap xep theo thu tu KHONG TANG
unsigned int Color[MAX_VERTEX];                       //Mau duoc to cho cac dinh
unsigned int V_Sort[MAX_VERTEX];                //Can mot Mang Trung gian: chua ten dinh da dc sx theo thu tu KHONG TANG cua deg
unsigned int sothaotac=0;

//------------------Thu tuc Nhap do thi tu file input.txt-----------------
//G={V,E}
//n: so dinh
//input.txt la file dau vao cho Danh sach Ke cua do thi G
//Dong dau tien: n
//Cac dong sau: Theo thu tu cac dinh tu 1-n. cac so sau la Cac dinh ke no. So 0 la dau hieu ket thuc
//Phan tu dau tien cua mang la So Bac cua dinh. VD: bac 0= V[0][0]=3, bac 1= V[1][0]=2
void InputGraph(void)
{
    unsigned int i=0,j=0;
    unsigned int val=1;

    //open file to read data
    FILE *finput;
    finput=fopen(FileInput,"rt");
    if(finput==NULL)
    {
        printf("\nKhong co file du lieu dau vao cua Do thi.");
        exit(1);
    }

    //read quantify of vertex
    fscanf(finput,"%d",&n);
    if(n==0)
    {
        printf("\nn=0. Do thi khong co dinh nao de xet.");
        exit(1);
    }

    //read Near List
    for(i=0;i<n;++i)
    {
        j=0;
        while(1)
        {
            fscanf(finput,"%d",&val);
            if(val==0)
            {
                V[i][0]=j;          //deg
                break;
            }
            V[i][++j]=val;

        }
    }


    //print to screen for debug
    printf("\n=====================DATA OF INPUT GRAPH======================\nSo dinh= %d\n",n);
    printf("\nDinh\tBac\tDanh sach Ke\n");
    for(i=0;i<n;++i)
    {
        printf("%d\t%d\t",(i+1),V[i][0]);
        for(j=1;j<=V[i][0];++j) printf("%d ",V[i][j]);
        printf("\n");
    }


    //close file
    fclose(finput);
}


//---------------------------Ham Thuoc?-------------------------
//Kiem tra xem Dinh j co thuoc Danh sach Ke cua dinh i hay k
//Neu co return 1
//Nguoc lai return 0
//Chi so bat dau tu 0
unsigned int Ke(int dinh_goc, int dinh_can_kt)
{
    unsigned int j=0;

    for(j=1;j<=V[dinh_goc][0];++j)
    {
        if(dinh_can_kt==(V[dinh_goc][j]-1)) return 1;           //Dinh can kiem tra THUOC DS Ke cua Dinh goc
    }

    return 0;
}


//---------------------------Thu tuc To mau-------------------------
//Quy uoc mau theo So: 1..n
//Can toi uu lai ky thuat lap trinh
void VertexColoring(void)
{
    unsigned int i=0,j=0,k=0,tg=0,a=0;
    unsigned int ColorBuffer[MAX_VERTEX];      //Mang nay chua cac dinh da duoc duoc To mau hien hanh
    //ColorBuffer[0]    //So dinh da duoc To mau hien hanh
    //Color[i]           //Ten cac dinh da duoc To mau hien hanh

    //Cap nhat deg cua cac dinh
    for(i=0;i<n;++i)
    {
        Deg[i]=V[i][0];
        V_Sort[i]=i;      //Ten dinh ban dau Luu trong Mang trung gian chua Ten Dinh
    }

    //Sap xep theo thu tu KHONG TANG - Don gian su dung SX Chon
    for(i=0;i<n-1;++i)
    {
        k=i;
        for(j=i+1;j<n;++j)
        {
            if(Deg[j]>Deg[k]) k=j;
        }
        if(k!=i)
        {
            tg=Deg[i]; Deg[i]=Deg[k]; Deg[k]=tg;                        //Doi cho Mang luu Deg
            tg=V_Sort[i]; V_Sort[i]=V_Sort[k]; V_Sort[k]=tg;        //Doi cho Mang luu Ten dinh trung gian
        }
    }

    //Bat dau to mau theo Welch-Powell Algorithm
    for(i=0;i<n;++i) Color[i]=0;                    //Chua dinh nao duoc To mau
    SacSo=0;
    for(i=0;i<n;++i)
    {
        if (Color[V_Sort[i]]==0)                                //Dinh duoc xet chua duoc to mau
        {
            SacSo++;
            Color[V_Sort[i]]=SacSo;                             //To mau cho Dinh duoc xet
            ColorBuffer[0]=1; ColorBuffer[1]=V_Sort[i];     //Ghi chu Dinh nay da duoc To mau hien hanh


            for(j=i+1;j<n;++j)                          //To mau cho cac dinh khong Ke voi dinh duoc xet
            {
                if((Color[V_Sort[j]]==0)&&(!Ke(V_Sort[i],V_Sort[j])))
                {

                    //Kiem tra xem Dinh j co la lien ke cua cac Dinh da duoc To mau hien anh
                    for(k=2;k<=ColorBuffer[0];++k)
                    {
                        if(Ke(ColorBuffer[k],V_Sort[j]))
                        {

                            goto next;
                        }
                    }
                    Color[V_Sort[j]]=SacSo;                     //Gan mau do cho cac Dinh khong thuoc DS Ke cua dinh dang xet
                    ColorBuffer[0]+=1; ColorBuffer[ColorBuffer[0]]=V_Sort[j];


                }
                next:a=0;
            }

        }

    }



}

//---------------------------Thu tuc Xuat du lieu ra file-------------------------
void OutputData(void)
{
    int i=0,j=0;
    //open file to write data
    FILE *foutput;
    foutput=fopen(FileOutput,"wt");
    if(foutput==NULL)
    {
        printf("\nCo loi khi tao file de ghi ket qua dau ra.");
        exit(1);
    }

    //In ra Man hinh
    printf("\n=====================DATA OF OUTPUT RESULTS======================");
    printf("\n1.Day cac Dinh sap xep theo thu tu KHONG TANG cua Bac:\n");
    for(i=0;i<n;++i) printf("  %d   ",V_Sort[i]+1);
    printf("\nDinh\tBac\tDanh sach Ke\n");                  //Danh sach ke cua Graph da duoc Sap xep
    for(i=0;i<n;++i)
    {
        printf("%d\t%d\t",V_Sort[i]+1,V[V_Sort[i]][0]);
        for(j=1;j<=V[V_Sort[i]][0];++j) printf("%d ",V[V_Sort[i]][j]);
        printf("\n");
    }
    printf("\n\n2.KET QUA TO MAU DO THI TREN THEO THUAT TOAN WELCH-POWELL:");
    printf("\nDinh\tMau");
    for(i=0;i<n;++i) printf("\n%d\t%d",i+1,Color[i]);
    printf("\nSac so= %d",SacSo);

    //printf("\nSo thao tac= %d",sothaotac);

    //In ra file
    fprintf(foutput,"=====================DATA OF INPUT GRAPH======================\nSo dinh= %d\n",n);
    fprintf(foutput,"\nDinh\tBac\tDanh sach Ke\n");
    for(i=0;i<n;++i)
    {
        fprintf(foutput,"%d\t%d\t",(i+1),V[i][0]);
        for(j=1;j<=V[i][0];++j) fprintf(foutput,"%d ",V[i][j]);
        fprintf(foutput,"\n");
    }
    fprintf(foutput,"\n=====================DATA OF OUTPUT RESULTS======================");
    fprintf(foutput,"\n1.Day cac Dinh sap xep theo thu tu KHONG TANG cua Bac:\n");
    for(i=0;i<n;++i) fprintf(foutput,"  %d   ",V_Sort[i]+1);

    fprintf(foutput,"\nDinh\tBac\tDanh sach Ke\n");                 //Danh sach ke cua Graph da duoc Sap xep
    for(i=0;i<n;++i)
    {
        fprintf(foutput,"%d\t%d\t",V_Sort[i]+1,V[V_Sort[i]][0]);
        for(j=1;j<=V[V_Sort[i]][0];++j) fprintf(foutput,"%d ",V[V_Sort[i]][j]);
        fprintf(foutput,"\n");
    }

    fprintf(foutput,"\n\n2.KET QUA TO MAU DO THI TREN THEO THUAT TOAN WELCH-POWELL:");
    fprintf(foutput,"\nDinh\tMau");
    for(i=0;i<n;++i) fprintf(foutput,"\n%d\t%d",i+1,Color[i]);
    fprintf(foutput,"\nSac so= %d",SacSo);


    //close file
    fclose(foutput);
}


//---------------------------MAIN FUNCTION-------------------------
int main()
{
    printf("----------------------------------------------------");
    printf("\nVertex Coloring with Welch-Powell Algorithm.\n");
    printf("Demo by Do Thanh Nam - Master of IT 27A - MTA.\n");
    printf("----------------------------------------------------\n\n");
    //1.Read data
    InputGraph();

    //2.Coloring
    VertexColoring();

    //3.Write to file
    OutputData();

    //
    getch();
    return 0;
}
