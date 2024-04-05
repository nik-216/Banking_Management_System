// banking management system using Trees (BST)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct transactions
{
    long long int acc_no;
    int trans;
    struct transactions *next;

} TRANS;

typedef struct account
{
    long long int acc_no;
    int cust_id;
    int balance;
    char fname[20];
    char lname[20];
    long long int phone;
    TRANS *head;
    TRANS *tail;
    struct account *left;
    struct account *right;

} ACC;

typedef struct tree
{
    ACC *root;

} TREE;

typedef struct admin
{
    char ad_name[10];
    char ad_id[7];

} ADMIN;

void tree_init(TREE *pt);
void load_cust_info(TREE *pt, FILE *cust);
void load_trans_info(TREE *pt, FILE *trans);
void load_admin_info(ADMIN ad[], FILE *admin);
void add_transactions(ACC *self_account, long long int other_account, int amt);
void insert(TREE *pt, long long int acc_no, int cid, char fname[], char lname[], long long int ph, int bal);
ACC *search(TREE *pt, long long int ele);
void inorderTraversal(TREE *pt);
void inorder(ACC *p);
void display_details(ACC *p);
ACC *create_acc(long long int acc_no, int cid, char fname[], char lname[], long long int phone, int bal);
void destroy(ACC *p);
void destroyTree(TREE *pt);

void tree_init(TREE *pt)
{
    pt->root = NULL;
}

ACC *create_acc(long long int acc_no, int cid, char fname[], char lname[], long long int phone, int bal)
{
    ACC *newacc = malloc(sizeof(ACC));
    newacc->acc_no = acc_no;
    newacc->cust_id = cid;
    strcpy(newacc->fname, fname);
    strcpy(newacc->lname, lname);
    newacc->phone = phone;
    newacc->balance = bal;
    newacc->head = NULL;
    newacc->tail = NULL;
    newacc->left = NULL;
    newacc->right = NULL;
    return newacc;
}

void display_details(ACC *p)
{
    printf("\n\nAccount Number:%lld\n", p->acc_no);
    printf("Customer ID:%d\n", p->cust_id);
    printf("Customer Name:%s %s\n", p->fname, p->lname);
    printf("Phone No:%lld\n", p->phone);
    printf("Balance:%d\n", p->balance);

    TRANS *x;
    printf("Transactions: \n");
    for (x = p->head; x != NULL; x = x->next)
        printf("\t %lld -> %d ", x->acc_no, x->trans);
    printf("\n");
}

void load_cust_info(TREE *pt, FILE *cust)
{
    int row = 0, column;
    char buffer[1024], fname[20], lname[20];
    int cid, balance;
    long long int acc_no, ph;

    while (fgets(buffer, 1024, cust))
    {
        column = 0;
        row++;

        // To avoid printing of column headers
        if (row == 1)
            continue;

        // Splitting the data
        const char *value = strtok(buffer, ",");

        while (value)
        {
            // acc_no number
            if (column == 0)
            {
                acc_no = atoll(value);
                value = strtok(NULL, ",");
                column++;
            }

            // customer id
            if (column == 1)
            {
                cid = atoi(value);
                value = strtok(NULL, ",");
                column++;
            }

            // first name
            if (column == 2)
            {
                strcpy(fname, value);
                value = strtok(NULL, ",");
                column++;
            }

            // last name
            if (column == 3)
            {
                strcpy(lname, value);
                value = strtok(NULL, ",");
                column++;
            }

            // Phone
            if (column == 4)
            {
                ph = atoll(value);
                value = strtok(NULL, ",");
                column++;
            }

            // balance
            if (column == 5)
            {
                balance = atoi(value);
                value = strtok(NULL, ",");
            }

            insert(pt, acc_no, cid, fname, lname, ph, balance);
        }
    }
}

