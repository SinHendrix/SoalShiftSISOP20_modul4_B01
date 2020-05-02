#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/sin/Documents";

int key = 10;
char cipher[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000], temp[1000];
	strcpy(temp, path);
	// /Documents/encv1_rhs/FOTO_PENTING/kelincilucu.jpg
	// membandingkan 7 karakter pertama
	if(strncmp(path, "/encv1_", 7) == 0) {
		decrypt(temp);
	}

	sprintf(fpath, "%s%s", dirpath, temp);
	// printf("getattr fpath : %s\n", fpath);
    res = lstat(fpath, stbuf);

    if(res == -1) return -errno;

	writeLOG("LS", fpath, 1, "");

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000], temppath[1000];
	
    if(strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else{
		strcpy(temppath, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temppath);
		}

		sprintf(fpath, "%s%s", dirpath, temppath);
	}
	// printf("readdir fpath : %s\n", fpath);
	
    int res = 0;

    DIR *dp;
    struct dirent *de;

    (void)offset;
    (void)fi;

    dp = opendir(fpath);

    if(dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
		//titik sama dua titik direktori
		if(strcmp(de->d_name, ".") == 0 || 
			strcmp(de->d_name, "..") == 0) {
				continue;
		}

		char temp[1000];
		
		strcpy(temp, de->d_name);	// ngambil nama file doang
		// printf("ini temp = %s\n", temp);

		if(strncmp(path, "/encv1_", 7) == 0) {
			encrypt(temp);
		}

		res = (filler(buf, temp, &st, 0));

        if (res != 0)
            break;
    }

    closedir(dp);
	writeLOG("CD", fpath, 1, "");
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}
	
	// printf("read fpath : %s\n", fpath);
    int res = 0;
    int fd = 0;

    (void)fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) 
        res = -errno;

    close(fd);

    return res;
}

static int xmp_truncate(const char *path, off_t size)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}

    int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

    // logs(0, "WRITE", fpath, "");
	writeLOG("WRITE", fpath, 1, "");

	return res;
}

static int xmp_unlink(const char *path)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

    // logs(1, "UNLINK", fpath, "");
	writeLOG("UNLINK", fpath, 0, "");

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	char src[1000], dst[1000];
	printf("ini from : %s\n", from);
	printf("ini to : %s\n", to);
	if (strcmp(from, "/") == 0)
    {
        from = dirpath;
        sprintf(src, "%s", from);
    }
    else {
		char tempa[1000];		//rename folder encv ke biasa
		strcpy(tempa, from);

		if(strncmp(from, "/encv1_", 7) == 0) {	//ada atau gak ada folder encv1_
			decrypt(tempa);
		}

		sprintf(src, "%s%s", dirpath, tempa);
	}

	if (strcmp(to, "/") == 0)
    {
        to = dirpath;
        sprintf(dst, "%s", to);
    }
    else {
		char tempb[1000];		//rename dari biasa ke encv
		strcpy(tempb, to);

		if(strncmp(to, "/encv1_", 7) == 0) {
			decrypt(tempb);		//dekrip yg didocomuents
		}

		sprintf(dst, "%s%s", dirpath, tempb);
	}
	
	int res;

	res = rename(src, dst);
	if (res == -1)
		return -errno;

	writeLOG("RENAME", src, 0, dst);

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = mkdir(fpath, 0750);
	if (res == -1)
		return -errno;

	// logs(0, "MKDIR", fpath, "");
	writeLOG("MKDIR", fpath, 1, "");

	return 0;
}

static int xmp_rmdir(const char *path)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);
		// /home/sin/Documents/encv1_rhs/FOTO_PENTING/kelincilucu.jpg
		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	// logs(1, "RMDIR", fpath, "");
	writeLOG("RMDIR", fpath, 0, "");

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	
    // logs(0, "OPEN", fpath, "");
	writeLOG("OPEN", fpath, 1, "");

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else {
		char temp[1000];
		strcpy(temp, path);

		if(strncmp(path, "/encv1_", 7) == 0) {
			decrypt(temp);
		}

		sprintf(fpath, "%s%s", dirpath, temp);
	}

	int res;

	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	// logs(0, "CREATE", fpath, "");
	writeLOG("CREATE", fpath, 1, "");

	return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr 	= xmp_getattr,
    .readdir 	= xmp_readdir,
    .read 		= xmp_read,
	.truncate   = xmp_truncate,
	.write      = xmp_write,
	.unlink     = xmp_unlink,
	.rename 	= xmp_rename,
	.mkdir		= xmp_mkdir,
	.rmdir		= xmp_rmdir,
	.open       = xmp_open,
	.mknod 		= xmp_mknod,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
