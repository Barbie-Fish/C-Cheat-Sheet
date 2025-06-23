#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>		//��� ������������� �������� �����

#define MAX_LEKARSTVA 30	//������ � ���� MAX_LEKARSTVA �� �������� 30 ����� �����������

typedef struct 				//���������
{
    char nazvanie[50];		//�������� ���������
    char profil[50];		//������� ����������
    char proizvoditel[50];	//�������������
    char pokazaniya[100];	//��������� � ����������
    char dozirovki[50];		//���������
    float stoimost;			//���������
} Spravochnik;				//����� � ���������� ��������� ���������� ��� ������������� ������ struct Spravochnik

void addRecord(Spravochnik R[], int *base);	//R[] - ������ �������
void printTable(Spravochnik R[], int base);	//base - ��������� �� ����������� ������
void deleteRecord(Spravochnik R[], int *base);
void deleteLek(Spravochnik R[], int *base);
void increaseCost(Spravochnik R[], int base);
void searchByCost(Spravochnik R[], int base);
void loadFromFile(char *filename, Spravochnik *R, int *base);

int main() 
{
    SetConsoleCP(1251);			//����� ����� ���� ��������� ������� ������� ���������
    SetConsoleOutputCP(1251);	//����� ������� ������� ������������ ���������

    Spravochnik R[MAX_LEKARSTVA]; 	//R[] - ������ �������
    int base = 0; 					//base - ��������� �� ����������� ������
    int k;

    while (k!=0) 
	{
        printf("\n����:\n");
        printf("1. �������� ������\n");
        printf("2. ������� ��� ������\n");
        printf("3. ������� ������ �� ������\n");
        printf("4. ������� ��������� �� �������\n");
        printf("5. ��������� ��������� ��������� �� 10%%\n");
        printf("6. ����� ��������� �� ���������\n");
        printf("7. ��������� � �����\n");
        printf("0. �����\n");
        printf("�������� ��������: ");
        scanf("%d", &k);
        
        getchar();

        switch (k) 
		{
            case 1: addRecord(R, &base); break;		//���������� �������
            case 2: printTable(R, base); break;		//����� �� ����� ���� ������� � ���� �������
            case 3: deleteRecord(R, &base); break;	//�������� ������� �� ������
            case 4: deleteLek(R, &base); break;		//�������� �������� �� �������
            case 5: increaseCost(R, base); break;	//���������� ��������� ��������� �� 10%
            case 6: searchByCost(R, base); break;	//����� ��������� �� ���������
            case 7: loadFromFile("depth1.bin", R, &base); break;		//����� �� ���������� �����
            case 0: printf("����� �� ���������...\n"); return 0;		//����� �� ���������
            default: printf("������������ �����. ���������� �����.\n");	//���� ��� ������ ������ � ������
        }
    }
}

void loadFromFile(char *filename, Spravochnik *R, int *base) 
{
    FILE *file = fopen(filename, "wb");	//�������� ����� � ������ �����������
    if (file == NULL) 					//�������� ������� �� ������ ����
	{
        printf("������ �������� �����");
        return;
    }
    
    Spravochnik s1 = {"�����������", "��������������", "����������", "�������� ����", "500", 2.10f};
	Spravochnik s2 = {"������������", "����������", "�������", "��������", "250", 3.40f};
    if (file != NULL) 				
	{
       	fwrite(&s1, sizeof(Spravochnik), 1, file);
		fwrite(&s2, sizeof(Spravochnik), 1, file);
    }
	fclose(file);
	
	file = fopen(filename, "rb");	//�������� ����� � ������ ������
	if (file == NULL) 				//�������� ������� �� ������ ����
	{
	    printf("������ �������� �����");
	    return;
	}

	//���������������� ������ �������� Spravochnik �� ��������� ����� � ���������� 
	//�� � ������ R
	//��������� ���������� ������ ���� �� ��� ���, ���� ������ �������� �������(==1)
	while (fread(&R[*base], sizeof(Spravochnik), 1, file) == 1) 
	{
	    (*base)++;
	}			  
    fclose(file);
}

void addRecord(Spravochnik R[], int *base) 			//���������� �������
{
    if (*base >= MAX_LEKARSTVA) 					//��� ������ ��� �������
	{
        printf("��������� ������������ ���������� �������!\n");
        return;
    }
    
    printf("������� �������� ���������: ");			//���� �������� ��� ������
    gets(R[*base].nazvanie);
    printf("������� ������� ����������: ");
    gets(R[*base].profil);
    printf("������� �������������: ");
    gets(R[*base].proizvoditel);
    printf("������� ��������� � ����������: ");
    gets(R[*base].pokazaniya);
    printf("������� ���������: ");
    gets(R[*base].dozirovki);
    printf("������� ���������: ");
    scanf("%f", &R[*base].stoimost);
    
    getchar();
    
    (*base)++;										//���������� ������ �� 1

	printf("������ ������� ���������.\n");
}

