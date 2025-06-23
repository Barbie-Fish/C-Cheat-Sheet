#include <stdlib.h>  // ���������� ��� ����������� �������: malloc, free, atof � ��.
#include <stdio.h>   // ���������� ��� �����/������: printf, fgets, fclose � ��.
#include <locale.h>  // ���������� ��� ��������� ������, ��������, ��� �������� �����.
#include <string.h>  // ���������� ��� ������ �� ��������: strchr, strcspn � ��.
#include <math.h>    // ���������� �������������� ��������
#include <graphics.h>  // ����������� ���������� ��� ���������� ��������

void sort(double x[], double y[], int n);
double average(double arr[], int n);
double dispersia(double arr[], int n, double average);
double sr_kv_otkl(double dispersia);
double corr(double x[], double y[], int n);	
void Function_1(double x[], int n, double X_1[]);
void Function_2(double x[], int n, double X_2[]);
double sum_kv_f( int n, double X_new[]);
double sum_f( int n, double X_new[]);
double d( int n, double X_new[]);
void parametr_a( double y[], double X_new[], int n, double *a0, double *a1);
void regress( int n, double a0, double a1, double X_new[], double results[]);
double ost_dispersia(double y[],double Y_new[], int n);
void findMinMax(double y[], double Y_1[], double Y_2[], int n, double *minY, double *maxY);
int scaleX(int value, int minX, int maxX, int graphWidth, int leftPadding);
int scaleY(int value, int minY, int maxY, int graphHeight, int topPadding);

