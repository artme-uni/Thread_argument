#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define THREAD_COUNT 4
#define ERR_CODE_ILLEGAL_ARG 2

typedef struct {
    int count;
    char **lines;
} line_sequence;

void *print_lines(void *args) {
    if (NULL == args) {
        return (void *) ERR_CODE_ILLEGAL_ARG;
    }
    int num_lines = ((line_sequence *) args)->count;
    char **lines = ((line_sequence *) args)->lines;
    
    for (int i = 0; i < num_lines; i++) {
        printf("%s\n", lines[i]);
    }
    return (void *) EXIT_SUCCESS;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    line_sequence thread_args[THREAD_COUNT] = {
            {1, (char *[]) {"Every man has his price"}},
            {2, (char *[]) {"Suffering for a friend", "doubleth friendship"}},
            {3, (char *[]) {"Dont keep", "a dog and", "bark yourself"}},
            {4, (char *[]) {"Its better", "to light", "a candle than", "curse the darkness"}}
    };

    int created_thread_count = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        int creation_code = pthread_create(&threads[i], NULL, print_lines, (void *) &thread_args[i]);
        if (0 != creation_code) {
            fprintf(stderr, "Cannot create thread #%d: %s\n", i, strerror(creation_code));
            break;
        }
        created_thread_count++;
    }

    int exit_code = EXIT_SUCCESS;
    for (int i = 0; i < created_thread_count; ++i) {
        void *return_value;
        int join_code = pthread_join(threads[i], &return_value);
        if (0 != join_code) {
            fprintf(stderr, "Cannot join child thread: %s\n", strerror(join_code));
            exit_code = EXIT_FAILURE;
        }
        if(EXIT_SUCCESS != (long) return_value){
            fprintf(stderr, "Illegal thread argument");
        }
    }
    return exit_code;
}