void printTable(Spravochnik R[], int base) 			//����� �� ����� ���� ������� � ���� �������
{
    if (base == 0) 									//���� ��� �������
	{
        printf("��� ������� ��� �����������.\n");
        return;
    }
    
    printf(" __________________________________________________________________________________________________ \n");
    printf("| ��������      | �������        | ������������� | ���������           | ���������     | ��������� |\n");
    printf("|_______________|________________|_______________|_____________________|_______________|___________|\n");
    
    for (int i = 0; i < base; i++) 
	{
        printf("| %-13s | %-14s | %-13s | %-19s | %-13s | %-9.2f |\n",
            	R[i].nazvanie, R[i].profil, R[i].proizvoditel, R[i].pokazaniya, R[i].dozirovki, R[i].stoimost);
    }
    
    printf("|_______________|________________|_______________|_____________________|_______________|___________|\n");
}

int searchByNumb(int base)	//����� �����
{
	int numb;
	printf("������� ����� ������: ");
    scanf("%d", &numb);
    
    if (numb < 1 || numb > base) //���� ��� ����� ������
	{
  		printf("������: �������� ����� ������.\n");
  		return -1;
    }
    return numb;
}

void deleteRecord(Spravochnik R[], int *base) 	//�������� ������� �� ������
{
	int i;
	int numb;
	numb = searchByNumb(*base);
	
	if (numb == -1) 
	{
        return; //���� ������, ���������� ����������
    }
    
	if (*base > 0) 
	{
        for (i = numb - 1; i < *base - 1; i++) 		//���� �������� ������
		{
            R[i] = R[i + 1];						//������ ����� ������ �� ���������
        }
        
        (*base)--;									//���������� ������ �� 1
        
        printf("������ ������� �������.\n");
    } 
	else 											//���� ��� �������
	{
        printf("��� ������� ��� ��������.\n");
    }
}

void deleteLek(Spravochnik R[], int *base) 			//�������� �������� �� �������
{
    if (*base == 0) 								//���� ��� �������
	{
        printf("��� ������� ��� ��������.\n");
        return;
    }

    char profil[50];
    printf("������� ������� ���������� ��� �������� �������: ");
    gets(profil);

    int i, j;
	i=0;
    while (i < *base) 								//�������� ���� �������
	{
        if (strcmp(R[i].profil, profil) == 0) 		//��������� ���� �����
		{
            for (j = i; j < *base - 1; j++) 		//�������� ������ ����� ������ �� �� ����
			{
                R[j] = R[j + 1];
            }
            
            (*base)--;								//���������� ������ �� 1
        } 
		else 
		{
            i++;
        }
    }
    
    printf("������ ��� �������� � �������� '%s' ������� �������.\n", profil);
}

void increaseCost(Spravochnik R[], int base) 		//���������� ��������� ��������� �� 10%
{
    if (base == 0) 									//���� ��� �������
	{
        printf("��� ������� ��� ��������� ���������.\n");
        return;
    }

    char nazvanie[50];
    printf("������� �������� ��������� ��� ���������� ��������� �� 10%%: ");
    gets(nazvanie);

    int found, i;
    found = 0;
    for (i = 0; i < base; i++) 
	{
        if (strcmp(R[i].nazvanie, nazvanie) == 0) 	//��������� ���� �����
		{
            R[i].stoimost *= 1.1;					//���������� ��������� �� 10%
            printf("����� ��������� ��������� '%s': %.2f\n", R[i].nazvanie, R[i].stoimost);
            found = 1;
        }
    }

    if (!found) 									//���� ��� ������ ���������
	{
        printf("��������� '%s' �� �������.\n", nazvanie);
    }
}

void searchByCost(Spravochnik R[], int base) 		//����� ��������� �� ���������
{
    if (base == 0) 									//���� ��� �������
	{
        printf("��� ������� ��� ������.\n");
        return;
    }

    float min, max;
    printf("������� �������� ��������� (��� ����): ");	//���� ��������� ���������
    scanf("%f %f", &min, &max);
    getchar();

    int found, i;
    found = 0;
    printf(" ___________________________________________________________________________________________ \n");
    printf("| ��������      | �������     | ������������� | ���������       | ���������     | ��������� |\n");
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

    if (!found) 									//���� ��� ������ ���������
	{
        printf("��� �������� � ��������� ���������.\n");
    }

    printf("|_______________|_____________|_______________|_________________|_______________|___________|\n");
}