int main()
{
	setlocale(LC_ALL, "Rus");
	
	char tab = ';'; 
    double y[100], x[100]; 
    char filename[15], str[100];
    char *rez1, *rez2; 
    int i, j, n;
	
	puts("���� ��� �����:");
	puts("	1) depth1.txt - ����� > 0.5");
	puts("	2) depth2.txt - ����� < 0.5");
    fgets(filename, sizeof(filename), stdin); 
    filename[strcspn(filename, "\n")] = '\0'; 

    FILE *file = fopen(filename, "r"); 
    // ������ �� ����?
    if (file == NULL) 
	{
        puts("���� �� ������");
    	getchar(); 
    	return 0;
    }
    
    FILE *output_file = fopen("results.txt", "w"); 
    // ������ �� ����?
    if (output_file == NULL) 
	{
	    puts("������ ��� �������� ����� results.txt!");
	    getchar(); 
	    return 0;
	}
    
    printf("\n=============== ���������� ����� ===============\n\n"); 
    
    while (fgets(str, sizeof(str), file) != NULL)
    {
    	printf("%s", str);
	}
    
    rewind(file);	// ��������� � ������ �����
    
    j = 0;
    while (fgets(str, sizeof(str), file) != NULL)
	{
        for (i = 0; str[i] != '\0'; i++) 
		{
	        if (str[i] == ',') 
			{
	            str[i] = '.';
	        }
    	}
    	
    	char buffer[256]; //������ ��� �������� ������
    	
		while (fgets(buffer, sizeof(buffer), file)) 
		{
			// ���� ������ �����������
	    	rez1 = strchr(buffer, tab);
	        if (rez1) 			
			{
				// ���� ������ �����������
	            rez2 = strchr(rez1 + 1, tab);	
	            if (rez2) 					
				{
					// �������� ������ ����� ������� �����������
	                *rez2 = '\0'; 
	                // ����������� ������ �������� � double � ���������� � x[j] (������������)
	                x[j] = atof(rez1 + 1); 
	                // ������ �������� � y[j] (��������������)
	                y[j] = atof(rez2 + 1); 
	                j++;
	            }
	        }
    	}
    }
    fclose(file); 
    
    n = j; 
    
    sort(x, y, n);  
    
    printf("\n\n============ ������ ============\n\n"); 
    fprintf(output_file,"\n\n============ ������ ============\n\n"); 
    printf(" ___________________________ \n");
    fprintf(output_file," ___________________________ \n");
    printf("|      X      |      Y      |\n");
    fprintf(output_file,"|      X      |      Y      |\n");
    printf("|-------------|-------------|\n");
    fprintf(output_file,"|-------------|-------------|\n");
    for (i = 0; i < n; i++) 
	{
        printf("|  %10.2f |  %10.2f |\n", x[i], y[i]);
        fprintf(output_file,"|  %10.2f |  %10.2f |\n", x[i], y[i]);
    }
    printf(" ___________________________ \n");
    fprintf(output_file," ___________________________ \n");
    
    float r = corr(x, y, n);
    
    if (fabs(r) > 0.5)
    {
    	double X_1[n], X_2[n];
        Function_1(x, n, X_1);
        Function_2(x, n, X_2);

        double a0_1, a1_1, a0_2, a1_2;
        parametr_a(y, X_1, n, &a0_1, &a1_1);
        parametr_a(y, X_2, n, &a0_2, &a1_2);

        double Y_1[n], Y_2[n];
        regress(n, a0_1, a1_1, X_1, Y_1);
        regress(n, a0_2, a1_2, X_2, Y_2);

        printf("\n====================== ��������� ������� ======================\n");
        fprintf(output_file,"\n====================== ��������� ������� ======================\n");
        printf("\n _____________________________________________________________ \n");
	    fprintf(output_file,"\n _____________________________________________________________ \n");
        printf("������� 1 (X = x*x*sqrt(x)):  a0 = %.8f; a1 = %.8f\n", a0_1, a1_1);
        fprintf(output_file,"������� 1 (X = sqrt(x[i])/log(x[i])): a0 = %.8f; a1 = %.8f\n", a0_1, a1_1);
        printf("������� 2 (X = log(sqrt(x))): a0 = %.8f; a1 = %.8f\n", a0_2, a1_2);
        fprintf(output_file,"������� 2 (X = x[i]*exp(1/sqrt(x[i]))): a0 = %.8f; a1 = %.8f", a0_2, a1_2);
		printf(" _____________________________________________________________ \n");
	    fprintf(output_file," _____________________________________________________________ \n");


        printf("\n==== �������� ��������� ====\n");
        fprintf(output_file,"\n==== �������� ��������� ====\n");
        printf("\n _________________________ \n");
	    fprintf(output_file,"\n _________________________ \n");
        printf("|  ������� 1 |  ������� 2 |\n");
        fprintf(output_file,"|  ������� 1 |  ������� 2 |\n");
        printf("|------------|------------|\n");
    	fprintf(output_file,"|------------|------------|\n");
        for (i = 0; i < n; i++) 
		{
            printf("| %10.2f | %10.2f |\n", Y_1[i], Y_2[i]);
            fprintf(output_file,"| %10.2f | %10.2f |\n", Y_1[i], Y_2[i]);
        }
        printf(" _________________________ \n");
	    fprintf(output_file," _________________________ \n");
        
        double average_x = average(x, n);
	    double average_y = average(y, n);
	    double dispersia_y = dispersia(y, n, average_y);
	
	    printf("\n=============== ���������� ===============\n");
	    fprintf(output_file,"\n=============== ���������� ===============\n");
	    printf("\n _______________________________________ \n");
	    fprintf(output_file,"\n _______________________________________ \n");
	    printf("| ��������                  | ��������   |\n");
	    fprintf(output_file,"| ��������                  | ��������   |\n");
	    printf("|---------------------------|------------|\n");
	    fprintf(output_file,"|---------------------------|------------|\n");
	    printf("| ������� X                 | %10.2f |\n", average_x);
	    fprintf(output_file,"| ������� X                 | %10.2f |\n", average_x);
	    printf("| ������� Y                 | %10.2f |\n", average_y);
	    fprintf(output_file,"| ������� Y                 | %10.2f |\n", average_y);
	    printf("| ��������� Y               | %10.2f |\n", dispersia_y);
	    fprintf(output_file,"| ��������� Y               | %10.2f |\n", dispersia_y);
	    printf("| ����������� ����������    | %10.2f |\n", r);
	    fprintf(output_file,"| ����������� ����������    | %10.2f |\n", r);
	    printf(" _______________________________________ \n");
	    fprintf(output_file," _______________________________________ \n");
	    
	    double ost_1 = ost_dispersia(y, Y_1, n);
        double ost_2 = ost_dispersia(y, Y_2, n);

        double F1 = dispersia_y / ost_1;
        double F2 = dispersia_y / ost_2;

        printf("\n================== ����������� ������ ==================\n");
        fprintf(output_file,"\n================== ����������� ������ ==================\n");
        printf("\n _______________________________________________________ \n");
	    fprintf(output_file,"\n _______________________________________________________ \n");
        printf("| ��������� | ���������� ��������� | ����������� ������ |\n");
        fprintf(output_file,"| ��������� | ���������� ��������� | ����������� ������ |\n");
        printf("|-----------|----------------------|--------------------|\n");
        fprintf(output_file,"|-----------|----------------------|--------------------|\n");
        printf("|     1     |      %.6f       |      %.6f      |\n", ost_1, F1);
        fprintf(output_file,"|     1     |      %.6f       |      %.6f      |\n", ost_1, F1);
        printf("|     2     |      %.6f       |      %.6f      |\n", ost_2, F2);
        fprintf(output_file,"|     2     |      %.6f       |      %.6f      |\n", ost_2, F2);
        printf(" _______________________________________________________ \n");
	    fprintf(output_file," _______________________________________________________ \n");
	    
	    // ����������� �������� ����, �������� ������� � ������������� ������������ ����
	    int width = GetSystemMetrics(SM_CXSCREEN) - 50;
	    int height = GetSystemMetrics(SM_CYSCREEN) - 100;
	    initwindow(width, height);
	    int graphHeight = height - 200;
	    int graphWidth = width - 50; 
	    
	    // ������������� ����� ���
	    setbkcolor(WHITE);
	    // �������� ����
	    cleardevice(); 
	    
	    // ����������� �������� �������
	    int leftPadding = graphWidth * 0.05;  // 5% �� ������ �������
	    int rightPadding = leftPadding;
	    int topPadding = graphHeight * 0.05;  // 5% �� ������ �������
	    int bottomPadding = topPadding;
	
	    // �������� �������
	    setcolor(RED);
	    circle(50, graphHeight + 50, 10);
	    outtextxy(65, graphHeight + 45, (char*)"- ��������� 1");
	
	    setcolor(GREEN);
	    circle(50, graphHeight + 80, 10);
	    outtextxy(65, graphHeight + 75, (char*)"- ��������� 2");
	
	    setcolor(LIGHTBLUE);
	    circle(50, graphHeight + 110, 10);
	    outtextxy(65, graphHeight + 105, (char*)"- ��������������");
	    	
	    // ��������� ����
	    setcolor(BLACK);
	    line(0, graphHeight, graphWidth + 50, graphHeight); // ��� X
	    line(50, graphHeight, 50, 0); // ��� Y
		
	    // ������� ����
	    outtextxy(graphWidth - 400, graphHeight + 10, (char*)"��������� ����������� �������� �������, ���. ���.");
	    outtextxy(60, 20, (char*)"��������������, %");
	    
	    double minX = x[0], maxX = x[n-1];
	    double minY, maxY;
	    findMinMax(y, Y_1, Y_2, n, &minY, &maxY);

		// ���������� �������� ���������
		setcolor(LIGHTBLUE);
		for (int i = 0; i < n; i++) 
		{
		    int x_coord = 50 + scaleX(x[i], minX, maxX, graphWidth, leftPadding);
		    int y_coord = scaleY(y[i], minY, maxY, graphHeight, topPadding);
		    int radius = 2;
		    circle(x_coord, y_coord, radius);
		}

		// ���������� �������� ���������
	    for (int i = 0; i < n - 1; i++)
	    {
	    	int xInit1 = 50 + scaleX(x[i], minX, maxX, graphWidth, leftPadding);
	    	int yInit1 = scaleY(Y_1[i], minY, maxY, graphHeight, topPadding);
	    	int xNext1 = 50 + scaleX(x[i + 1], minX, maxX, graphWidth, leftPadding);
	    	int yNext1 = scaleY(Y_1[i + 1], minY, maxY, graphHeight, topPadding);
	    	setcolor(RED);
	    	line(xInit1, yInit1, xNext1, yNext1);
	    	
	    	int xInit2 = 50 + scaleX(x[i], minX, maxX, graphWidth, leftPadding);
	        int yInit2 = scaleY(Y_2[i], minY, maxY, graphHeight, topPadding);
	        int xNext2 = 50 + scaleX(x[i + 1], minX, maxX, graphWidth, leftPadding);
	        int yNext2 = scaleY(Y_2[i + 1], minY, maxY, graphHeight, topPadding);
	        setcolor(GREEN);
	        line(xInit2, yInit2, xNext2, yNext2);
	    }

	    if(F1>F2)
	    {
	    	printf("\n������ ��������� ����� ��������� ����������� y = f(x).\n");
    		fprintf(output_file, "\n������ ��������� ����� ��������� ����������� y = f(x).\n");
		}
		else 
		if(F2>F1)
		{
			printf("\n������ ��������� ����� ��������� ����������� y = f(x).\n");
    		fprintf(output_file, "\n������ ��������� ����� ��������� ����������� y = f(x).\n");
		}
		else
		{
			printf("\n��� ��������� ��������� ������ ��������� ����������� y = f(x).\n");
    		fprintf(output_file, "\n��� ��������� ��������� ������ ��������� ����������� y = f(x).\n");
		}
	}
	else
	{
		printf("\n����������� ����������: %.2f", r,"\n");
        fprintf(output_file,"\n����������� ����������: %.2f", r,"\n");
		printf("\n�������������� ����������� �����������..\n");
        fprintf(output_file,"\n�������������� ����������� �����������..\n");
	}
    fclose(output_file);
    
    getch();
    closegraph();
    return 0;  
}

