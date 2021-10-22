#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *routine(void *arg)
{
	printf("In threads\n");
	return (NULL);
}

int main()
{
	pthread_t t1;
	pthread_create(&t1, NULL, routine, NULL);
	pthread_join(t1, NULL);
	return 0;
}