void add_cust(TREE *pt, FILE *fp_cust)
{
    char fname[20], lname[20];
    long long int phone = 0, n1, lan = 10000000000, uan = 99999999999;
    int n2, lci = 1000000, uci = 9999999, bal = 1000;
    printf("\t\t...........................................\n\n");
    printf("Enter First Name: ");
    scanf("%s", fname);
    printf("Enter Last Name: ");
    scanf("%s", lname);

    printf("Enter a phone number: ");
    scanf("%lld", &phone);
    while (phone < 7000000000 || phone > 9999999999)
    {
        printf("--Invalid Phone number--\n");
        printf("Enter a valid number: ");
        scanf("%lld", &phone);
        printf("\n");
    }

    printf("Enter amount to be deposited: ");
    scanf("%d", &bal);
    while (bal < 1000)
    {
        printf("\t\tMinimum amount is Rs 1000\n");
        printf("Enter a valid amount: ");
        scanf("%d", &bal);
    }

    printf("\t\tYour Account Number: ");
    do
    {
        n1 = (rand() % (uan - lan + 1) + lan);

    } while (search(pt, n1));

    // n1=(rand()%(uan-lan+1)+lan);
    printf("%lld\n", n1);

    printf("\t\tCustomer ID: ");
    n2 = (rand() % (uci - lci + 1) + lci);
    printf("%d\n", n2);

    insert(pt, n1, n2, fname, lname, phone, bal);

    printf("\n\t\tAccount created successfully\n");
    printf("\t\t...........................................\n");
    // printf("\nYOUR ACCOUNT NUMBER:%d",n1);
    // printf("\nCustomer ID:%d",n2);
}

ACC *deleteR(ACC *p, long long int acc_no)
{
    ACC *q = NULL;
    if (p == NULL)
        return p;
    if (acc_no < p->acc_no)
        p->left = deleteR(p->left, acc_no);
    else if (acc_no > p->acc_no)
        p->right = deleteR(p->right, acc_no);
    else if (p->left == NULL) // Single right child or leaf ACC case
    {
        q = p->right;
        free(p);
        return q;
    }
    else if (p->right == NULL) // Single left child case
    {
        q = p->left;
        free(p);
        return q;
    }
    else // ACC with 2 children, Replace with inorder successor
    {
        q = p->right;
        while (q->left != NULL)
            q = q->left;

        p->acc_no = q->acc_no;
        p->right = deleteR(p->right, q->acc_no);
    }
    return p;
}

void deleteACC(TREE *pt, long long int ele)
{
    pt->root = deleteR(pt->root, ele);
}

void add_transactions(ACC *self_account, long long int other_account, int amt)
{
    TRANS *newNode = malloc(sizeof(TRANS));
    newNode->acc_no = other_account;
    newNode->trans = amt;
    newNode->next = NULL;

    if (self_account->head == NULL)
    {
        self_account->head = newNode;
        self_account->tail = newNode;
    }
    else
    {
        self_account->tail->next = newNode;
        self_account->tail = newNode;
    }
}

void phone_update(ACC *acc)
{
    long long int new_ph;
    printf("Please enter your new Phone No. :");
    scanf("%lld", &new_ph);
    while (new_ph < 7000000000 || new_ph > 9999999999)
    {
        printf("\nInvalid Phone number\n");
        printf("Please enter your new Phone No. :");
        scanf("%lld", &new_ph);
    }
    acc->phone = new_ph;
    printf("---Phone number updated---");
}

void make_transactions(TREE *pt, ACC *send_acc)
{
    int amt;
    long long int r_acc;
    printf("\nEnter Amount to be Transfered - ");
    scanf("%d", &amt);
    printf("Enter Account number - ");
    scanf("%lld", &r_acc);

    if (amt < 0 || amt > send_acc->balance)
    {
        printf("--Transaction Invalid--\n");
        return;
    }
    ACC *rec_acc = search(pt, r_acc);

    add_transactions(send_acc, rec_acc->acc_no, (-amt));
    send_acc->balance -= amt;

    if (rec_acc != NULL)
    {
        add_transactions(rec_acc, send_acc->acc_no, amt);
        rec_acc->balance += amt;
    }

    printf("Transaction Completed\n");
    printf("%d send to %lld", amt, r_acc);
}

void load_trans_info(TREE *pt, FILE *trans)
{
    // format - <account send from> <-ve amt>
    //          <amount send to> <+ve amt>
    char buffer[1024];
    while (fgets(buffer, 1024, trans)) // reads line
    {
        ACC *acc = NULL;
        int row = 0, transa;
        long long int acc_no, o_acc_no;
        const char *value = strtok(buffer, ",");

        while (value)
        {
            if (row == 0) // reads acc id
            {
                acc_no = atoll(value);
                acc = search(pt, acc_no);
                row++;
            }
            else if (row % 2)
            {
                o_acc_no = atoll(value);
                row++;
            }
            else // reads transactions
            {
                transa = atoi(value);
                add_transactions(acc, o_acc_no, transa);
                row++;
            }
            value = strtok(NULL, ",");
        }
    }
}