void findMinMax(double y[], double Y_1[], double Y_2[], int n, double *minY, double *maxY) 
{
    *minY = y[0];  
    *maxY = y[0];

    for (int i = 1; i < n; i++) 
    {
        if (y[i] < *minY) *minY = y[i];
        if (y[i] > *maxY) *maxY = y[i];
        if (Y_1[i] < *minY) *minY = Y_1[i];
        if (Y_1[i] > *maxY) *maxY = Y_1[i];
        if (Y_2[i] < *minY) *minY = Y_2[i];
        if (Y_2[i] > *maxY) *maxY = Y_2[i];
    }
}

// ������� ��� ��������������� �������� X
int scaleX(int value, int minX, int maxX, int graphWidth, int leftPadding) 
{
    return leftPadding + (value - minX) * (graphWidth - 2 * leftPadding) / (maxX - minX);
}

// ������� ��� ��������������� �������� Y 
int scaleY(int value, int minY, int maxY, int graphHeight, int topPadding) 
{
    return graphHeight - topPadding - (value - minY) * (graphHeight - 2 * topPadding) / (maxY - minY);
}

// C��������� ���������
void sort(double x[], double y[], int n) 
{
	int i, j;
    	for (i = 0; i < n - 1; i++) 
    	{
	 		for (j = 0; j < n - i - 1; j++) 
	        		{
	            		if (x[j] > x[j + 1]) 
	            		{
	                			// ���������� x
	                			double temp = x[j];
	                			x[j] = x[j + 1];
	                			x[j + 1] = temp;
	                
	                			// ����������� y �� x
	                			temp = y[j];
	                			y[j] = y[j + 1];
	                			y[j + 1] = temp;
	            		}
	        		}
    	}	
}

