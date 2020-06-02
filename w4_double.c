#include <stdio.h>
#include <stdlib.h>

void print_file(FILE *fptr)
{
  int c;
  rewind(fptr);
  while ((c = getc(fptr)) != EOF)
    putc(c, stdout);
}

void double_space(FILE *ifp, FILE *ofp)
{
  int c;
  rewind(ifp);
  while ((c = getc(ifp)) != EOF) {
    putc(c, ofp);

    if (c == '\n')
      putc('\n', ofp);
  }
}

int main(int argc, char * argv[])
{
  FILE *ifp, *ofp;
  int c;

  if (argc != 3)
  {
    printf("Need executable inputfile outputfile\n");
    exit(1);
  }
  else {
    ifp = fopen(argv[1], "r");
    ofp = fopen(argv[2], "w+");
    printf("number of arguments: %d\n", argc);
    printf("My %s file as read is\n", argv[1]);
    print_file(ifp);
    printf("---------------------------------\n");

    double_space(ifp, ofp);
    printf("My %s file double space is\n", argv[2]);
    print_file(ofp);
    printf("---------------------------------\n");
    fclose(ifp);
    fclose(ofp);
  }
}
