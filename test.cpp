#include <stdlib.h>  // Библиотека для стандартных функций: malloc, free, atof и др.
#include <stdio.h>   // Библиотека для ввода/вывода: printf, fgets, fclose и др.
#include <locale.h>  // Библиотека для установки локали, например, для русского языка.
#include <string.h>  // Библиотека для работы со строками: strchr, strcspn и др.
#include <math.h>    // Библиотека математических операций
#include <graphics.h>  // Подключение библиотеки для построения графиков

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
	
	puts("Есть два файла:");
	puts("	1) depth1.txt - коэфф > 0.5");
	puts("	2) depth2.txt - коэфф < 0.5");
    fgets(filename, sizeof(filename), stdin); 
    filename[strcspn(filename, "\n")] = '\0'; 

    FILE *file = fopen(filename, "r"); 
    // Открыт ли файл?
    if (file == NULL) 
	{
        puts("ФАЙЛ НЕ ОТКРЫТ");
    	getchar(); 
    	return 0;
    }
    
    FILE *output_file = fopen("results.txt", "w"); 
    // Открыт ли файл?
    if (output_file == NULL) 
	{
	    puts("Ошибка при открытии файла results.txt!");
	    getchar(); 
	    return 0;
	}
    
    printf("\n=============== СОДЕРЖИМОЕ ФАЙЛА ===============\n\n"); 
    
    while (fgets(str, sizeof(str), file) != NULL)
    {
    	printf("%s", str);
	}
    
    rewind(file);	// Вернуться к началу строк
    
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
    	
    	char buffer[256]; //буффер для хванения строки
    	
		while (fgets(buffer, sizeof(buffer), file)) 
		{
			// Ищет первый разделитель
	    	rez1 = strchr(buffer, tab);
	        if (rez1) 			
			{
				// Ищем второй разделитель
	            rez2 = strchr(rez1 + 1, tab);	
	            if (rez2) 					
				{
					// Обрезаем строку после второго разделителя
	                *rez2 = '\0'; 
	                // Преобразует второе значение в double и записывает в x[j] (армортизация)
	                x[j] = atof(rez1 + 1); 
	                // Третье значение в y[j] (рентабольность)
	                y[j] = atof(rez2 + 1); 
	                j++;
	            }
	        }
    	}
    }
    fclose(file); 
    
    n = j; 
    
    sort(x, y, n);  
    
    printf("\n\n============ ДАННЫЕ ============\n\n"); 
    fprintf(output_file,"\n\n============ ДАННЫЕ ============\n\n"); 
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

        printf("\n====================== ПАРАМЕТРЫ ФУНКЦИЙ ======================\n");
        fprintf(output_file,"\n====================== ПАРАМЕТРЫ ФУНКЦИЙ ======================\n");
        printf("\n _____________________________________________________________ \n");
	    fprintf(output_file,"\n _____________________________________________________________ \n");
        printf("Функция 1 (X = x*x*sqrt(x)):  a0 = %.8f; a1 = %.8f\n", a0_1, a1_1);
        fprintf(output_file,"Функция 1 (X = sqrt(x[i])/log(x[i])): a0 = %.8f; a1 = %.8f\n", a0_1, a1_1);
        printf("Функция 2 (X = log(sqrt(x))): a0 = %.8f; a1 = %.8f\n", a0_2, a1_2);
        fprintf(output_file,"Функция 2 (X = x[i]*exp(1/sqrt(x[i]))): a0 = %.8f; a1 = %.8f", a0_2, a1_2);
		printf(" _____________________________________________________________ \n");
	    fprintf(output_file," _____________________________________________________________ \n");


        printf("\n==== ЗНАЧЕНИЯ РЕГРЕССИЙ ====\n");
        fprintf(output_file,"\n==== ЗНАЧЕНИЯ РЕГРЕССИЙ ====\n");
        printf("\n _________________________ \n");
	    fprintf(output_file,"\n _________________________ \n");
        printf("|  Функция 1 |  Функция 2 |\n");
        fprintf(output_file,"|  Функция 1 |  Функция 2 |\n");
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
	
	    printf("\n=============== СТАТИСТИКА ===============\n");
	    fprintf(output_file,"\n=============== СТАТИСТИКА ===============\n");
	    printf("\n _______________________________________ \n");
	    fprintf(output_file,"\n _______________________________________ \n");
	    printf("| Параметр                  | Значение   |\n");
	    fprintf(output_file,"| Параметр                  | Значение   |\n");
	    printf("|---------------------------|------------|\n");
	    fprintf(output_file,"|---------------------------|------------|\n");
	    printf("| Среднее X                 | %10.2f |\n", average_x);
	    fprintf(output_file,"| Среднее X                 | %10.2f |\n", average_x);
	    printf("| Среднее Y                 | %10.2f |\n", average_y);
	    fprintf(output_file,"| Среднее Y                 | %10.2f |\n", average_y);
	    printf("| Дисперсия Y               | %10.2f |\n", dispersia_y);
	    fprintf(output_file,"| Дисперсия Y               | %10.2f |\n", dispersia_y);
	    printf("| Коэффициент корреляции    | %10.2f |\n", r);
	    fprintf(output_file,"| Коэффициент корреляции    | %10.2f |\n", r);
	    printf(" _______________________________________ \n");
	    fprintf(output_file," _______________________________________ \n");
	    
	    double ost_1 = ost_dispersia(y, Y_1, n);
        double ost_2 = ost_dispersia(y, Y_2, n);

        double F1 = dispersia_y / ost_1;
        double F2 = dispersia_y / ost_2;

        printf("\n================== КОЭФФИЦИЕНТ ФИШЕРА ==================\n");
        fprintf(output_file,"\n================== КОЭФФИЦИЕНТ ФИШЕРА ==================\n");
        printf("\n _______________________________________________________ \n");
	    fprintf(output_file,"\n _______________________________________________________ \n");
        printf("| Регрессия | Остаточная дисперсия | Коэффициент Фишера |\n");
        fprintf(output_file,"| Регрессия | Остаточная дисперсия | Коэффициент Фишера |\n");
        printf("|-----------|----------------------|--------------------|\n");
        fprintf(output_file,"|-----------|----------------------|--------------------|\n");
        printf("|     1     |      %.6f       |      %.6f      |\n", ost_1, F1);
        fprintf(output_file,"|     1     |      %.6f       |      %.6f      |\n", ost_1, F1);
        printf("|     2     |      %.6f       |      %.6f      |\n", ost_2, F2);
        fprintf(output_file,"|     2     |      %.6f       |      %.6f      |\n", ost_2, F2);
        printf(" _______________________________________________________ \n");
	    fprintf(output_file," _______________________________________________________ \n");
	    
	    // Определение размеров окна, пределов графика и инициализация графического окна
	    int width = GetSystemMetrics(SM_CXSCREEN) - 50;
	    int height = GetSystemMetrics(SM_CYSCREEN) - 100;
	    initwindow(width, height);
	    int graphHeight = height - 200;
	    int graphWidth = width - 50; 
	    
	    // Устанавливаем белый фон
	    setbkcolor(WHITE);
	    // Очистить окно
	    cleardevice(); 
	    
	    // Определение отступов графика
	    int leftPadding = graphWidth * 0.05;  // 5% от ширины графика
	    int rightPadding = leftPadding;
	    int topPadding = graphHeight * 0.05;  // 5% от высоты графика
	    int bottomPadding = topPadding;
	
	    // Создание легенды
	    setcolor(RED);
	    circle(50, graphHeight + 50, 10);
	    outtextxy(65, graphHeight + 45, (char*)"- Регрессия 1");
	
	    setcolor(GREEN);
	    circle(50, graphHeight + 80, 10);
	    outtextxy(65, graphHeight + 75, (char*)"- Регрессия 2");
	
	    setcolor(LIGHTBLUE);
	    circle(50, graphHeight + 110, 10);
	    outtextxy(65, graphHeight + 105, (char*)"- Рентабельность");
	    	
	    // Отрисовка осей
	    setcolor(BLACK);
	    line(0, graphHeight, graphWidth + 50, graphHeight); // Ось X
	    line(50, graphHeight, 50, 0); // Ось Y
		
	    // Подписи осей
	    outtextxy(graphWidth - 400, graphHeight + 10, (char*)"Стоимость амортизации основных средств, млн. руб.");
	    outtextxy(60, 20, (char*)"Рентабельность, %");
	    
	    double minX = x[0], maxX = x[n-1];
	    double minY, maxY;
	    findMinMax(y, Y_1, Y_2, n, &minY, &maxY);

		// Построение точечной диаграммы
		setcolor(LIGHTBLUE);
		for (int i = 0; i < n; i++) 
		{
		    int x_coord = 50 + scaleX(x[i], minX, maxX, graphWidth, leftPadding);
		    int y_coord = scaleY(y[i], minY, maxY, graphHeight, topPadding);
		    int radius = 2;
		    circle(x_coord, y_coord, radius);
		}

		// Построение графиков регрессий
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
	    	printf("\nПервая регрессия лучше описывает зависимость y = f(x).\n");
    		fprintf(output_file, "\nПервая регрессия лучше описывает зависимость y = f(x).\n");
		}
		else 
		if(F2>F1)
		{
			printf("\nВторая регрессия лучше описывает зависимость y = f(x).\n");
    		fprintf(output_file, "\nВторая регрессия лучше описывает зависимость y = f(x).\n");
		}
		else
		{
			printf("\nОбе регрессии одинаково хорошо описывают зависимость y = f(x).\n");
    		fprintf(output_file, "\nОбе регрессии одинаково хорошо описывают зависимость y = f(x).\n");
		}
	}
	else
	{
		printf("\nКоэффициент корреляции: %.2f", r,"\n");
        fprintf(output_file,"\nКоэффициент корреляции: %.2f", r,"\n");
		printf("\nФункциональная зависимость отсутствует..\n");
        fprintf(output_file,"\nФункциональная зависимость отсутствует..\n");
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

// Функция для масштабирования значений X
int scaleX(int value, int minX, int maxX, int graphWidth, int leftPadding) 
{
    return leftPadding + (value - minX) * (graphWidth - 2 * leftPadding) / (maxX - minX);
}

// Функция для масштабирования значений Y 
int scaleY(int value, int minY, int maxY, int graphHeight, int topPadding) 
{
    return graphHeight - topPadding - (value - minY) * (graphHeight - 2 * topPadding) / (maxY - minY);
}

// Cортировка пузырьком
void sort(double x[], double y[], int n) 
{
	int i, j;
    	for (i = 0; i < n - 1; i++) 
    	{
	 		for (j = 0; j < n - i - 1; j++) 
	        		{
	            		if (x[j] > x[j + 1]) 
	            		{
	                			// Сортировка x
	                			double temp = x[j];
	                			x[j] = x[j + 1];
	                			x[j + 1] = temp;
	                
	                			// Перемещение y по x
	                			temp = y[j];
	                			y[j] = y[j + 1];
	                			y[j + 1] = temp;
	            		}
	        		}
    	}	
}

// Функция для вычисления среднего значения массива
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

// Функция для вычисления дисперсии массива
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

// Функция для вычисления среднеквадратического отклонения
double sr_kv_otkl(double dispersia) 
{
    return (sqrt(dispersia)); 
}

// Функция для вычисления коэффициента парной корреляции
double corr(double x[], double y[], int n) 
{
	int i ;
    double average_x = average(x, n); 
    double average_y = average(y, n); 

    double dispersia_x = dispersia(x, n, average_x);
    double dispersia_y = dispersia(y, n, average_y); 

    double sr_kv__otkl_x = sr_kv_otkl(dispersia_x); 
    double sr_kv_otkl_y = sr_kv_otkl(dispersia_y); 
	
	// Числитель формулы корреляции
    double chislitel = 0;
    for (i=0; i < n; i++) 
    {
    	chislitel += (x[i] - average_x) * (y[i] - average_y);
	}
	// Знаменатель формулы корреляции
    double znamen = (n-1) * sr_kv__otkl_x * sr_kv_otkl_y;

    return (chislitel / znamen); 
}

// Функция, создающая массив Х_1, содержащий переменные для вычисления необходимых параметров по функции 1
void Function_1(double x[], int n, double X_1[]) 
{
	int i;
	for (i=0; i<n;i++)
	{
		X_1[i] = sqrt(x[i])/log(x[i]);
	}
}

// Функция, создающая массив Х_2, содержащий переменные для вычисления необходимых параметров по функции 2
void Function_2(double x[], int n, double X_2[]) 
{
	int i;
	for (i=0; i<n;i++)
	{
		X_2[i] = x[i]*exp(1/sqrt(x[i]));
	}
}

// Функция для вычисления суммы квадратов элементов массива
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

// Функция для вычисления суммы всех элементов массива
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

// Функция для вычисления переменной d
double d(int n, double X_new[]) 
{
	double d = 0; 
    double sum_F_kv = sum_kv_f(n, X_new);
    double sum_F = sum_f(n, X_new) ; 

	d = n * sum_F_kv - sum_F * sum_F;
    return (d);
}

// Функция для вычисления параметров a0 и a1
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

// Функция для вычисления значений регрессии
void regress( int n, double a0, double a1, double X_new[], double results[]) 
{	
	int i ;
    for (i=0; i < n; i++) 
    {
    	results[i] = a0 + a1 * X_new[i];
	}
}

// Функция для вычисления остаточной дисперсии
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
