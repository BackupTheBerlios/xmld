#ifndef HAVE_SOSEL_H
#define HAVE_SOSEL_H

xmld_status_t sosel_init(void);
xmld_status_t sosel_shutdown(void);
xmld_status_t sosel_sadd(int);
xmld_status_t sosel_sremove(int);
xmld_status_t sosel_add(int, char*);
xmld_status_t sosel_remove(int);
void sosel_run(void*);

#endif /* HAVE_SOSEL_H */