// ������� ��� ���������� �������� �������� �������
double average(double arr[], int n) 
{
    double SR, S = 0; 
	int i ;
	
    for (i=0; i < n; i++) 
    {
    	S += arr[i];
	}
    SR = S/n; 
    
    return (SR);
}

// ������� ��� ���������� ��������� �������
double dispersia(double arr[], int n, double average) 
{
    double dispersia = 0; 
    int i;
    for (i = 0; i < n; i++) 
    {
    	dispersia += ((arr[i] - average) * (arr[i] - average)); 
	}
    dispersia = dispersia/(n-1); 
	 
    return (dispersia);
}

// ������� ��� ���������� ��������������������� ����������
double sr_kv_otkl(double dispersia) 
{
    return (sqrt(dispersia)); 
}

// ������� ��� ���������� ������������ ������ ����������
double corr(double x[], double y[], int n) 
{
	int i ;
    double average_x = average(x, n); 
    double average_y = average(y, n); 

    double dispersia_x = dispersia(x, n, average_x);
    double dispersia_y = dispersia(y, n, average_y); 

    double sr_kv__otkl_x = sr_kv_otkl(dispersia_x); 
    double sr_kv_otkl_y = sr_kv_otkl(dispersia_y); 
	
	// ��������� ������� ����������
    double chislitel = 0;
    for (i=0; i < n; i++) 
    {
    	chislitel += (x[i] - average_x) * (y[i] - average_y);
	}
	// ����������� ������� ����������
    double znamen = (n-1) * sr_kv__otkl_x * sr_kv_otkl_y;

    return (chislitel / znamen); 
}