void load_admin_info(ADMIN ad[], FILE *admin)
{
    int row = -2, column;
    char buffer[1024], name[20], id[10];

    while (fgets(buffer, 1024, admin))
    {
        column = 0;
        row++;

        // To avoid printing of column headers
        if (row == -1)
            continue;

        // Splitting the data
        const char *value = strtok(buffer, ",");

        while (value)
        {
            // name
            if (column == 0)
            {
                strcpy(ad[row].ad_name, value);
                value = strtok(NULL, ",");
                column++;
            }

            // admin id
            if (column == 1)
            {
                strcpy(ad[row].ad_id, value);
                ad[row].ad_id[strlen(ad[row].ad_id) - 1] = '\0';
                value = strtok(NULL, ",");
                column++;
            }
        }
    }
    // printf("HO");
}

// adding new acc_no give bal as 1000 in arg

void insert(TREE *pt, long long int acc, int cid, char fname[], char lname[], long long int ph, int bal)
{

    ACC *newacc = create_acc(acc, cid, fname, lname, ph, bal);
    if (pt->root == NULL)
        pt->root = newacc;
    else
    {
        ACC *p = pt->root;
        ACC *q = NULL;

        while (p != NULL)
        {
            q = p;
            if (newacc->acc_no < p->acc_no)
                p = p->left;
            else
                p = p->right;
        }
        if (newacc->acc_no < q->acc_no)
            q->left = newacc;
        else
            q->right = newacc;
    }
}

ACC *search(TREE *pt, long long int ele)
{
    ACC *p = pt->root;

    while (p != NULL)
    {
        if (ele == p->acc_no)
            return p;
        else if (ele < p->acc_no)
            p = p->left;
        else
            p = p->right;
    }
    return NULL;
}

void inorder(ACC *p)
{
    if (p != NULL)
    {
        inorder(p->left);
        display_details(p);
        inorder(p->right);
    }
}

void inorderTraversal(TREE *pt)
{
    if (pt->root != NULL)
        inorder(pt->root);
    else
        printf("Empty Tree\n");
}

void write_cust(ACC *account, FILE *fp_cust)
{
    if (account == NULL)
        return;

    fprintf(fp_cust, "%lld, %d,%s,%s, %lld, %d\n", account->acc_no, account->cust_id, account->fname, account->lname, account->phone, account->balance);
    write_cust(account->left, fp_cust);
    write_cust(account->right, fp_cust);
}

void write_trans(ACC *account, FILE *fp_trans)
{
    if (account == NULL)
        return;

    fprintf(fp_trans, "%lld", account->acc_no);

    TRANS *x;
    for (x = account->head; x != NULL; x = x->next)
        fprintf(fp_trans, ", %lld, %d", x->acc_no, x->trans);
    fprintf(fp_trans, "\n");
    write_trans(account->left, fp_trans);
    write_trans(account->right, fp_trans);
}

void write(TREE *pt)
{
    FILE *fp_cust = fopen("Customer.csv", "w");
    FILE *fp_trans = fopen("transactions.csv", "w");

    fprintf(fp_cust, "Account Number, Customer ID, First_Name,Last_Name, Phone, Balance\n");
    write_cust(pt->root, fp_cust);

    write_trans(pt->root, fp_trans);

    fclose(fp_cust);
    fclose(fp_trans);
}

void destroy(ACC *p)
{

    // makes accounts free
    if (p != NULL)
    {

        destroy(p->left);
        destroy(p->right);
        // makes tranactions free
        TRANS *x;
        for (x = p->head; p->head != NULL;)
        {
            p->head = p->head->next;
            free(x);
            x = p->head;
        }
        free(p);
    }
}

void destroyTree(TREE *pt)
{
    if (pt->root != NULL)
        destroy(pt->root);
}

