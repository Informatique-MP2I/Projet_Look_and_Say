#include <stdio.h>
#include <stdlib.h>

const int buffer_size = 1000;

void print_look_and_say(char *value, int size) {
  for (int i = 0; i < size; i++)
    if(value[i]>=0 && value[i]<10) 
      printf("%d", value[i]);
    else
      printf("?");
  printf("\n");
}

int compute_next_look_and_say(char *last, int last_size, char *next, int max_size) { 
  int count = 1;
  int next_index = 0;
  for (int i = 1; i < last_size; i++) {
    if (last[i] == last[i - 1]) 
      count++;
    else {
      if (next_index + 2 >= max_size) // next is too small?
        return -1;
      next[next_index++] = count;
      next[next_index++] = last[i - 1];
      count = 1;
    }
  }
  // Add the last entry in next if possible
  if (next_index + 2 > max_size)
    return -1;
  next[next_index++] = count;
  next[next_index++] = last[last_size - 1];
  return next_index;
}

int read_look_and_say(char* input, char *output, int output_size) {
  int i = 0;
  while (input[i] != '\0' && i < output_size) {
    if (input[i] < '0' || input[i] > '9')
      return -1;  // Invalid input string
    output[i] = input[i] - '0';  // Character to digit conversion
    i++;
  }
  if (input[i] != '\0') 
    return -1;  // The size of the array is not sufficient
  return i;  // Returns the number of elements in the output
}

/*
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

  char value[10] = {1, 1, 2, 1, 0, 0, 0, 0, 0, 0};
  int size = 4;
  print_look_and_say(value, size);

  char before[10] = {1, 1, 1, 2, 2, 1, 0, 0, 0, 0};
  char after[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int new_size = compute_next_look_and_say(before, 6, after, 10);
  print_look_and_say(after, new_size);
  
  return 0;
}
*/

void swap(char **p1, char **p2) {
  char *temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

int compute_look_and_say(char *input, int n, char **output) {
  char *buffer_1 = (char *)malloc(buffer_size * sizeof(char));
  if(buffer_1==NULL) return -1;
  char *buffer_2 = (char *)malloc(buffer_size * sizeof(char));
  if (buffer_2==NULL) {
    free(buffer_1);
    return -1;
  }
  
  int size = read_look_and_say(input, buffer_1, buffer_size);
  if (size == -1) {
    free(buffer_1);
    free(buffer_2);
    return -1;
  }
  for (int i = 0; i < n; i++) {
    size=compute_next_look_and_say(buffer_1, size, buffer_2, buffer_size);
    if(size==-1) 
      break;
    swap(&buffer_1, &buffer_2);
  }
  free(buffer_2);
  *output = buffer_1;  
  return size;
}

/*
int main(int argc, char *argv[]) {
  char output[buffer_size];
  if (argc != 2) {
    printf("Usage: %s <look-and-say-sequence>\n", argv[0]);
    return 1;
  }
  int size = read_look_and_say(argv[1], output, buffer_size);
  if (size == -1) {
    printf("Error: invalid input or output buffer too small.\n");
    return 1;
  }
  print_look_and_say(output, size);
  return 0;
}
*/

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <n> <look-and-say-seed>\n", argv[0]);
    return 1;
  }
  int n = atoi(argv[1]);
  char *output = NULL;
  int size = compute_look_and_say(argv[2], n, &output);
  if (size == -1) {
    printf("Error: Could not compute Look-and-say sequence.\n");
    return 1;
  }
  print_look_and_say(output, size);
  free(output); 
  return 0;
}
