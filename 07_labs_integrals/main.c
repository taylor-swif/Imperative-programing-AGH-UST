#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp)(double);

// example functions of one variable
double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
    return x * (x * (x * x * (2 * x - 4) + 3.5) +1.35) - 6.25;
}

double f_rat(double x) {
    return 1/((x - 0.5)*(x - 0.5) + 0.01);
}

double f_exp(double x) {
    return 2*x*exp(-1.5*x) - 1;
}

double f_trig(double x) {
    return x* tan(x);
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
    double field = 0;
    double h = (b - a) / n;
    double x = a;
    for (int i = 0; i < n; i++){
        field += h * f1(x);
        x += h;
    }
    return field;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
    double field = 0;
    double h = (b - a) / n;
    double x = a;
    for (int i = 0; i < n; i++){
        field += h *f1(x + h);
        x += h;
    }
    return field;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
    double field = 0;
    double h = (b - a) / n;
    double x = a;
    for (int i = 0; i < n; i++){
        field += h * f1((x + x + h) / 2);
        x += h;
    }
    return field;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
    double field = 0;
    double h = (b - a) / n;
    double x = a;
    double fa = func(a);
    double fb;
    for (int i = 0; i < n; i++){
        x += h;
        fb = func(x);
        field += h*(fa + fb) / 2;
        fa = fb;
    }
    return field;
}

// Simpson's rule
double quad_simpson(Func1vFp f, double a, double b, int n) {
    double field = 0;
    double h = (b - a) / n;
    double x = a;
    double fa = f(a);
    double fb;
    double fc;
    for (int i = 0; i < n; i++){
        fc = f((x + x + h)/2);
        x += h;
        fb = f(x);
        field += h*(fa + 4*fc + fb)/6;
        fa = fb;
    }
    return field;
}

// pointer to quadrature function
typedef double (*QuadratureFp)(Func1vFp, double , double, int);

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
        quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    return quad_tab[quad_no](func_tab[fun_no], a, b, n);
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
    if (level == RECURS_LEVEL_MAX){
        return NAN;
    }
    double c = (a + b)/2;
    double S1 = quad(f, a, c, 1);
    double S2 = quad(f, c, b, 1);
    if (fabs(S - S1 - S2) < delta){
        return S1 + S2;
    }

    return recurs(f, a, c, S1, delta/2, quad, level + 1) + recurs(f, c, b, S2, delta/2, quad, level + 1);
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
    double S = quad(f, a, b, 1);
    return recurs(f, a, b, S, delta, quad, 0);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp)(double, double);

double func2v_2(double x, double y) {
    return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
    return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
    return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
    double volume = 0;
    double hx = (x2 - x1) / nx;
    double hy = (y2 - y1) / ny;
    double x = x1;
    double y;
    for (int i = 0; i < nx; i++){
        y = y1;
        for (int j = 0; j < ny; j++) {
            volume += f(x, y);
            y += hy;
        }
        x += hx;
    }
    return volume * hx * hy;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy, Func1vFp fg, Func1vFp fh) {

    double hx = (x2-x1) / nx;
    double x = (2*x1 + hx)/2;
    double field = 0;

    for (int i = 0; i < nx; i++) {
        double y1 = fg(x);
        double y = (2*y1+ hy) /2;
        while (y <= fh(x)) {
            field += f(x, y) * hy * hx;
            y += hy;
        }
        x += hx;
    }
    return field;
}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2,
                           int ny, Func1vFp fg, Func1vFp fh) {

    double hy = (x2 - x1) / nx;
    double field = 0;
    double a, b;
    double x = x1;


    for (int i = 0; i < nx; i++) {
        a = fmax(y1, fg(x));
        b = fmin(y2, fh(x));
        if (a <= b) {
            field += dbl_integr(f, x, x + hy, 1, a, b, (b - a) / ((y2 - y1) / ny) + 1);
        }
        x += hy;
    }
    return field;

}
// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
    return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
    return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
    double fv = 1.;
    for (int i = 1; i < n; ++i) {
        fv += sin(i*v[i]);
    }
    return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
    double r = 0.0;
    for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
    return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
    double integral = 0;
    double hx = (variable_lim[0][1] - variable_lim[0][0])/tn[0];
    double hy = (variable_lim[1][1] - variable_lim[1][0])/tn[1];
    double hz = (variable_lim[2][1] - variable_lim[2][0])/tn[2];
    double x = variable_lim[0][0];
    double y;
    double z;

    for (int i = 0; i < tn[0]; i++){
        y = variable_lim[1][0];

        for (int j = 0; j < tn[1]; j++){
            z = variable_lim[2][0];

            for (int k = 0; k < tn[2]; k++){
                double v[] = {x + hx, y + hy, z + hz};
                if (boundary == NULL || boundary(v, 3)) integral += f(v, 3) * hx * hy * hz;
                z += hz;
            }
            y += hy;
        }
        x += hx;
    }
    return integral;
}

// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
double right_point(FuncNvFp f, int n, double wspolrzedne[], double steps[], const int tn[], int idx_iterowania, BoundNvFp boundary)
{
    // printf("idx: %d\n %f %f %f\n", idx_iterowania, wspolrzedne[0], wspolrzedne[1], wspolrzedne[2]);

    double initial_wspolrzedne[n];
    for (int i = 0; i<n; i++){
        initial_wspolrzedne[i] = wspolrzedne[i];
    }
    double result = 0;
    for (int i = 0; i < tn[idx_iterowania]; i++)
    {
        if (idx_iterowania == n-1)
        {
            // printf("idx: %d\n %f %f %f\n", idx_iterowania, wspolrzedne[0], wspolrzedne[1], wspolrzedne[2]);
            if (!boundary || boundary(wspolrzedne, n) == 1){
                // printf("%f\n", f(wspolrzedne, n));
                result += f(wspolrzedne, n) * steps[idx_iterowania];
                // printf("res %f\n", result);
            }
        }
        else
        {
            result += right_point(f, n,wspolrzedne, steps, tn, idx_iterowania + 1, boundary) * steps[idx_iterowania];
            //printf("result %f\n", result);
        }

        wspolrzedne[idx_iterowania] += steps[idx_iterowania];
        // printf("po zwiekszeniu %f o %f\n", wspolrzedne[idx_iterowania], steps[idx_iterowania]);
    }
    for (int i=idx_iterowania; i<n; i++){
        // printf("zerowanie\n");
        wspolrzedne[i] = initial_wspolrzedne[i];
    }

    return result;
}



// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
double recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[],
                           double variable_lim[][2], const int tn[], int level, BoundNvFp boundary)
{
    // nie rekurencyjna funkcja, totalnie
    int WYMIAR = variable_no;
    double wspolrzedne[WYMIAR];
    double steps[WYMIAR];
    for (int i=0; i<WYMIAR; i++){
        steps[i] = (variable_lim[i][1] - variable_lim[i][0]) / tn[0];
        wspolrzedne[i] = (variable_lim[i][0] + variable_lim[i][0] + steps[i])/2;
    }
    return right_point(f, WYMIAR, wspolrzedne, steps, tn, 0, boundary);

}

//0.98941
//7
//4
//0 1 10
//0 1 10
//0 1 10
//0 1 10
//1

int main(void) {
    int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
    int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
    int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
    double a, b, x1, x2, y1, y2, hy, sum, delta;
    double t_variable[N_MAX], variable_lim[N_MAX][2];
    int tn[N_MAX];

    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // loop over quadratures and integrands
            scanf("%lf %lf %d", &a, &b, &n);
            for(int q = 0; q < no_quads; ++q) {
                for(int f = 0; f < no_funcs; ++f) {
                    printf("%.5f ",quad_select(f, q, a, b, n));
                }
                printf("\n");
            }
            break;
        case 2: // adaptive algorithm
            scanf("%d %d",&integrand_fun_no,&method_no);
            scanf("%lf %lf %lf", &a, &b, &delta);
            printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
            break;
        case 3: // double integral over a rectangle
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
            break;
        case 4: // double integral over normal domain
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf", &hy);
            printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
            break;
        case 5: // double integral over multiple normal domains
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
            break;
        case 6: // triple integral over a cuboid
            scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
            scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
            scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
            scanf("%d", &flag);
            printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
            break;
        case 7: // multiple integral over hyper-cuboid
            scanf("%d", &n);
            if(n > N_MAX) break;
            for(int i = 0; i < n; ++i) {
                scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
            }
            scanf("%d", &flag);
            sum = 0.;
            double result = recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL);
            printf("%.5f\n", result);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }
    return 0;
}

