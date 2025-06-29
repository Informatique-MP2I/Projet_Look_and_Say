int main(int argc, char **argv) {
  char *current = (char[]){1,2,1,1};
  char *successor = (char[]){1,1,1,2,2,1};
  char **p1 = &current;
  char **p2 = &successor;
  printf("Before swap : ");
  printf("current=%p, successor=%p\n", current, successor);
  swap(p1, p2);
  printf("After swap  : ");
  printf("current=%p, successor=%p\n", current, successor);

  return 0;
}

