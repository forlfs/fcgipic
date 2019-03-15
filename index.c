#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include"fcgi_stdio.h"
#include"param_lib.h"
#include"snowflake.h"

void index_html()
{
	printf("Content-Type: text/html; charset=utf-8\r\n");
	printf("\r\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n");
	printf("<title>pic</title>\n");
	printf("</head>\n");
	printf("<body>\n");
	printf("<form method=\"post\" action=\"pic.html\" enctype=\"multipart/form-data\">\n");
	printf("<input type=\"file\" name=\"pic\">\n");
	printf("<input type=\"submit\" value=\"提交\">\n");
	printf("</form>\n");
	printf("</body>\n");
	printf("</html>\n");
}

void pic_html()
{
	param_env *p;
	char *pic;
	int l;
	long long id;
	char s[120];
	int fd;
	p=param_new(4);
	param_form_data_set(p);
	l=0;
	pic=param_get_x(p,"pic",&l);
	if(l==0){
		param_del(p);
		printf("Location: index.html\r\n");
		printf("\r\n");
		return;
	}
	id=snowflake_id();
	if(id==-1){
		param_del(p);
		printf("Location: index.html\r\n");
		printf("\r\n");
		return;
	}
	sprintf(s,"/home/www/fcgipic/pic/%lld.jpg",id);
	fd=open(s,O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
	write(fd,pic,l);
	close(fd);
	param_del(p);
	printf("Content-Type: text/html; charset=utf-8\r\n");
	printf("\r\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n");
	printf("<title>pic</title>\n");
	printf("</head>\n");
	printf("<body>\n");
	sprintf(s,"<img src=\"pic/%lld.jpg\">\n",id);
	printf(s);
	printf("</body>\n");
	printf("</html>\n");
}

int main()
{
	char *s;
	while(FCGI_Accept()>=0){
		s=getenv("SCRIPT_FILENAME");
		if(strcmp(s,"/pic.html")==0){
                        pic_html();
                }else{
			index_html();
		}
	}
	exit(0);
}

