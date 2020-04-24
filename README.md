
# SoalShiftSISOP20_modul4_B01
### Soal Shift Modul 4
1. [Soal 1](#1-soal-1)
2. [Soal 2](#2-soal-2)
3. [Soal 3](#3-soal-3)
4. [Soal 4](#4-soal-4)
---
## Narasi
Di suatu perusahaan, terdapat pekerja baru yang super jenius, ia bernama jasir. Jasir baru bekerja selama seminggu di perusahan itu, dalam waktu seminggu tersebut ia selalu terhantui oleh ketidak amanan dan ketidak efisienan file system yang digunakan perusahaan tersebut. Sehingga ia merancang sebuah file system yang sangat aman dan efisien. Pada segi keamanan, Jasir telah menemukan 2 buah metode enkripsi file. Pada mode enkripsi pertama, nama file-file pada direktori terenkripsi akan dienkripsi menggunakan metode substitusi. Sedangkan pada metode enkripsi yang kedua, file-file pada direktori terenkripsi akan di-split menjadi file-file kecil. Sehingga orang-orang yang tidak menggunakan filesystem rancangannya akan kebingungan saat mengakses direktori terenkripsi tersebut. Untuk segi efisiensi, dikarenakan pada perusahaan tersebut sering dilaksanakan sinkronisasi antara 2 direktori, maka jasir telah merumuskan sebuah metode agar filesystem-nya mampu mengsingkronkan kedua direktori tersebut secara otomatis. Agar integritas filesystem tersebut lebih terjamin, maka setiap command yang dilakukan akan dicatat kedalam sebuah file log. (catatan: filesystem berfungsi normal layaknya linux pada umumnya) (catatan: mount source (root) filesystem adalah direktori /home/[user]/Documents)
## 1. Soal 1
Pada soal nomer 1 kita disuruh untuk encrypt nama file dan folder pada fuse dengan Caesar Cipher. berikut fungsinya : 
```c
void enkrip (char *kata)
{
	int i,j;
	if(!strcmp(kata,".") || !strcmp(kata,"..")) return;
	for(i=0;i<(strlen(kata));i++)
	{
		printf("%c",kata[i]);
		if(kata[i]!='/')
		{
			for(j=0;j<(strlen(urutan));j++)
			{
				if(urutan[j]==kata[i])
				{
					int aa = (j + 10) % 87;
					kata[i] = urutan[aa];
					break;
				}
			}
		}	
	}
}

void dekrip (char *kata)
{
	int i,j;
	if(!strcmp(kata,".") || !strcmp(kata,"..")) return;
	for(i=0;i<(strlen(kata));i++)
	{
		printf("%c",kata[i]);
		if(kata[i]!='/')
		{
			for(j=0;j<(strlen(urutan));j++)
			{
				if(urutan[j]==kata[i])
				{
					int aa = (j + 87-10) % 87;
					kata[i] = urutan[aa];
					break;
				}
			}
		}	
	}
}
```
## 2. Soal 2
Belum sempat mengerjakan karena kekurangan waktu
## 3. Soal 3
Belum sempat mengerjakan karena kekurangan waktu dan kurang mengerti maksud soal
## 4. Soal 4
Pada soal nomer 4 kita disuruh membuat log dengan 2 jenis status yakni INFO dan WARNING. Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink. Berikut fungsinya :
```c
void writeLOG(char *cmd, char* path, int flag)
{
    char* status;
    char str[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
	if(flag == 1){
		status = "INFO";
	}
	else status = "WARNING";
    sprintf(str, "[%s]::%02d%02d%02d-%02d:%02d:%02d::%s::%s", status, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, cmd, path);
    FILE *out = fopen("/home/sin/fs.log", "a");  
    fprintf(out, "%s\n", str);  
    fclose(out);  
    return 0;
    
}
```
