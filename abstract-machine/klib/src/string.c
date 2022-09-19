#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
//   panic("Not implemented");
      int len = 0;
    assert(s);
    while(*s++ != '\0')
        len ++;
    return len;

}

// char *strcpy(char *dst, const char *src) {
//   panic("Not implemented");
// }

char* strcpy(char *dst, const char *src)
{
    char *p = dst;
    while((*dst++ = *src ++) != '\0');
    return p;
}

char *strncpy(char *dst, const char *src, size_t n) {
//   panic("Not implemented");
    char *tmp = dst;

   while (n) {
        if ((*tmp = *src) != 0)
            src++;
        tmp++;
        n--;
    }
    return dst;
}
// char *strcat(char *dst, const char *src) {
//   panic("Not implemented");
// }

char* strcat(char *dst, const char *src)
{
    assert(dst != NULL && src != NULL);
    char *temp = dst;
    while (*temp != '\0')
        temp++;
    while ((*temp++ = *src++) != '\0');

    return dst;
}

// int strcmp(const char *s1, const char *s2) {
//   // panic("Not implemented");

// }
int strcmp(const char *s1, const char *s2)  
{  
    for ( ; *s1 == *s2; s1++, s2++)  
    {     
        if (*s1 == '\0')  
        return 0;  
    }  
      
    if (*(unsigned char*)s1 > *(unsigned char*)s2){  
        return 1;  
    }else if (*(unsigned char*)s1 < *(unsigned char*)s2){  
        return -1;  
    } else return 0;
}  

int strncmp(const char *s1, const char *s2, size_t n) {

 
    if(!n)return 0;
 
    while(--n && *s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
 
    return *s1 - *s2;
//   panic("Not implemented");
}

// void *memset(void *s, int c, size_t n) {
//   panic("Not implemented");
// }



void *memset(void *s, int c, size_t n){
	// if (s == NULL || n < 0)
	// {
	// 	return NULL;
	// }
	char *pdest = (char *)s;
	while (n-->0)
	{
		*pdest++ = c;
	}
	return s;
}


void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *s1, const void *s2, size_t n) {
  // panic("Not implemented");
  	if(s1 == NULL || s2 == NULL || n <= 0)
		return s1;
 
	char* dst_pos = (char *)s1;
	char* src_pos = (char *)s2;
	if(dst_pos < src_pos + n && s1 > s2){ //DOWN COPY，向前拷贝
		dst_pos = dst_pos + n;
		src_pos = src_pos + n;
 
		while(n > 0){
			*dst_pos-- = *src_pos--;
			n --;
		}
	}
	else { //UP COPY,向后拷贝
		while(n > 0){
			*dst_pos++ = *src_pos++;
			n --;
		}
	}
 
	return s1; 
}

// int memcmp(const void *s1, const void *s2, size_t n) {
//   panic("Not implemented");
// }

int memcmp(const void * s1, const void * s2, size_t n )

{

// if (!n )
// {return 0;}

	while (--n &&*(char*) s1 == *(char *)s2){
		s1 = (char *)s1 + 1;
		s2 = (char *)s2 + 1;
	}
	return (*(unsigned char*) s1 - *(unsigned char*)s2 );
}

#endif
