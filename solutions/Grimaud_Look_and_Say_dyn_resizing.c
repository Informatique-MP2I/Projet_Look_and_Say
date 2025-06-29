#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void swap(char **p1, char **p2) {
  char *temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

int compute_look_and_say(char *input, int n, char **output) {
  const double lambda = 1.5; 
  int buffer_1_size = buffer_size;
  int buffer_2_size = buffer_size;
  char *buffer_1 = (char *)malloc(buffer_1_size * sizeof(char));
  if(buffer_1==NULL) return -1;
  char *buffer_2 = (char *)malloc(buffer_2_size * sizeof(char));
  if (buffer_2==NULL) {
    free(buffer_1);
    return -1;
  }
  int size = read_look_and_say(input, buffer_1, buffer_1_size);
  if (size == -1) {
    free(buffer_1);
    free(buffer_2);
    return -1;
  }
  for (int i = 0; i < n; i++) {
    int estimated_size = (int)(size * lambda);
    // Output buffer size verification 
    if (estimated_size >= buffer_2_size) {
      buffer_2_size = estimated_size;
      free(buffer_2);
      buffer_2 = (char *)malloc(buffer_2_size * sizeof(char));
      if (buffer_2 == NULL) {
        free(buffer_1);
        return -1;
      }
    }
    size = compute_next_look_and_say(buffer_1, size, buffer_2, buffer_2_size);
    //fprintf(stderr,"step %d - estimated size = %d, actual size %d\n",i, estimated_size, size);
    swap(&buffer_1, &buffer_2);
    int tmp=buffer_1_size;
    buffer_1_size=buffer_2_size;
    buffer_2_size=tmp;
    if(size==-1) {
      free(buffer_1);
      free(buffer_2);
      return -1;
    }
  }
  free(buffer_2);
  *output = buffer_1;
  return size;
}

void compute_abundance(char *value, int value_size, int num_atoms, char **atom_names, char **atom_sequences, int *sequence_sizes) {
  int atom_counts[num_atoms];
  int digit_counts[10] = {0,0,0,0,0,0,0,0,0,0};
  int total_digits = 0;
  int total_atoms = 0;
  for (int i = 0; i < num_atoms; i++) 
    atom_counts[i]=0;
  for (int i = 0; i < value_size; i++) {
    if(value[i]>=0 && value[i]<10) {
      digit_counts[(int)value[i]]++;
      total_digits++;
    } 
    // Check if an atom starts at this position
    for (int atom_index = 0; atom_index < num_atoms; atom_index++) {
      int sequence_size = sequence_sizes[atom_index];
      // Ensure the atom fits within the remaining sequence
      if (i + sequence_size <= value_size) {
        int match = 1;
        // Check if the atom matches the sequence at this position
        for (int j = 0; j < sequence_size; j++) {
          if (value[i + j] != atom_sequences[atom_index][j]) {
            match = 0;
            break;
          }
        }
        if (match) {
          atom_counts[atom_index]++;
          total_atoms++;
          for(int j=i+1;j<i+sequence_size;j++)
            if(value[j]>=0 && value[j]<10) {
              digit_counts[(int)value[j]]++;
              total_digits++;
            }
          i += sequence_size - 1;
          break; // No need to check other atoms at this position
        }
      }
    }
  }
  printf("Atom abundance:\n");
  for (int atom_index = 0; atom_index < num_atoms; atom_index++) {
    int count = atom_counts[atom_index];
    double percentage = ((double)count / total_atoms) * 100;
    printf("Atom %s: %d occurrence(s) (%.2f%%)\n", atom_names[atom_index], count, percentage);
  }
  printf("\nDigit distribution:\n");
  for (int digit = 0; digit < 10; digit++) {
    float percentage = ((float)digit_counts[digit] / total_digits) * 100;
    if(digit_counts[digit]!=0)
      printf("Digit %d: %d occurrence(s) (%.2f%%)\n", digit, digit_counts[digit], percentage);
  }
}  

/*
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
*/

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <n> <look-and-say-seed> <-print|-stat>\n", argv[0]);
    return 1;
  }
  int n = atoi(argv[1]);
  char *output = NULL;
  int size = compute_look_and_say(argv[2], n, &output);
  if (size == -1) {
    printf("Error: Could not compute Look-and-say sequence.\n");
    return 1;
  }
  if (strcmp(argv[3], "-print") == 0) {
    print_look_and_say(output, size);
  } else if (strcmp(argv[3], "-stat") == 0) {
    printf("Number of digits: %d\n", size);          
    char *atom_names[4] = {"C", "N", "O", "H"};
    char *atom_sequences[4] = {
      (char[]){3, 1, 1, 3, 1, 1, 2, 2, 1, 1, 3, 2, 2, 1, 1, 2, 
               2, 1, 1, 2, 1, 3, 3, 2, 2, 1, 1, 2}, 
      (char[]){1, 1, 1, 3, 1, 2, 2, 1, 2, 2, 2, 1, 1, 2, 1, 1, 
               2, 3, 2, 2, 2, 1, 1, 2}, 
      (char[]){1, 3, 2, 1, 1, 2, 2, 1, 1, 2, 1, 3, 3, 2, 2, 1,
               1, 2},
      (char[]){2, 2} 
      };
    int sequence_sizes[4] = {28, 24, 18,2};
    compute_abundance(output, size, 4, atom_names, atom_sequences, sequence_sizes);
  } else {
    printf("Invalid argument: use -print or -stat.\n");
    return 1;
  }
  free(output);
  return 0;
}

