#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "calculator.h"

typedef struct {
    char operator;
    double operand1;
    double operand2;
    double result;
} Calculation;

void* calculate(void* arg) {
    Calculation* calc = (Calculation*)arg;
    switch (calc->operator) {
        case '+':
            calc->result = calc->operand1 + calc->operand2;
            break;
        case '-':
            calc->result = calc->operand1 - calc->operand2;
            break;
        case '*':
            calc->result = calc->operand1 * calc->operand2;
            break;
        case '/':
            if (calc->operand2 != 0) {
                calc->result = calc->operand1 / calc->operand2;
            } else {
                fprintf(stderr, "Error: Division by zero.\n");
                calc->result = 0;
            }
            break;
        default:
            fprintf(stderr, "Error: Unknown operator '%c'.\n", calc->operator);
            calc->result = 0;
            break;
    }
    return NULL;
}

void start_calculator() {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(fd[1]);
        Calculation calc;
        read(fd[0], &calc, sizeof(Calculation));
        close(fd[0]);

        pthread_t thread;
        pthread_create(&thread, NULL, calculate, (void*)&calc);
        pthread_join(thread, NULL);

        printf("Result: %f\n", calc.result);
        exit(EXIT_SUCCESS);
    } else {
        close(fd[0]);
        Calculation calc;

        printf("Enter operation (format: operand1 operator operand2): ");
        scanf("%lf %c %lf", &calc.operand1, &calc.operator, &calc.operand2);

        write(fd[1], &calc, sizeof(Calculation));
        close(fd[1]);

        wait(NULL); 
    }
}
