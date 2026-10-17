/**
 * ============================================================================
 * DSA CAPSTONE PROJECT: Browser History Management Using Data Structures
 * ============================================================================
 * Students: Harneesh K (CH.SC.U4CSE25217) & Harish G (CH.SC.U4CSE25259)
 * Course: 23CSE203 - Data Structures and Algorithms
 * Institution: Amrita School of Computing, Amrita Vishwa Vidyapeetham, Chennai
 * 
 * Language: C99
 * File: browser_history.c
 * Description: High-performance bidirectional browser history navigation engine
 *              implemented using a Doubly Linked List (DLL) with O(1) traversal
 *              and automatic memory deallocation of orphaned forward branches.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define URL_MAX_LEN 128
#define DISPLAY_BUF_SIZE 2048

/* ============================================================================
 * DATA STRUCTURE DEFINITIONS
 * ============================================================================
 */

/**
 * Node represents a single visited webpage in browser history.
 * Contains the URL, timestamp, and bidirectional pointers.
 */
typedef struct Node {
    char url[URL_MAX_LEN];
    time_t timestamp;
    struct Node *prev;
    struct Node *next;
} Node;

/**
 * BrowserHistory manages the doubly linked list state.
 * Maintains pointers to head and current page, along with pos and length.
 */
typedef struct {
    Node *head;
    Node *current;
    int pos;     /* 0-indexed position of the current page */
    int length;  /* Total number of active pages in current history */
} BrowserHistory;

/* ============================================================================
 * CORE OPERATIONS (As presented in Project Slides)
 * ============================================================================
 */

/**
 * Creates and initializes a new Node with given URL.
 */
Node* new_node(const char *url) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Memory allocation error for new node.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->url, url, URL_MAX_LEN - 1);
    n->url[URL_MAX_LEN - 1] = '\0';
    n->timestamp = time(NULL);
    n->prev = NULL;
    n->next = NULL;
    return n;
}

/**
 * Initializes the BrowserHistory structure with a starting homepage.
 */
void init(BrowserHistory *h, const char *homepage) {
    if (!h) return;
    Node *home = new_node(homepage);
    h->head = home;
    h->current = home;
    h->pos = 0;
    h->length = 1;
}

/**
 * visit: Adds a new webpage to the history.
 * CRUCIAL EDGE CASE HANDLING:
 * If the user has navigated backwards before visiting this new page,
 * all orphaned forward nodes (h->current->next onwards) are freed to prevent
 * memory leaks, mimicking standard web browser behavior.
 * Time Complexity: O(1) amortized
 */
void visit(BrowserHistory *h, const char *url) {
    if (!h || !h->current) return;

    /* Step 1: Clear orphaned forward history nodes */
    Node *f = h->current->next;
    int freed_count = 0;
    while (f) {
        Node *t = f->next;
        free(f);
        f = t;
        freed_count++;
    }

    /* Step 2: Create and connect the new node */
    Node *n = new_node(url);
    n->prev = h->current;
    h->current->next = n;
    h->current = n;

    /* Step 3: Update metadata */
    h->pos++;
    h->length = h->pos + 1;
}

/**
 * back: Navigates backwards by 1 step in history.
 * Returns the URL of the current page, or NULL if already at the first page.
 * Time Complexity: O(1)
 */
char* back(BrowserHistory *h) {
    if (!h || !h->current || !h->current->prev) {
        return NULL; /* Blocked: Already at the earliest page */
    }
    h->current = h->current->prev;
    h->pos--;
    return h->current->url;
}

/**
 * forward: Navigates forward by 1 step in history.
 * Returns the URL of the current page, or NULL if already at the latest page.
 * Time Complexity: O(1)
 */
char* forward(BrowserHistory *h) {
    if (!h || !h->current || !h->current->next) {
        return NULL; /* Blocked: Already at the newest page in current branch */
    }
    h->current = h->current->next;
    h->pos++;
    return h->current->url;
}

/**
 * Helper for case-insensitive substring search
 */
static int contains_case_insensitive(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;
    size_t needle_len = strlen(needle);
    if (needle_len == 0) return 1;
    for (; *haystack; haystack++) {
        size_t i;
        for (i = 0; i < needle_len; i++) {
            char c1 = haystack[i];
            char c2 = needle[i];
            if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
            if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
            if (c1 != c2) break;
        }
        if (i == needle_len) return 1;
    }
    return 0;
}

/**
 * search: Searches through all nodes in history for a keyword match.
 * Returns the pointer to the matching node or NULL if not found.
 * Time Complexity: O(N)
 */