int main()
{
    FILE *fp_cust = fopen("Customer.csv", "r");
    FILE *fp_trans = fopen("transactions.csv", "r");
    FILE *fp_admin = fopen("Admin.csv", "r");
    // getchar();
    TREE tobj;
    // memset(&tobj, 0, sizeof(TREE));
    tree_init(&tobj);
    ADMIN *admin = calloc(3, sizeof(ADMIN));
    // ADMIN *admin=malloc(3*sizeof(ADMIN));

    if ((!fp_cust) || (!fp_trans) || (!fp_admin))
        return 0;

    // printf("all files opened\n");
    load_cust_info(&tobj, fp_cust); // wurks lessgoooo
    load_trans_info(&tobj, fp_trans);
    load_admin_info(admin, fp_admin); // works!!

    fclose(fp_admin);
    fclose(fp_cust);
    fclose(fp_trans);
    // selecting between admin or customer

    while (1)
    {
        printf("\n\nCustomer login(0) or Admin login(1) or Customer Signup(2) or logout(-1)? : ");
        int log_who;
        scanf("%d", &log_who);
        printf("\n\n");
        int logout = 0;
        if (log_who == 0)
        {
            int choice, cid;
            int logout = 0;
            long long int acc;
            printf("Please Enter Account Number: ");
            scanf("%lld", &acc);
            printf("Please Enter Customer ID: ");
            scanf("%d", &cid);
            ACC *p = search(&tobj, acc);
            if (p != NULL)
            {
                if (cid == p->cust_id)
                {
                    int op;
                    printf("\n\t\t~~~~~~~~~~Welcome %s %s!~~~~~~~~~~", p->fname, p->lname);
                    do
                    {
                        printf("\n\n1.See your details\n2.Transfer money\n3.Check Balance\n4.Delete Account\n5.Update Ph. Number\n6.Logout\n");
                        printf("What would you like to do?: ");
                        scanf("%d", &op);
                        printf("\t\t--------------------------------\n");
                        switch (op)
                        {
                        case 1:
                            display_details(p);
                            break;
                        case 2:
                            make_transactions(&tobj, p);
                            break;

                        case 3:
                            printf("\nBalance - %d", p->balance);
                            break;

                        case 4:
                            printf("Deleting Acoount ......\n");
                            // nishta's delete account
                            deleteACC(&tobj, acc);
                            printf("Deleted \nLOGINING OUT ");
                            logout = 1;
                            break;

                        case 5:
                            phone_update(p);
                            break;

                        default:
                            logout = 1;
                            break;
                        }
                        printf("\n\t\t--------------------------------\n");
                    } while (!logout);
                }
            }
            else
            {
                printf("Account Number is incorrect.");
            }
        }

        else if (log_who == 1)
        {
            // nishta not working
            int incorrect = 1, log = 0;
            char a_id[7];
            char a_name[10];
            printf("Please enter Name: ");
            scanf("%s", a_name);
            printf("Please Enter Admin ID: ");
            scanf("%s", a_id);
            // printf("%s %s\n",a_id,a_name);
            int i;
            ADMIN *p = admin;
            for (i = 0; i < 3; i++)
            {
                // printf("%s\n---",p[i].ad_id);
                if (!strcmp(p[i].ad_id, a_id))
                {

                    // printf("id okay\n");
                    if (!strcmp(p[i].ad_name, a_name))
                    {
                        // printf("name okay\n");
                        incorrect = 0;
                        break;
                    }
                    else
                    {
                        printf("Incorrect Admin Name\n");
                    }
                }
            }
            if (incorrect == 1)
            {
                printf("Incorrect Admin ID\n");
            }
            else
            {
                printf("\n\t<<<<<<<<<<<<<<<<  Welcome %s!  >>>>>>>>>>>>>>>>\n", p[i].ad_name);
                do
                {
                    printf("\t\tHere are all the Customer Details: \n");
                    inorderTraversal(&tobj);
                    printf("logout?(1): ");
                    scanf("%d", &log);
                } while (!log);
                printf("\n\t\t=========================================\n");
            }
        }

        else if (log_who == 2)
        {
            // nishta's create account
            add_cust(&tobj, fp_cust);
        }

        else if (log_who == -1)
        {
            break;
        }
    }

    // inorderTraversal(&tobj); // works
    write(&tobj);
    // printf("A-okay");
    destroyTree(&tobj);
    // printf("A-okay");
    return 0;
}
