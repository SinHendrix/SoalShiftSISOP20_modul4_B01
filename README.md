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
Pada soal nomer 1 kita disuruh untuk encrypt isi folder yang berawalan dengan "encv1_". Seluruh isi dari folder tersebut akan terenkripsi dengan metode Caesar Cipher dengan key sebagai berikut :
```9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO```. berikut fungsinya : 
```c
void encrypt(char *x) {
	printf("string enkrip = %s\n", x);
	//string enkrip = FOTO_PENTING
	//string enkrip = encv2_lol

	int xlength = strlen(x), xbegin = 0;
	int i;
	for(i = strlen(x); i >= 0; i--) {
		if(x[i] == '/') {	//cari indeks mulai nama file
			// printf("i break = %d", i);
			break;
		}

		if(x[i] == '.') {	
			xlength = i - 1; // cari panjang filename
		}
	}
	// /encv1_rhs/FOTO_PENTING
	for (int i = 1; i < xlength; i++) { 	//ngambil yang paling kanan
		if(x[i] == '/') {
			xbegin = i;
			printf("xbegin = %d\n", xbegin);
		}
	}
	
	int ind;
	char *ptr;
	for(i = xbegin ; i < xlength; i++) {
		if(x[i] == '/') {
			continue;
		}

		ptr = strchr(cipher, x[i]);
		// printf("ptr sebelum : %s\n", ptr);
		if(ptr) {
			ind = ptr - cipher;
			// printf("ptr sesudah : %s\n", ptr);
			// printf("ini ind : %d\n", ind);
			x[i] = cipher[(ind + key) % strlen(cipher)];
		}
	}

	// printf("hasil encrypt %s\n", x);
}

void decrypt(char *y) {
	printf("string dekrip = %s\n", y);

	int ylength = strlen(y), ybegin = 0;
	int i;
	
	//kalo ketemu slash atau end of array di fullpath dibreak
	for (int i = 1; i < ylength; i++) {
		if(y[i] == '/' || y[i+1] == '\0') {
			ybegin = i + 1;
			printf("ybegin = %d\n", ybegin);
			break;
		}
	}

	for(int i = strlen(y); i >= 0; i--) {
		// printf("y[i] -> %c\n", y[i]);
		if(y[i] == '/') {	
			break;
		}
		if(y[i] == '.' && i == (strlen(y)-1)) { 
			ylength = strlen(y);
			break;
		}
		if(y[i] == '.' && i != (strlen(y)-1)) {
			ylength = i - 1;
			break;
		}
	}

	int ind;
	char *ptr;
	for(i = ybegin ; i < ylength; i++) {	// loop namefile
		if(y[i] == '/') {	//kalo nemu slash continue
			continue;
		}
		ptr = strchr(cipher, y[i]);
		// printf("ptr dekrip sebelum : %s\n", ptr);
		if(ptr) {
			ind = ptr - cipher - key; // cari indeks dekrip
			// printf("ptr dekrip sesudah : %s\n", ptr);
			// printf("ini ind dekrip : %d\n", ind);
			if (ind < 0) {
				ind = ind + strlen(cipher);
			}
			y[i] = cipher[ind];
		}
		// printf("y keluar %c\n", y[i]);
	}

	// printf("hasil decrypt %s\n", y);
}
```
Pemanggilan fungsi dekrip pada semua fungsi fuse dan pemanggilan fungsi enkrip pada fungsi fuse readdir.
## 2. Soal 2
Belum sempat mengerjakan karena kekurangan waktu
## 3. Soal 3
Belum sempat mengerjakan karena kekurangan waktu dan kurang mengerti maksud soal
## 4. Soal 4
Pada soal nomer 4 kita disuruh membuat log dengan 2 jenis status yakni INFO dan WARNING. Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink. Berikut fungsinya :
```c
char *LOG = "/home/sin/fs.log";

void writeLOG(char *cmd, char* path, int flag, char* temp2)
{
	char desc2[1000];
    char* status;
    char str[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
	if(flag == 1){
		status = "INFO";
	}
	else status = "WARNING";
	if(strcmp(temp2,"")) sprintf(desc2, "::%s", temp2);
    sprintf(str, "%s::%02d%02d%02d-%02d:%02d:%02d::%s::%s%s", status, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, cmd, path, desc2);
    FILE *out = fopen("/home/sin/fs.log", "a");  
    fprintf(out, "%s\n", str);  
    fclose(out);  
    // return 0;
    
}
```
Fungsi writeLOG dipanggil pada semua fungsi fuse. Contoh :
```c
writeLOG("LS", fpath, 1, ""); // INFO
```
```c
writeLOG("RMDIR", fpath, 0, ""); // WARNING
```
```c
writeLOG("RENAME", src, 0, dst); // RENAME
```