// �������, ��������� ������ �_1, ���������� ���������� ��� ���������� ����������� ���������� �� ������� 1
void Function_1(double x[], int n, double X_1[]) 
{
	int i;
	for (i=0; i<n;i++)
	{
		X_1[i] = sqrt(x[i])/log(x[i]);
	}
}

// �������, ��������� ������ �_2, ���������� ���������� ��� ���������� ����������� ���������� �� ������� 2
void Function_2(double x[], int n, double X_2[]) 
{
	int i;
	for (i=0; i<n;i++)
	{
		X_2[i] = x[i]*exp(1/sqrt(x[i]));
	}
}

// ������� ��� ���������� ����� ��������� ��������� �������
double sum_kv_f( int n, double X_new[]) 
{
	double sum_kv_f = 0;
	int i ;
    for ( i = 0; i < n; i++) 
    {
    	sum_kv_f += X_new[i] * X_new[i];
	}
    return (sum_kv_f);   
}

// ������� ��� ���������� ����� ���� ��������� �������
double sum_f( int n, double X_new[]) 
{
	double sum_f = 0;
	int i ;
    for ( i = 0; i < n; i++) 
    {
    	sum_f += X_new[i];
	}
    return (sum_f);   
}

// ������� ��� ���������� ���������� d
double d(int n, double X_new[]) 
{
	double d = 0; 
    double sum_F_kv = sum_kv_f(n, X_new);
    double sum_F = sum_f(n, X_new) ; 

	d = n * sum_F_kv - sum_F * sum_F;
    return (d);
}

// ������� ��� ���������� ���������� a0 � a1
void parametr_a(double y[], double X_new[], int n, double *a0, double *a1) 
{
    double sum_xy_F = 0, sum_y_F = 0, D;
	int i ;
    for (i=0; i < n; i++) 
    { 
		sum_y_F += y[i];
		sum_xy_F += y[i]*X_new[i];
	}
       
    double sum_F_kv = sum_kv_f( n, X_new);
    double sum_F = sum_f( n, X_new) ; 
    D = d( n, X_new);

    *a0 = (sum_F_kv * sum_y_F - sum_F * sum_xy_F) / D;
    *a1 = (n * sum_xy_F - sum_F * sum_y_F) / D;
}

// ������� ��� ���������� �������� ���������
void regress( int n, double a0, double a1, double X_new[], double results[]) 
{	
	int i ;
    for (i=0; i < n; i++) 
    {
    	results[i] = a0 + a1 * X_new[i];
	}
}

// ������� ��� ���������� ���������� ���������
double ost_dispersia(double y[],double Y_new[], int n) 
{
    double ost = 0;
    double run = 0;
    int i ;
    for (i = 0; i < n; i++)
    {
    	run += (y[i] - Y_new[i]) * (y[i] - Y_new[i]);
	}
    ost = run/(n-1); 
    return (ost);
}
