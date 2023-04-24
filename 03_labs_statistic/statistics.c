#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define TWO_DICE_SUM 11

// Calculates arithmetic mean and variance of numbers from n-element array v[]
// for n = 0: mean = variance = 0
void mean_variance(const int v[], int n, double *mean, double *variance) {
    double m_sum = 0;
    double v_sum = 0;
    for(int i = 0; i < n; i++){
        m_sum += v[i];
    }
     *mean = (double) roundf(m_sum/n * 100) / 100;
    //*mean = (double)m_sum/n;
    for(int i = 0; i < n; i++){
        v_sum += (v[i] - *mean) * (v[i] - *mean);
    }
    *variance = roundf(v_sum/n * 100) / 100;
    //*variance = (double)v_sum/n;
}

// Fill n-element array v[] with Bernoulli sample (binary values)
void bernoulli_gen(int v[], int n, double probability) {
    double res = 0;
    for(int i = 0; i < n; i++){
        res = rand();
        if (res < probability*(RAND_MAX)){
            v[i] = 1;
        }
        else{
            v[i] = 0;
        }
    }
}

// Calculate pmf (probability mass function) of discrete random variable.
// Two dice are thrown simultaneously The random variable is the sum of the
// numbers shown (the result is from [2, 12]).
// trials - number of trials
void pmf(double v[], int trials) {
    int sum;
    for (int i = 2; i < 13; i++){
        v[i] = 0;
    }
    for (int i = 0; i < trials; i++){
        sum = 0;
        sum += 1 + rand()%6;
        sum += 1 + rand()%6;
        v[sum] += 1;
    }
    double probability;
    for (int i = 2; i <= 12; i++) {
        probability = roundf(v[i] / trials * 1000.0) / 1000.0;
        v[i] = probability;
    }

}

// Calculates the cumulative distribution function (v) for 'trials' numbers
// Two dice are thrown simultaneously The random variable is the sum of the
// numbers shown (the result is from [2, 12]).
// trials - number of trials
void cdf(double v[], int trials) {
    int sum;
    for (int i = 1; i < 13; i++){
        v[i] = 0;
    }
    for (int i = 0; i < trials; i++){
        sum = 0;
        sum += 1 + rand()%6;
        sum += 1 + rand()%6;
        v[sum] += 1;
    }
    double probability;
    for (int i = 2; i <= 12; i++) {

        //probability = roundf(v[i] / trials * 1000.0) / 1000.0;
        probability = (double)(v[i])/trials;
        v[i] = probability+v[i-1];
    }


}

// Histogram - bar chart for the values from v[] array of length n
// x_start - the first value on the abscissa (x increment is 1),
// y_scale - y value corresponding to single char of the histogram
void print_histogram(const double v[], int n, int x_start, double y_scale, char mark) {
    for (int i = x_start; i <= 12; i++){
        double probability = v[i];
        if (i < 10){
            printf(" %d", i);
        }
        else{
            printf("%d", i);
        }
        printf(" |");

        for (int i = 0; i < roundf(probability/y_scale); i++){
            printf("%c", mark);
        }
        printf(" %.3f\n", probability);
    }
}

// Simulate Monty-Hall problem
// input: n - number of trials
// output: wins - number od wins if door switched
// output: win_probability - probability of win if door switched

// This function calls rand() exactly 2 times:
// the first one returns winning door number,
// the second one returns door number that the player has chosen the first time,
void monty_hall(int n, int *p_switch_wins) {

    int prize, choose, cnt =0;
    for (int i = 0; i < n; i++){
        prize = rand()%3;
        choose = rand()%3;
        if (prize!=choose){
            cnt +=1;
        }
    }
    *p_switch_wins = cnt;
}

// print double vector of size n (with 2 figures after the decimal point)
//void print_vector(const double v[], int n) {
//	for (int i = 0; i < n; ++i) {
//		printf("%.2f ", v[i]);
//	}
//	printf("\n");
//}

// print integer vector
void fill_with_randoms(int arr[], int n, int a, int b){
    for (int i = 0; i < n; i++){
        arr[i] = a+rand()%(b-a+1);
    }
}
void print_int_vector(const int v[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int main(void) {
    char mark;
    int to_do, n, seed, m_h_wins, a, b, i_vector[100];
    double arithmetic_mean, variance, probability, d_vector[100];
    scanf("%d", &to_do);
    scanf("%d",&seed);
    scanf("%d",&n);
    srand((unsigned)seed);

    switch (to_do) {
        case 1: // mean_variance
            scanf("%d %d", &a, &b);
            fill_with_randoms(i_vector, n, a, b);
            mean_variance(i_vector, n, &arithmetic_mean, &variance);
            printf("%.2f %.2f\n", arithmetic_mean, variance);
            break;
        case 2: // bernoulli_gen
            scanf("%lf", &probability);
            bernoulli_gen(i_vector, n, probability);
            print_int_vector(i_vector, n);
            break;
        case 3: // pmf
            scanf(" %c", &mark);
            pmf(d_vector, n);
            print_histogram(d_vector, TWO_DICE_SUM, 2, 0.005, mark);
            break;
        case 4: // cdf
            scanf(" %c", &mark);
            cdf(d_vector, n);
            print_histogram(d_vector, TWO_DICE_SUM, 2, 0.02, mark);
            break;
        case 5: // monty_hall
            monty_hall(n, &m_h_wins);
            printf("%d %d\n", m_h_wins, n - m_h_wins);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

