#include <unistd.h>
#include <stdio.h>

int main()
{
  char *str = "---- New APP 1.0 -----";
  int i = 0;
  while (1)
  {
    printf("hellow [%i]\n", i);
    printf("%s\n", str);
    i++;
    sleep(2);
  }
  return 0;
}