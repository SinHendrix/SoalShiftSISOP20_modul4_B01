#define FUSE_USE_VERSION 28

#include <fuse.h>

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <fcntl.h>

#include <dirent.h>

#include <errno.h>

#include <sys/time.h>

  

static  const  char *dirpath = "/home/sin/Documents";
char urutan[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

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



static  int  xmp_getattr(const char *path, struct stat *stbuf)

{

writeLOG("LS", path, 1);
int res;

char fpath[1000];
char s[1000];
sprintf(s, "%s", path);
printf("get attr s = %s\n", s);
dekrip(s);
printf("get attr enkrip s = %s\n", s);
sprintf(fpath,"%s%s",dirpath, s);
printf("get attr path = %s\n", path);
printf("get attr fpath = %s\n", fpath);

res = lstat(fpath, stbuf);

  

if (res == -1)

return -errno;

  

return 0;

}

  

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,

off_t offset, struct fuse_file_info *fi)

{

writeLOG("CD", path, 1);

char fpath[1000];

if(strcmp(path,"/") == 0)

{

path=dirpath;

sprintf(fpath,"%s",path);
// printf("readdir if = %s\n", fpath);

}

else {
    char s[1000];
    sprintf(s, "%s", path);
    printf("readdir s = %s\n", s);
    dekrip(s);
    printf("readdir enkrip s = %s\n", s);
    sprintf(fpath, "%s%s",dirpath, s);
    // printf("readdir else = %s\n", fpath);
}

int res = 0;

  

DIR *dp;

struct dirent *de;

  

(void) offset;

(void) fi;

  

dp = opendir(fpath);

if (dp == NULL)

return -errno;

  

while ((de = readdir(dp)) != NULL) 
  {
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;
    char s[1000];
    sprintf(s, "%s", de->d_name);
    printf("readdir de %s\n", s);
    enkrip(s);
    printf("readdir dekrip s = %s\n", s);
    if (filler(buf, s, &st, 0)) break;
  }


  

closedir(dp);

return 0;

}

  

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,

struct fuse_file_info *fi)

{

writeLOG("CAT", path, 1);

char fpath[1000];

if(strcmp(path,"/") == 0)

{

path=dirpath;

sprintf(fpath,"%s",path);
// printf("read if = %s", fpath);


}

else {
    char s[1000];
    sprintf(s, "%s", path);
    printf("xmp_read s = %s\n", s);
    dekrip(s);
    printf("xmp_read enkrip s = %s\n", s);
    sprintf(fpath, "%s/%s",dirpath, s);
    // printf("read else = %s", fpath);
}

int res = 0;

int fd = 0 ;

  

(void) fi;

fd = open(fpath, O_RDONLY);

if (fd == -1)

return -errno;

  

res = pread(fd, buf, size, offset);

if (res == -1)

res = -errno;

  

close(fd);

return res;

}

static int xmp_mkdir(const char *path, mode_t mode)
{
    writeLOG("MKDIR", path, 1);
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    writeLOG("CREATE", path, 1);
	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}  

static int xmp_unlink(const char *path)
{

    writeLOG("REMOVE", path, 2);
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
    writeLOG("RMDIR", path, 2);
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    writeLOG("MOVE", from, 1);
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    writeLOG("TRUNCATE", path, 1);
	int res;

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    writeLOG("OPEN", path, 1);
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
    writeLOG("WRITE", path, 1);
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {

.getattr = xmp_getattr,

.readdir = xmp_readdir,

.read = xmp_read,

.mkdir = xmp_mkdir,

.mknod = xmp_mknod,

.unlink = xmp_unlink,

.rmdir = xmp_rmdir,

.rename = xmp_rename,

.truncate = xmp_truncate,

.open = xmp_open,

.read = xmp_read,

.write = xmp_write,

};

  

int  main(int  argc, char *argv[])

{

umask(0);

return fuse_main(argc, argv, &xmp_oper, NULL);

}
