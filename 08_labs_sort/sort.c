#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// it doesn't working 
typedef struct {
    int day;
    int month;
    int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
    char name[20];
    float price;
    int amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
    Date *d11 = (Date * )d1;
    Date *d22 = (Date * )d2;

    if(d11->year != d22->year) return d11->year - d22->year;
    if(d11->month != d22->month) return d11->month - d22->month;
    return d11->day - d22->day;
}

// compare foods
int cmp(const void *a, const void *b) {
    Food *aa = (Food *) a;
    Food *bb = (Food *) b;

    if ((int)(aa->price * 100) - (int)(bb->price * 100) != 0){
        return (int)(aa->price * 100) - (int)(bb->price * 100);
    }
    int date_cmp = (aa->valid_date.day + aa->valid_date.month * 30 + aa->valid_date.year * 365) -
                   (bb->valid_date.day + bb->valid_date.month * 30 + bb->valid_date.year * 365);
    if (date_cmp !=0){
        return date_cmp;
    }
    else{
        return strcmp(aa->name, bb->name);
    }
}

// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result) {
    int low = 0;
    int high = (int) nmemb;
    int mid, curr_state;
    int is_found = 0;
    while (low <= high){
        mid = low + (high - low)/2;
        curr_state = compar(key, &base[mid]);
        if (curr_state == 0){
            is_found = 1;
            break;
        }
        if (curr_state > 1){
            high = mid - 1;
        }
        else{
            low = mid + 1;
        }
    }
    if (is_found){
        *result = '1';
    }
    else{
        *result = '0';
    }
    return (void *)&base[low];


}

// print goods of given name
void print_art(Food *p, int n, char *art) {
    for(int i = 0; i < n; i++){
        if((strcmp(p[i].name, art)) == 0){
            if (p[i].valid_date.day < 10){
                printf("%.2f %d 0%d.", p[i].price, p[i].amount, p[i].valid_date.day);
            }
            else{
                printf("%.2f %d %d.", p[i].price, p[i].amount, p[i].valid_date.day);
            }
            if (p[i].valid_date.month < 10){
                printf("0%d.%d\n", p[i].valid_date.month, p[i].valid_date.year);
            }
            else{
                printf("%d.%d\n", p[i].valid_date.month, p[i].valid_date.year);
            }
        }
    }
}

// add record to table if absent
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
    char result;
    if (*np == 0){
        tab = new;//?
        *np += 1;
        return tab;//?
    }
    Food *ptr = bsearch2(new, tab, *np, sizeof (Food), compar, &result);
    if (result == '1'){
        ptr->amount += new->amount;
    }
    else{
        int index = ptr - tab;
        memmove(ptr+1, ptr, (*np-index)*sizeof(Food));
        memcpy(ptr, new, sizeof(Food));
        *np = *np + 1;
    }

}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    int cnt = 0;
    for (int i = 0; i < no; i++){
        Food new;
        fscanf(stream, "%s %f %d %d.%d.%d", new.name, &new.price, &new.amount, &new.valid_date.day,
              &new.valid_date.month, &new.valid_date.year);
        if (sorted){
            add_record(tab, &cnt, cmp, &new);
        }
        else {
            tab[cnt] = new;
            cnt++;
        }
    }

}

int cmp_qs(const void *a, const void *b) {
    Food *fa = (Food*)a, *fb = (Food*)b;
    return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
    Date *pd = (Date*)a;
    Food *fb = (Food*)b;
    return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
    qsort(food_tab, n, sizeof(Food), cmp_qs);
    struct tm cdate = {0};
    cdate.tm_year = curr_date.year - 1900;
    cdate.tm_mon = curr_date.month - 1;
    cdate.tm_mday = curr_date.day;
    time_t ctime = mktime(&cdate);

    float val = 0.0;

    for(int i=0; i < n; i++){
        struct tm fdate = {0};
        fdate.tm_year = food_tab[i].valid_date.year - 1900;
        fdate.tm_mon = food_tab[i].valid_date.month - 1;
        fdate.tm_mday = food_tab[i].valid_date.day;
        time_t ftime = mktime(&fdate);

        if(ctime + days *24*60*60 == ftime){
            val+=food_tab[i].amount * food_tab[i].price;
        }
    }

    return val;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL in_line:1;
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
    if (person1->bits.sex == person2->bits.sex) return 0;
    if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
    if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
    return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
    for(int i=0; i<n; i++){
        if(person_tab[i].bits.in_line == no){
            memmove(&person_tab[i],&person_tab[i+1],(n-(i+1))*sizeof(Person));
            n-=1;
            i-=1;
        }
    }
    return n;
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
    qsort(person_tab, n, sizeof (Person), cmp_bs);
}

int main(void) {
    Person person_tab[] =
            {
            {"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
            {"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
            {"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
            {"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
            {"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
            {"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
            {"William", {M,yes}, {21, 6, 1982}, "Charles III"},
            {"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
            {"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
            {"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
            {"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
            {"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
            {"James", {M,yes}, {17, 12, 2007}, "Edward"},
            {"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
            {"Charles", {M,yes}, {1, 7, 1999}, "David"},
            {"Margarita", {F,yes}, {14, 5, 2002}, "David"},
            {"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
            {"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
            {"George", {M,yes}, {22, 7, 2013}, "William"},
            {"George VI", {M,no}, {14, 12, 1895}, NULL},
            {"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
            {"Louis", {M,yes}, {23, 4, 2018}, "William"},
            {"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
            {"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
            {"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
            {"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
            {"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
            {"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
            {"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
            {"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
            {"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
            {"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
            {"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
    };

    int to_do, no;
    int n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    fgets(buff, ART_MAX, stdin);
    sscanf(buff, "%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 1);
            scanf("%s",buff);
            print_art(food_tab, n, buff);
            break;
        case 2: // qsort
            printf("this probably doesn't work");
//            fgets(buff, ART_MAX, stdin);
//            sscanf(buff, "%d", &no);
//            n = read_goods(food_tab, no, stdin, 0);
//            Date curr_date;
//            int days;
//            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
//            scanf("%d", &days);
//            printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
            break;
        case 3: // succession
            scanf("%d",&no);
//              printf("i will do it later");
//            int no_persons = sizeof(person_tab) / sizeof(Person);
//            create_list(person_tab,no_persons);
//            print_person(person_tab + no - 1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
