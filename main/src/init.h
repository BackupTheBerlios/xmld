#ifndef HAVE_INIT_H
#define HAVE_INIT_H

/* A structure representing a part that needs initialization */
struct xmld_part {
 xmld_status_t (*init_func) (void); /* initialization function */
 xmld_status_t (*shutdown_func) (void); /* shutdown function */
 int ok; /* whether initilization went fine */
};

void init_create_part(struct xmld_part*, xmld_status_t (*) (void), xmld_status_t (*) (void));
void init_shutdown_parts(void);
#endif /* HAVE_INIT_H */
