#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>		//для распознования русского языка

#define MAX_LEKARSTVA 30	//замена в коде MAX_LEKARSTVA на значение 30 перед компиляцией

typedef struct 				//структура
{
    char nazvanie[50];		//название лекарства
    char profil[50];		//профиль применения
    char proizvoditel[50];	//производитель
    char pokazaniya[100];	//показания к применению
    char dozirovki[50];		//дозировка
    float stoimost;			//стоимость
} Spravochnik;				//чтобы в дальнейшем объявлять переменные без необходимости писать struct Spravochnik

void addRecord(Spravochnik R[], int *base);	//R[] - массив записей
void printTable(Spravochnik R[], int base);	//base - указатель на колличество зписей
void deleteRecord(Spravochnik R[], int *base);
void deleteLek(Spravochnik R[], int *base);
void increaseCost(Spravochnik R[], int base);
void searchByCost(Spravochnik R[], int base);
void loadFromFile(char *filename, Spravochnik *R, int *base);

int main() 
{
    SetConsoleCP(1251);			//чтобы можно было корректно вводить символы кириллицы
    SetConsoleOutputCP(1251);	//чтобы русские символы отображались правильно

    Spravochnik R[MAX_LEKARSTVA]; 	//R[] - массив записей
    int base = 0; 					//base - указатель на колличество зписей
    int k;

    while (k!=0) 
	{
        printf("\nМеню:\n");
        printf("1. Добавить запись\n");
        printf("2. Вывести все записи\n");
        printf("3. Удалить запись по номеру\n");
        printf("4. Удалить лекарства по профилю\n");
        printf("5. Увеличить стоимость лекарства на 10%%\n");
        printf("6. Найти лекарства по стоимости\n");
        printf("7. Загрузить с файла\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &k);
        
        getchar();

        switch (k) 
		{
            case 1: addRecord(R, &base); break;		//добавление записей
            case 2: printTable(R, base); break;		//вывод на экран всех записей в виде таблицы
            case 3: deleteRecord(R, &base); break;	//удаление записей по номеру
            case 4: deleteLek(R, &base); break;		//удаление лекарств по профилю
            case 5: increaseCost(R, base); break;	//увеличение стоимости лекарства на 10%
            case 6: searchByCost(R, base); break;	//найти лекарство по стоимости
            case 7: loadFromFile("depth1.bin", R, &base); break;		//вывод из текстового файла
            case 0: printf("Выход из программы...\n"); return 0;		//выход из программы
            default: printf("Некорректный выбор. Попробуйте снова.\n");	//если нет такого номера в списке
        }
    }
}

void loadFromFile(char *filename, Spravochnik *R, int *base) 
{
    FILE *file = fopen(filename, "wb");	//открытие файла в режиме записывания
    if (file == NULL) 					//проверка успешно ли открыт файл
	{
        printf("Ошибка открытия файла");
        return;
    }
    
    Spravochnik s1 = {"Парацетамол", "Обезболивающее", "ФармИнвест", "Головная боль", "500", 2.10f};
	Spravochnik s2 = {"Амоксициллин", "Антибиотик", "МедФарм", "Инфекции", "250", 3.40f};
    if (file != NULL) 				
	{
       	fwrite(&s1, sizeof(Spravochnik), 1, file);
		fwrite(&s2, sizeof(Spravochnik), 1, file);
    }
	fclose(file);
	
	file = fopen(filename, "rb");	//открытие файла в режиме чтения
	if (file == NULL) 				//проверка успешно ли открыт файл
	{
	    printf("Ошибка открытия файла");
	    return;
	}

	//последовательное чтение структур Spravochnik из бинарного файла и добавление 
	//их в массив R
	//программа продолжает читать файл до тех пор, пока чтение структур успешно(==1)
	while (fread(&R[*base], sizeof(Spravochnik), 1, file) == 1) 
	{
	    (*base)++;
	}			  
    fclose(file);
}

void addRecord(Spravochnik R[], int *base) 			//добавление записей
{
    if (*base >= MAX_LEKARSTVA) 					//как предел для записей
	{
        printf("Превышено максимальное количество записей!\n");
        return;
    }
    
    printf("Введите название лекарства: ");			//ввод значений для записи
    gets(R[*base].nazvanie);
    printf("Введите профиль применения: ");
    gets(R[*base].profil);
    printf("Введите производителя: ");
    gets(R[*base].proizvoditel);
    printf("Введите показания к применению: ");
    gets(R[*base].pokazaniya);
    printf("Введите дозировку: ");
    gets(R[*base].dozirovki);
    printf("Введите стоимость: ");
    scanf("%f", &R[*base].stoimost);
    
    getchar();
    
    (*base)++;										//увеличение номера на 1

	printf("Запись успешно добавлена.\n");
}

void printTable(Spravochnik R[], int base) 			//вывод на экран всех записей в виде таблицы
{
    if (base == 0) 									//если нет записей
	{
        printf("Нет записей для отображения.\n");
        return;
    }
    
    printf(" __________________________________________________________________________________________________ \n");
    printf("| Название      | Профиль        | Производитель | Показания           | Дозировки     | Стоимость |\n");
    printf("|_______________|________________|_______________|_____________________|_______________|___________|\n");
    
    for (int i = 0; i < base; i++) 
	{
        printf("| %-13s | %-14s | %-13s | %-19s | %-13s | %-9.2f |\n",
            	R[i].nazvanie, R[i].profil, R[i].proizvoditel, R[i].pokazaniya, R[i].dozirovki, R[i].stoimost);
    }
    
    printf("|_______________|________________|_______________|_____________________|_______________|___________|\n");
}

int searchByNumb(int base)	//найти номер
{
	int numb;
	printf("Введите номер записи: ");
    scanf("%d", &numb);
    
    if (numb < 1 || numb > base) //если нет такой записи
	{
  		printf("Ошибка: Неверный номер записи.\n");
  		return -1;
    }
    return numb;
}

void deleteRecord(Spravochnik R[], int *base) 	//удаление записей по номеру
{
	int i;
	int numb;
	numb = searchByNumb(*base);
	
	if (numb == -1) 
	{
        return; //если ошибка, прекращаем выполнение
    }
    
	if (*base > 0) 
	{
        for (i = numb - 1; i < *base - 1; i++) 		//само удаление записи
		{
            R[i] = R[i + 1];						//замена одной записи на следующую
        }
        
        (*base)--;									//уменьшение номера на 1
        
        printf("Запись успешно удалена.\n");
    } 
	else 											//если нет записей
	{
        printf("Нет записей для удаления.\n");
    }
}

void deleteLek(Spravochnik R[], int *base) 			//удаление лекарств по профилю
{
    if (*base == 0) 								//если нет записей
	{
        printf("Нет записей для удаления.\n");
        return;
    }

    char profil[50];
    printf("Введите профиль применения для удаления записей: ");
    gets(profil);

    int i, j;
	i=0;
    while (i < *base) 								//проверка всех записей
	{
        if (strcmp(R[i].profil, profil) == 0) 		//сравнение двух строк
		{
            for (j = i; j < *base - 1; j++) 		//удаление записи путем замены ее на след
			{
                R[j] = R[j + 1];
            }
            
            (*base)--;								//уменьшение номера на 1
        } 
		else 
		{
            i++;
        }
    }
    
    printf("Записи для лекарств с профилем '%s' успешно удалены.\n", profil);
}

void increaseCost(Spravochnik R[], int base) 		//увеличение стоимости лекарства на 10%
{
    if (base == 0) 									//если нет записей
	{
        printf("Нет записей для изменения стоимости.\n");
        return;
    }

    char nazvanie[50];
    printf("Введите название лекарства для увеличения стоимости на 10%%: ");
    gets(nazvanie);

    int found, i;
    found = 0;
    for (i = 0; i < base; i++) 
	{
        if (strcmp(R[i].nazvanie, nazvanie) == 0) 	//сравнение двух строк
		{
            R[i].stoimost *= 1.1;					//увеличение стоимости на 10%
            printf("Новая стоимость лекарства '%s': %.2f\n", R[i].nazvanie, R[i].stoimost);
            found = 1;
        }
    }

    if (!found) 									//если нет такого лекарства
	{
        printf("Лекарство '%s' не найдено.\n", nazvanie);
    }
}

void searchByCost(Spravochnik R[], int base) 		//найти лекарство по стоимости
{
    if (base == 0) 									//если нет записей
	{
        printf("Нет записей для поиска.\n");
        return;
    }

    float min, max;
    printf("Введите диапазон стоимости (мин макс): ");	//ввод диапозона стоимости
    scanf("%f %f", &min, &max);
    getchar();

    int found, i;
    found = 0;
    printf(" ___________________________________________________________________________________________ \n");
    printf("| Название      | Профиль     | Производитель | Показания       | Дозировки     | Стоимость |\n");
    printf("|_______________|_____________|_______________|_________________|_______________|___________|\n");

    for (i = 0; i < base; i++) 
	{
        if (R[i].stoimost >= min && R[i].stoimost <= max) 
		{
            printf("| %-13s | %-14s | %-13s | %-15s | %-13s | %-9.2f |\n",
            	R[i].nazvanie, R[i].profil, R[i].proizvoditel, R[i].pokazaniya, R[i].dozirovki, R[i].stoimost);
            found = 1;
        }
    }

    if (!found) 									//если нет такого диапозона
	{
        printf("Нет лекарств в указанном диапазоне.\n");
    }

    printf("|_______________|_____________|_______________|_________________|_______________|___________|\n");
}