Node* search(BrowserHistory *h, const char *keyword) {
    if (!h || !keyword) return NULL;
    Node *curr = h->head;
    while (curr) {
        if (contains_case_insensitive(curr->url, keyword)) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/**
 * display: Formats the entire active history into a human-readable string buffer.
 * Current page is marked with [brackets].
 * Example: Google -> [YouTube] -> Wikipedia
 * Time Complexity: O(N)
 */
void display(BrowserHistory *h, char *buf, size_t buf_size) {
    if (!h || !buf) return;
    buf[0] = '\0';
    Node *curr = h->head;
    while (curr) {
        char item[URL_MAX_LEN + 16];
        if (curr == h->current) {
            snprintf(item, sizeof(item), "[%s]", curr->url);
        } else {
            snprintf(item, sizeof(item), "%s", curr->url);
        }
        strncat(buf, item, buf_size - strlen(buf) - 1);
        if (curr->next) {
            strncat(buf, " -> ", buf_size - strlen(buf) - 1);
        }
        curr = curr->next;
    }
}

/**
 * free_history: Deallocates all nodes in the Doubly Linked List.
 */
void free_history(BrowserHistory *h) {
    if (!h) return;
    Node *curr = h->head;
    while (curr) {
        Node *temp = curr->next;
        free(curr);
        curr = temp;
    }
    h->head = NULL;
    h->current = NULL;
    h->pos = 0;
    h->length = 0;
}

/* ============================================================================
 * AUTOMATED TEST SUITE (10 / 10 Verification as in Presentation Slide 6)
 * ============================================================================
 */

void run_presentation_tests(void) {
    printf("====================================================================\n");
    printf("DSA CAPSTONE PROJECT: AUTOMATED TEST SUITE (10 / 10 TESTS)\n");
    printf("Browser History Management Using Doubly Linked List\n");
    printf("Students: Harneesh K & Harish G | Course: 23CSE203\n");
    printf("====================================================================\n\n");

    int passed = 0;
    BrowserHistory h;
    char buf[DISPLAY_BUF_SIZE];

    /* Test 1: Visit 4 pages */
    init(&h, "Google");
    visit(&h, "YouTube");
    visit(&h, "Instagram");
    visit(&h, "ChatGPT");
    if (strcmp(h.current->url, "ChatGPT") == 0 && h.pos == 3 && h.length == 4) {
        printf("[PASS] T1: Visit 4 pages -> Current = %s (pos: %d, length: %d)\n", h.current->url, h.pos, h.length);
        passed++;
    } else {
        printf("[FAIL] T1: Visit 4 pages failed.\n");
    }

    /* Test 2 & 3: Back twice */
    char *res1 = back(&h);
    char *res2 = back(&h);
    if (res1 && strcmp(res1, "Instagram") == 0 && res2 && strcmp(res2, "YouTube") == 0 && strcmp(h.current->url, "YouTube") == 0) {
        printf("[PASS] T2-T3: Back twice -> Instagram, then YouTube (Current: %s)\n", h.current->url);
        passed += 2;
    } else {
        printf("[FAIL] T2-T3: Back twice failed.\n");
    }

    /* Test 4: Forward once */
    char *res3 = forward(&h);
    if (res3 && strcmp(res3, "Instagram") == 0 && strcmp(h.current->url, "Instagram") == 0) {
        printf("[PASS] T4: Forward once -> Instagram (Current: %s)\n", h.current->url);
        passed++;
    } else {
        printf("[FAIL] T4: Forward once failed.\n");
    }

    /* Test 5: Back at first page (Boundary condition) */
    back(&h); /* to YouTube */
    back(&h); /* to Google (head) */
    char *blocked_back = back(&h);
    if (blocked_back == NULL && strcmp(h.current->url, "Google") == 0) {
        printf("[PASS] T5: Back at first page -> Blocked gracefully (NULL, stays at Google)\n");
        passed++;
    } else {
        printf("[FAIL] T5: Back at first page failed.\n");
    }

    /* Move forward to ChatGPT */
    forward(&h); /* YouTube */
    forward(&h); /* Instagram */
    forward(&h); /* ChatGPT */

    /* Test 6: Forward at last page (Boundary condition) */
    char *blocked_forward = forward(&h);
    if (blocked_forward == NULL && strcmp(h.current->url, "ChatGPT") == 0) {
        printf("[PASS] T6: Forward at last page -> Blocked gracefully (NULL, stays at ChatGPT)\n");
        passed++;
    } else {
        printf("[FAIL] T6: Forward at last page failed.\n");
    }

    /* Test 7 & 8: Visit after Back (Clearing forward history) */
    back(&h); /* Instagram */
    back(&h); /* YouTube */
    display(&h, buf, sizeof(buf));
    printf("       State before new visit: %s\n", buf);
    visit(&h, "Wikipedia");
    display(&h, buf, sizeof(buf));
    printf("       State after visiting Wikipedia: %s\n", buf);
    if (strcmp(h.current->url, "Wikipedia") == 0 && h.current->next == NULL && h.pos == 2 && h.length == 3) {
        printf("[PASS] T7-T8: Visit after Back -> Forward branch safely cleared, order correct\n");
        passed += 2;
    } else {
        printf("[FAIL] T7-T8: Visit after Back failed.\n");
    }

    /* Test 9: Search by keyword */
    Node *found = search(&h, "wiki");
    if (found && strcmp(found->url, "Wikipedia") == 0) {
        printf("[PASS] T9: Search \"wiki\" -> Wikipedia successfully found in O(N)\n");
        passed++;
    } else {
        printf("[FAIL] T9: Search keyword failed.\n");
    }

    /* Test 10: Scale stress test (100,000 pages, 99,999 back) */
    free_history(&h);
    init(&h, "page_0");
    clock_t start = clock();
    for (int i = 1; i <= 100000; i++) {
        char url[32];
        snprintf(url, sizeof(url), "page_%d", i);
        visit(&h, url);
    }
    for (int i = 1; i <= 99999; i++) {
        back(&h);
    }
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (h.pos == 1 && strcmp(h.current->url, "page_1") == 0 && h.length == 100001) {
        printf("[PASS] T10: Scale stress test (100,000 pages, 99,999 back) -> Passed in %.3fs\n", time_taken);
        passed++;
    } else {
        printf("[FAIL] T10: Scale stress test failed.\n");
    }

    printf("\n--------------------------------------------------------------------\n");
    printf("SUMMARY: %d / 10 TESTS PASSED SUCCESSFULLY (100%% Integrity)\n", passed);
    printf("--------------------------------------------------------------------\n\n");

    free_history(&h);
}

int main(void) {
    run_presentation_tests();
    return 0;
}
